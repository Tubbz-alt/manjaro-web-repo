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

#include "repositorystatepage.h"



RepositoryStatePage::RepositoryStatePage() :
    WContainerWidget()
{
    /*
     * Table View
     */
    tableView = new WTableView(this);
    model = new WStandardItemModel(this);
    tableView->setSelectable(true);
    tableView->setColumnResizeEnabled(true);
    tableView->setAlternatingRowColors(true);
    tableView->setRowHeight(28);
    tableView->setHeaderHeight(28);
    tableView->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);

    updateRepositoryStates();

    addWidget(tableView);


    /*
     * Refresh Button
     */
    buttonRefresh = new Wt::WPushButton("Refresh", this);
    buttonRefresh->clicked().connect(this, &RepositoryStatePage::buttonRefresh_clicked);
    addWidget(buttonRefresh);
}



void RepositoryStatePage::updateRepositoryStates() {
    // Get branches
    vector<string> branches;
    boost::split(branches, REPO_BRANCHES, boost::is_any_of(" "), boost::token_compress_on);

    // Get repo states
    const vector<Global::RepoState> repoStates = Global::getRepoStates();

    // Clear model
    model->clear();

    /*
     * Set header data
     */
    int headerColumn = 0;
    model->insertColumns(0, 4);
    model->setHeaderData(headerColumn, Wt::Horizontal, WString("Mirror"));
    model->setHeaderData(++headerColumn, Wt::Horizontal, WString("Country"));
    model->setHeaderData(++headerColumn, Wt::Horizontal, WString("Protocol"));
    model->setHeaderData(++headerColumn, Wt::Horizontal, WString("Last sync (hh:mm)"));

    for (unsigned int x = 0; x < branches.size(); x++) {
        model->insertColumns(++headerColumn, 1);
        model->setHeaderData(headerColumn, Wt::Horizontal, WString(branches.at(x)));
    }


    /*
     * Set repo content
     */
    if (repoStates.empty()) {
        WStandardItem *mirrorItem = new WStandardItem("Loading repository states...");
        model->setItem(0, 0, mirrorItem);
    }

    for (unsigned int i = 0; i < repoStates.size(); i++) {
        const Global::RepoState *repo = &repoStates.at(i);
        int column = 0;

        // Mirror
        WStandardItem *mirrorItem = new WStandardItem(repo->url);
        model->setItem(i, column, mirrorItem);

        // Country
        WStandardItem *item = new WStandardItem(repo->country);
        model->setItem(i, ++column, item);

        // Protocol
        item = new WStandardItem(repo->protocol);
        model->setItem(i, ++column, item);

        // Last sync
        STATE mirrorItemState = STATE_NORMAL;
        item = new WStandardItem(minutesToString(repo->lastSync));

        if (repo->lastSync < 240) {          // 4 hours
            mirrorItemState = STATE_YELLOW;
            item->setStyleClass("tableView_yellow");
        }
        else if (repo->lastSync >= 240 && repo->lastSync < 420) {     // 7 hours
            mirrorItemState = STATE_ORANGE;
            item->setStyleClass("tableView_orange");
        }
        else if (repo->lastSync >= 420) {
            mirrorItemState = STATE_RED;
            item->setStyleClass("tableView_red");
        }

        model->setItem(i, ++column, item);


        for (unsigned int x = 0; x < branches.size(); x++) {
            const string branch = branches.at(x);

            Global::STATE state = Global::STATE_UNKOWN;

            if (repo->states.find(branch) != repo->states.end() )
                state = repo->states.at(branch);

            item = new WStandardItem("unknown");

            if (state == Global::STATE_UP_TO_DATE) {
                item->setText("up-to-date");
            }
            else if (state == Global::STATE_OUT_OF_DATE) {
                item->setText("out-of-date");

                if (mirrorItemState == STATE_YELLOW)
                    item->setStyleClass("tableView_yellow");
                else if (mirrorItemState == STATE_ORANGE)
                    item->setStyleClass("tableView_orange");
                else if (mirrorItemState == STATE_RED)
                    item->setStyleClass("tableView_red");
            }

            model->setItem(i, ++column, item);
        }


        if (mirrorItemState == STATE_YELLOW)
            mirrorItem->setStyleClass("tableView_yellow");
        else if (mirrorItemState == STATE_ORANGE)
            mirrorItem->setStyleClass("tableView_orange");
        else if (mirrorItemState == STATE_RED)
            mirrorItem->setStyleClass("tableView_red");
    }

    tableView->setModel(model);


    /*
     * Set column width
     */
    int column = 0;
    tableView->setColumnWidth(column, 400);
    tableView->setColumnWidth(++column, 150);
    tableView->setColumnWidth(++column, 100);
    tableView->setColumnWidth(++column, 150);

    for (unsigned int i = 0; i < branches.size(); i++)
        tableView->setColumnWidth(++column, 100);

    tableView->setWidth(828 + 107 * branches.size());
}



string RepositoryStatePage::minutesToString(long minutes) {
    if (minutes < 0)
        return "unknown";

    const int hours = minutes / 60;
    minutes -= hours * 60;

    string final;

    if (hours < 10)
        final = "0" + to_string(hours);
    else
        final = to_string(hours);

    final += ":";

    if (minutes < 10)
        final += "0" + to_string(minutes);
    else
        final += to_string(minutes);

    return final;
}



void RepositoryStatePage::buttonRefresh_clicked() {
    updateRepositoryStates();
}
