/*
 *  ManjaroWebRepo
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

#include "mainwidget.h"

#include <WHBoxLayout>
#include <WMenu>
#include <WNavigationBar>
#include <WStackedWidget>
#include <WText>
#include <WVBoxLayout>

#include "repositorystatepage.h"



MainWidget::MainWidget() :
    WContainerWidget()
{
    setOverflow(OverflowHidden);

    navigation = new Wt::WNavigationBar();
    navigation->addStyleClass("main-nav");
    navigation->setTitle("Manjaro Repository");
    navigation->setResponsive(true);

    contentsStack = new Wt::WStackedWidget();
    Wt::WAnimation animation(Wt::WAnimation::Fade,
                             Wt::WAnimation::Linear,
                             200);
    contentsStack->setTransitionAnimation(animation, true);

    /*
     * Setup the top-level menu
     */
    Wt::WMenu *menu = new Wt::WMenu(contentsStack, 0);
    menu->setInternalPathEnabled();
    menu->setInternalBasePath("/");

    addToMenu(menu, "Repository State", new RepositoryStatePage());

    navigation->addMenu(menu);

    /*
     * Add it all inside a layout
     */
    Wt::WVBoxLayout *layout = new Wt::WVBoxLayout(this);
    layout->addWidget(navigation);
    layout->addWidget(contentsStack, 1);
    layout->setContentsMargins(0, 0, 0, 0);
}



Wt::WMenuItem* MainWidget::addToMenu(Wt::WMenu *menu,
                                     const Wt::WString& name,
                                     WContainerWidget *widget)
{
  widget->addStyleClass("content");
  widget->setOverflow(WContainerWidget::OverflowAuto);

  Wt::WMenuItem *item = new Wt::WMenuItem(name, widget);
  menu->addItem(item);

  return item;
}
