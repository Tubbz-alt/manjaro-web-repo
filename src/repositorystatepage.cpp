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
    model = new WStandardItemModel(tableView);
    tableView = new WTableView(this);
    tableView->setSelectable(true);
    tableView->setColumnResizeEnabled(true);
    tableView->setAlternatingRowColors(true);
    tableView->setRowHeight(28);
    tableView->setHeaderHeight(28);
    tableView->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);

    updateRepositoryStates();

    int column = 0;
    tableView->setColumnWidth(column, 400);
    tableView->setColumnWidth(++column, 150);
    tableView->setColumnWidth(++column, 100);
    tableView->setColumnWidth(++column, 150);

    const int branches = model->columnCount() - column - 1;
    for (int i = 0; i < branches; i++)
        tableView->setColumnWidth(++column, 100);

    tableView->setWidth(828 + 107 * branches);

    addWidget(tableView);


    /*
     * Refresh Button
     */
    buttonRefresh = new Wt::WPushButton("Refresh", this);
    buttonRefresh->clicked().connect(this, &RepositoryStatePage::buttonRefresh_clicked);
    addWidget(buttonRefresh);
}



void RepositoryStatePage::updateRepositoryStates() {
    const QStringList branches = QString(REPO_BRANCHES).split(' ', QString::SkipEmptyParts);
    const QList<Global::RepoState> repoStates = Global::getRepoStates();
    model->clear();

    // Set header data
    int headerColumn = 0;
    model->insertColumns(0, 4);
    model->setHeaderData(headerColumn, Wt::Horizontal, WString("Mirror"));
    model->setHeaderData(++headerColumn, Wt::Horizontal, WString("Country"));
    model->setHeaderData(++headerColumn, Wt::Horizontal, WString("Protocol"));
    model->setHeaderData(++headerColumn, Wt::Horizontal, WString("Last sync (hh:mm)"));

    for (int x = 0; x < branches.size(); x++) {
        model->insertColumns(++headerColumn, 1);
        model->setHeaderData(headerColumn, Wt::Horizontal, WString(branches.at(x).toStdString()));
    }

    // Set repo data
    for (int i = 0; i < repoStates.size(); i++) {
        const Global::RepoState *repo = &repoStates.at(i);
        int column = 0;

        // Mirror
        WStandardItem *mirrorItem = new WStandardItem(repo->url.toStdString());
        model->setItem(i, column, mirrorItem);

        // Country
        WStandardItem *item = new WStandardItem(repo->country.toStdString());
        model->setItem(i, ++column, item);

        // Protocol
        item = new WStandardItem(repo->protocol.toStdString());
        model->setItem(i, ++column, item);

        // Last sync
        item = new WStandardItem(minutesToString(repo->lastSync).toStdString());

        if (repo->lastSync >= 1440 && repo->lastSync < 2880) {  // 1 day
            item->setStyleClass("tableView_orange");
            mirrorItem->setStyleClass("tableView_orange");
        }
        else if (repo->lastSync >= 2880) {                      // 2 days
            item->setStyleClass("tableView_red");
            mirrorItem->setStyleClass("tableView_red");
        }

        model->setItem(i, ++column, item);


        for (int x = 0; x < branches.size(); x++) {
            const Global::STATE state = repo->states.value(branches.at(i), Global::STATE_UNKOWN);

            item = new WStandardItem("unknown");

            if (state == Global::STATE_UP_TO_DATE) {
                item->setText("up-to-date");
            }
            else if (state == Global::STATE_OUT_OF_DATE) {
                item->setText("out-of-date");
                item->setStyleClass("tableView_red");
                mirrorItem->setStyleClass("tableView_red");
            }

            model->setItem(i, ++column, item);
        }
    }

    tableView->setModel(model);
}



QString RepositoryStatePage::minutesToString(const long minutes) {
    if (minutes < 0)
        return "unknown";

    return QString("%1:%2").arg(QString::number(minutes / 60), QString::number(minutes));
}



void RepositoryStatePage::buttonRefresh_clicked() {
    updateRepositoryStates();
}
