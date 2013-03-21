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

#include <QMessageBox>
#include <QString>
#include <QWidget>
#include <QApplication>
#include <QStatusBar>

#include "showMessage.h"

void showMessage(const QString& ms, const QString &t){
   QString title("Qt application");
   QWidget *aw = qApp->activeWindow();
   if (t.size()) title = t;
   else if (aw) title = aw->windowTitle();
   QMessageBox::information(aw,title,ms);
}

void showMessage(int i, const QString &t){
   showMessage(QString::number(i),t);
};

// показване на не повече от msgMax съобщения
void countedMessage(const QString &m){
   static int msgCount = 0;
   const int msgMax = 8;
   if (msgCount<msgMax) showMessage(m);
   msgCount++;
}

void countedMessage(int &m){
   countedMessage(QString::number(m));
};

QStatusBar *myStatusBar = 0;

void setMyStatusBar(QStatusBar *sb){
   myStatusBar = sb;
};

void showStatus(const QString &ms){
   if (myStatusBar){
      myStatusBar->showMessage(ms);
      myStatusBar->repaint();
   }
};

