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


#include "global.h"

#include <iostream>
#include <QMutexLocker>
#include <Wt/WStandardItem>



QMutex Global::repoStatesMutex;
QList<Global::RepoState> Global::repoStates;




void Global::init() {
    curl_global_init(CURL_GLOBAL_ALL);
}



void Global::cleanup() {
    curl_global_cleanup();
}



QList<Global::RepoState> Global::getRepoStates() {
    QMutexLocker locker(&repoStatesMutex);

    return repoStates;
}



void Global::setRepoStates(const QList<Global::RepoState> & repoStates) {
    QMutexLocker locker(&repoStatesMutex);

    Global::repoStates.clear();
    Global::repoStates = repoStates;
}



std::string Global::getWebContent(const std::string & url)
{
    std::ostringstream oss;

    if(CURLE_OK == curl_read(url, oss))
        return oss.str();

    return "";
}




//###
//### Private
//###


size_t Global::data_write(void* buf, size_t size, size_t nmemb, void* userp)
{
    if(userp)
    {
        std::ostream& os = *static_cast<std::ostream*>(userp);
        std::streamsize len = size * nmemb;
        if(os.write(static_cast<char*>(buf), len))
            return len;
    }

    return 0;
}



CURLcode Global::curl_read(const std::string& url, std::ostream& os, long timeout)
{
    CURLcode code(CURLE_FAILED_INIT);
    CURL* curl = curl_easy_init();

    if(curl)
    {
        if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FILE, &os))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str())))
        {
            code = curl_easy_perform(curl);
        }
        curl_easy_cleanup(curl);
    }

    return code;
}





