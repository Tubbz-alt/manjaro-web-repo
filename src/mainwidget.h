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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <WContainerWidget>

using namespace Wt;


class MainWidget : public WContainerWidget
{
public:
    MainWidget();

private:
  Wt::WNavigationBar *navigation;
  Wt::WStackedWidget *contentsStack;

  Wt::WMenuItem *addToMenu(Wt::WMenu *menu,
                           const Wt::WString& name,
                           WContainerWidget *widget);
};

#endif // MAINWIDGET_H
