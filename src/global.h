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


#ifndef GLOBAL_H
#define GLOBAL_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <curl.h>
#include <boost/thread.hpp>

#include <WStandardItemModel>
#include <WStandardItem>


#include "const.h"

using namespace std;
using namespace Wt;


class Global
{
public:
    enum STATE { STATE_UP_TO_DATE, STATE_OUT_OF_DATE, STATE_UNKOWN };

    struct RepoState {
        string url, country, protocol;
        long lastSync;
        map<string, STATE> states;
    };

    static void init();
    static void cleanup();

    static vector<Global::RepoState> getRepoStates();
    static void setRepoStates(const vector<Global::RepoState> & repoStates);

    static bool executeCommand(const string & cmd, const string & workingDir);
    static string getWebContent(const string & url);

private:
    static boost::mutex repoStatesMutex;
    static vector<RepoState> repoStates;

    static size_t data_write(void* buf, size_t size, size_t nmemb, void* userp);
    static CURLcode curl_read(const string & url, ostream& os, long timeout = 30);
};

#endif // GLOBAL_H
