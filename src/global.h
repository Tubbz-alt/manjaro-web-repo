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

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <curl/curl.h>

#include <QString>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>

#include <Wt/WStandardItemModel>

#include "const.h"

using namespace Wt;


class Global
{
public:
    enum STATE { STATE_UP_TO_DATE, STATE_OUT_OF_DATE, STATE_UNKOWN };

    struct RepoState {
        QString url, country, protocol;
        long lastSync;
        QMap<QString, STATE> states;
    };

    static void init();
    static void cleanup();

    static QList<Global::RepoState> getRepoStates();
    static void setRepoStates(const QList<Global::RepoState> & repoStates);

    static std::string getWebContent(const std::string & url);

private:
    static QMutex repoStatesMutex;
    static QList<RepoState> repoStates;

    static size_t data_write(void* buf, size_t size, size_t nmemb, void* userp);
    static CURLcode curl_read(const std::string& url, std::ostream& os, long timeout = 30);
};

#endif // GLOBAL_H
