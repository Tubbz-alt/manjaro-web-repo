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

#ifndef BACKGROUNDWORKER_H
#define BACKGROUNDWORKER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <WDateTime>

#include "const.h"
#include "global.h"

using namespace std;
using namespace Wt;


class BackgroundWorker
{
public:
    BackgroundWorker();
    ~BackgroundWorker();

    void start();

private:
    boost::thread m_Thread;

    void run();
    void updateRepositoryStateList();
    long getRepoLastSync(const string & url, const WDateTime & currentDateTime);
    Global::STATE getRepoBranchState(const string & url, const string & currentBranchHashState);
};

#endif // BACKGROUNDWORKER_H
