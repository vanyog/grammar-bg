/*
grammar-bg - open source Bulgarian grammar
Copyright (C) 2007  Vanyo Georgiev <info@vanyog.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <QStringListModel>

#include "largelistview.h"
#include "showMessage.h"

LargeListView::LargeListView(QWidget *parent)
   :QWidget(parent)
{
   ui.setupUi(this);
};

void LargeListView::setList(QStringList *ls){
   sList = ls;
   if (sList!=0) ui.listView->setModel( new QStringListModel(*sList) );
};

QListView *LargeListView::listView(){
   return ui.listView;
};

QLineEdit *LargeListView::lineEdit(){
   return ui.lineEdit;
};

int LargeListView::count(){
   QModelIndex mi = listView()->model()->index(0,0,QModelIndex());
   return listView()->model()->rowCount(mi);
};

int LargeListView::index(){
   return listView()->currentIndex().row();
};

QString LargeListView::item(int i){
   QModelIndex mi = listView()->model()->index(i,0,QModelIndex());
   return listView()->model()->data(mi).toString();
};

