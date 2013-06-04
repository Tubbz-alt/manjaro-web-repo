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


#ifndef CONST_H
#define CONST_H

#define TMP_PATH "/tmp/manjaro-web-repo_tmp"
#define GIT_URL "git://git.manjaro.org/core/pacman-mirrors.git"
#define GIT_NAME "pacman-mirrors"
#define MIRRORS_PATH "pacman-mirrors/data/mirrors"
#define REPO_PATH "/var/repo/repo"
#define REPO_BRANCHES "stable testing unstable"
#define DOWNLOAD_CMD "curl -s -L --connect-timeout 30"

#endif // CONST_H
