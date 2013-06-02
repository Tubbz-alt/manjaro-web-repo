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
    QList<Global::RepoState> repoStates;
    QMap<QString, QString> currentBranchHashStates;
    const QStringList branches = QString(REPO_BRANCHES).split(' ', QString::SkipEmptyParts);
    QDateTime currentDateTime = QDateTime::currentDateTime();


    /* Get current branch hash state */
    for (int i = 0; i < branches.size(); i++) {
        const QString branch = branches.at(i);
        QFile file(QString(REPO_PATH) + "/" + branch + "/state");

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::cerr << QString("error: failed to open file '%1'!").arg(file.fileName()).toUtf8().data() << std::endl;
            return;
        }

        QTextStream in(&file);

        while (!in.atEnd()) {
            QString line = in.readLine().split('#', QString::KeepEmptyParts).first().trimmed();

            if (!line.startsWith("state") || !line.contains('='))
                continue;

            currentBranchHashStates[branch] = line.split('=', QString::SkipEmptyParts).last().trimmed();
        }
    }


    /* Get git source */
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);

    QString tmpPath = QString(TMP_PATH) + "/git";
    QStringList args;

    if (QDir(tmpPath).exists()) {
        process.setWorkingDirectory(tmpPath + "/" + GIT_NAME);
        args << "pull" << "origin" << "master";
    }
    else {
        QDir().mkpath(tmpPath);
        process.setWorkingDirectory(tmpPath);
        args << "clone" << GIT_URL;
    }

    process.start("git", args);

    if (!process.waitForFinished()) {
        std::cerr << "error: failed to start git process" << std::endl;
        return;
    }

    if (process.exitCode() != 0) {
        std::cerr << "error: git process failed" << std::endl;
        std::cerr << "message: " << QString(process.readAll()).toUtf8().data() << std::endl;
        return;
    }


    /* Get mirrors and time */
    const QStringList files = QDir(tmpPath + "/" + MIRRORS_PATH).entryList(QDir::Files | QDir::NoDotAndDotDot);

    for (int i = 0; i < files.size(); i++) {
        const QString filePath = tmpPath + "/" + MIRRORS_PATH + "/" + files.at(i);

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::cerr << QString("error: failed to open file '%1'!").arg(filePath).toUtf8().data() << std::endl;
            continue;
        }

        QString country = "unknown";
        QTextStream in(&file);

        while (!in.atEnd()) {
            QString line = in.readLine().split('#', QString::KeepEmptyParts).first().trimmed();
            if (line.isEmpty())
                continue;

            if (line.startsWith('[') && line.endsWith(']')) {
                country = line.mid(1, line.length() - 2);
            }
            else if (line.startsWith("Server") && line.contains('=')) {
                Global::RepoState repo;
                repo.country = country;
                repo.url = line.split('=', QString::SkipEmptyParts).last().trimmed();

                for (int i = 0; i < branches.size(); i++) {
                    const QString branch = branches.at(i);
                    repo.states[branch] = getRepoBranchState(QString(repo.url).replace("$branch/$repo/$arch", branch + "/state"),
                                                             currentBranchHashStates.value(branch, ""));
                }

                repo.url = repo.url.mid(0, repo.url.indexOf("/$branch"));
                repo.lastSync = getRepoLastSync(repo.url + "/state", currentDateTime);

                if (repo.url.startsWith("http://"))
                    repo.protocol = "http";
                else if (repo.url.startsWith("ftp://"))
                    repo.protocol = "ftp";
                else
                    repo.protocol = "unknown";

                repoStates.append(repo);
            }
        }

        file.close();
    }

    Global::setRepoStates(repoStates);
}



long BackgroundWorker::getRepoLastSync(const QString & url, const QDateTime & currentDateTime) {
    QStringList split = QString::fromStdString(Global::getWebContent(url.toStdString())).split('\n', QString::SkipEmptyParts);
    QString repoDateTimeStr;

    for (int i = 0; i < split.size(); i++) {
        QString line = split.at(i).split('#', QString::KeepEmptyParts).first().trimmed();

        if (!line.startsWith("date") || !line.contains('='))
            continue;

        repoDateTimeStr = line.split('=', QString::SkipEmptyParts).last().trimmed();
    }

    if (repoDateTimeStr.isEmpty())
        return -1;

    QDateTime repoDateTime = QDateTime::fromString(repoDateTimeStr, Qt::ISODate);

    if (repoDateTime > currentDateTime)
        return -1;

    return repoDateTime.secsTo(currentDateTime) / 60;
}



Global::STATE BackgroundWorker::getRepoBranchState(const QString & url, const QString & currentBranchHashState)
{
    QStringList split = QString::fromStdString(Global::getWebContent(url.toStdString())).split('\n', QString::SkipEmptyParts);
    QString hashState;

    for (int i = 0; i < split.size(); i++) {
        QString line = split.at(i).split('#', QString::KeepEmptyParts).first().trimmed();

        if (!line.startsWith("state") || !line.contains('='))
            continue;

        hashState = line.split('=', QString::SkipEmptyParts).last().trimmed();
    }

    if (hashState.isEmpty())
        return Global::STATE_UNKOWN;
    else if (hashState == currentBranchHashState)
        return Global::STATE_UP_TO_DATE;
    else
        return Global::STATE_OUT_OF_DATE;
}







