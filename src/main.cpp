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

#include <iostream>

#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WHBoxLayout>
#include <Wt/WBootstrapTheme>
#include <Wt/WCssTheme>

#include "global.h"
#include "mainwidget.h"
#include "backgroundworker.h"


using namespace Wt;



WApplication *createApplication(const WEnvironment& env)
{
    Wt::WApplication* app = new Wt::WApplication(env);

    if (app->appRoot().empty())
        std::cerr << "warning: the approot looks suspect!" << std::endl;

    const std::string *theme = env.getParameter("theme");
    if (theme)
        app->setTheme(new Wt::WCssTheme(*theme));
    else
        app->setTheme(new Wt::WBootstrapTheme(app));


    Wt::WHBoxLayout *layout = new Wt::WHBoxLayout(app->root());
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(new MainWidget());

    app->setTitle("Manjaro Linux Repository");
    app->useStyleSheet("style/style.css");

    return app;
}



int main(int argc, char **argv)
{
    // Initialize
    Global::init();

    // Start our background worker
    BackgroundWorker worker;
    worker.start();

    // Main Application
    int ret = WRun(argc, argv, &createApplication);

    // Cleanup;
    Global::cleanup();

    return ret;
}
