/* Copyright (C) 2007  Vanyo Georgiev <info@vanyog.com>
*  
*  This program is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  as published by the Free Software Foundation; either version 2
*  of the License, or (at your option) any later version.
*  
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*  
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
*  USA. */

#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include "ui_searchDialog.h"
#include "grammar.h"
#include "largelistview.h"

class SearchDialog : public QDialog
{
 Q_OBJECT

public:
 SearchDialog(LargeListView *llView, QWidget *parent = 0);
 void resetProgressBar();

 QString fileName;
 bool contains();
 void setContains(bool ch);
 bool cLast();
 void setCLast(bool ch);
 QString sText();
 void setSText(QString t);

private slots:
 void onRadioButton();
 void onFindNext();
 void onSaveToFile();
 void notReady();

private:
 Ui::SearchDialog ui;
 LargeListView *largeListView;
 QString ss0;
 QStringList sw;
 QSet<QChar> cSet;
 int step100;

 void setupLisr();
 int findNext(int i, QStringList *sw);
 int findNext2(int i, QStringList *sw);
 int findNext3(int i, QStringList *sw);
};


#endif
