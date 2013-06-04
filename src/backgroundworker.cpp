/*
 *  manjaro-web-repo
 *  Roland Singer <roland@manjaro.org>
 *
 *  Copyright (C) 2007 Free Software Foundation, Inc.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "backgroundworker.h"



BackgroundWorker::BackgroundWorker()
{
}



BackgroundWorker::~BackgroundWorker()
{
}



void BackgroundWorker::start()
{
    m_Thread = boost::thread(&BackgroundWorker::run, this);
}



void BackgroundWorker::run()
{
    boost::posix_time::minutes sleepTime(15);

    while (true) {
        updateRepositoryStateList();

        // Sleep
        boost::this_thread::sleep(sleepTime);
    }
}



void BackgroundWorker::updateRepositoryStateList()
{
    vector<Global::RepoState> repoStates;
    map<string, string> currentBranchHashStates;
    WDateTime currentDateTime = WDateTime::currentDateTime();

    /* Get branches */
    vector<string> branches;
    boost::split(branches, REPO_BRANCHES, boost::is_any_of(" "), boost::token_compress_on);


    /* Get current branch hash state */
    for (unsigned int i = 0; i < branches.size(); i++) {
        const string branch = branches.at(i);
        const string filePath = string(REPO_PATH) + "/" + branch + "/state";

        ifstream file(filePath.c_str(), std::ios::in);

        if (!file.is_open()) {
            cerr << "error: failed to open file '" << filePath << "'!" << endl;
            return;
        }

        string line;

        while (!file.eof()) {
            getline(file, line);

            size_t pos = line.find_first_of('#');
            if (pos != string::npos)
                line.erase(pos);

            boost::trim(line);

            if (!boost::starts_with(line, "state") || string::npos == line.find("="))
                continue;

            line = line.substr(line.find('=') + 1, string::npos);
            boost::trim(line);

            currentBranchHashStates[branch] = line;
        }

        file.close();
    }


    /* Get git source */
    string cmd, workingDir;
    string tmpPath = string(TMP_PATH) + "/git";;

    if (boost::filesystem::exists(tmpPath)) {
        workingDir = tmpPath + "/" + string(GIT_NAME);
        cmd = "git pull origin master";
    }
    else {
        if (!boost::filesystem::create_directories(tmpPath)) {
            cerr << "error: failed to create git tmp path!" << endl;
            return;
        }

        workingDir = tmpPath;
        cmd = "git clone " + string(GIT_URL);
    }

    if (!Global::executeCommand(cmd, workingDir)) {
        cerr << "error: git process failed" << endl;
        return;
    }


    /* Get mirrors and time */
    boost::filesystem::directory_iterator end_itr;
    for(boost::filesystem::directory_iterator i(tmpPath + "/" + MIRRORS_PATH); i != end_itr; ++i)
    {
        // Skip if not a file
        if(!boost::filesystem::is_regular_file(i->status()))
            continue;

        ifstream file(i->path().string().c_str(), std::ios::in);
        if (!file.is_open()) {
            cerr << "error: failed to open file '" << i->path().string() << "'!" << endl;
            return;
        }

        string country = "unknown";
        string line;

        while (!file.eof()) {
            getline(file, line);

            size_t pos = line.find_first_of('#');
            if (pos != string::npos)
                line.erase(pos);

            boost::trim(line);

            if (boost::starts_with(line, "[") && boost::ends_with(line, "]")) {
                country = line.substr(1, line.length() - 2);
            }
            else if (boost::starts_with(line, "Server") && string::npos != line.find("=")) {
                if (string::npos == line.find("dacentec"))
                    continue;

                line = line.substr(line.find('=') + 1, string::npos);
                boost::trim(line);

                Global::RepoState repo;
                repo.country = country;
                repo.url = line;

                for (unsigned int i = 0; i < branches.size(); i++) {
                    const string branch = branches.at(i);

                    if (currentBranchHashStates.find(branch) == currentBranchHashStates.end() ) {
                        repo.states[branch] = Global::STATE_UNKOWN;
                        continue;
                    }

                    string url = repo.url;
                    boost::replace_last(url, "$branch/$repo/$arch", branch + "/state");

                    repo.states[branch] = getRepoBranchState(url, currentBranchHashStates.at(branch));
                }

                repo.url = repo.url.substr(0, repo.url.find("/$branch"));
                repo.lastSync = getRepoLastSync(repo.url + "/state", currentDateTime);

                if (boost::starts_with(repo.url, "http://"))
                    repo.protocol = "http";
                else if (boost::starts_with(repo.url, "ftp://"))
                    repo.protocol = "ftp";
                else
                    repo.protocol = "unknown";

                repoStates.push_back(repo);
            }
        }

        file.close();
    }

    Global::setRepoStates(repoStates);
}



long BackgroundWorker::getRepoLastSync(const string & url, const WDateTime & currentDateTime)
{
    vector<string> split;
    string webContent= Global::getWebContent(url);
    boost::trim(webContent);
    boost::split(split, webContent, boost::is_any_of("\n"), boost::token_compress_on);

    string repoDateTimeStr;

    for (unsigned int i = 0; i < split.size(); i++) {
        string line = split.at(i);
        line = line.substr(0, line.find('#'));
        boost::trim(line);

        if (!boost::starts_with(line, "date") || string::npos == line.find("="))
            continue;

        repoDateTimeStr = line.substr(line.find('=') + 1, string::npos);
        boost::trim(repoDateTimeStr);
    }

    if (repoDateTimeStr.empty())
        return -1;

    WDateTime repoDateTime = WDateTime::fromString(repoDateTimeStr, "yyyy-MM-dd'T'hh:mm:ss");

    if (repoDateTime > currentDateTime)
        return -1;

    return repoDateTime.secsTo(currentDateTime) / 60;
}



Global::STATE BackgroundWorker::getRepoBranchState(const string & url, const string & currentBranchHashState)
{
    vector<string> split;
    string webContent= Global::getWebContent(url);
    boost::trim(webContent);
    boost::split(split, webContent, boost::is_any_of("\n"), boost::token_compress_on);

    string hashState;

    for (unsigned int i = 0; i < split.size(); i++) {
        string line = split.at(i);
        line = line.substr(0, line.find('#'));
        boost::trim(line);

        if (!boost::starts_with(line, "state") || string::npos == line.find("="))
            continue;

        hashState = line.substr(line.find('=') + 1, string::npos);
        boost::trim(hashState);
    }

    if (hashState.empty())
        return Global::STATE_UNKOWN;
    else if (hashState == currentBranchHashState)
        return Global::STATE_UP_TO_DATE;
    else
        return Global::STATE_OUT_OF_DATE;
}







