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

#ifndef MAINFORM_H
#define MAINFORM_H

#include <QClipboard>

#include "grammar.h"
#include "largelistview.h"
#include "searchDialog.h"
#include "ui_mainform.h"

class QModelIndex;
class BgEnDictionary;

class MyMainWindow : public QMainWindow
{
   Q_OBJECT

public:
    // Директория с данни
    QString dataDir;
    QString helpDir;

   LangDictionary langDic, idiDic;
   MyMainWindow(QWidget *parent = 0);
   void spellCheckFile();
   void spellCheckFile(const QString &fn, const QString &codec = "cp1251");
   void setPList();

public slots:
   void changeRootsAndForms(bool yes);

private:
   Ui::MainWindow ui;
   LargeListView *llView;
   SearchDialog *searchDialog;
   WordRoot *cRoot;
   QStringList *cList();
   WordRootFormPairList *rfpList();
   WordRootFormPair *cRfp;
   BgEnDictionary *bgEnDic;
   BgEnDictionary *thesDic;
   bool wordAdded;
   
   void closeEvent(QCloseEvent *event);
   
   void showInfo(int i=0); // показва информация за думата с номер i от списъка
   void showFormList();
   void showByTab(int tab, int i);
   void writeSettings();
   void readSettings();

private slots:
   void onWordClicked(const QModelIndex &mi);
   void onWordFormClicked(const QModelIndex &mi);
   void onWordTextClicked(const QModelIndex &mi);
   void onWordEditChanged(const QString &tx);
   void onAddButtonPressed();
   void onFindButtonPressed();
   void onPNameChanged(const QString &n);
   void onTabChanged(int tab);
   void onAboutPr();
   void onDictionaryInfo();
   void onFileSaveAs();
   void onFileOpenDat();
   void onFileExploreData();
   void onFileDeleteIndexFiles();
   void onFileIDIDic();
   void onEditSearch();
   void onFontAction();
   void onAutoCSPellcheck();
   void onToolsSpellCheckFile();
   void onToolsSpellCheckFileUtf8();
   void onToolsSpellCheckClipboard();
   void onToolsContinueSpelling();
   void on_actionDetailed_help_triggered();
};

#endif

