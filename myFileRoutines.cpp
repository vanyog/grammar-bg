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

#include "myFileRoutines.h"
#include "showMessage.h"

#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QApplication>

QString myDirName(const QString &fn){
 return fn.left(fn.lastIndexOf("/"));
};

QString myFileName(const QString &fn){
 return fn.mid(fn.lastIndexOf("/")+1);
};

QFileInfo myFileInfo(const QString &fn){
 QStringList nf;
 nf << myFileName(fn);
 QDir dir(myDirName(fn));
 QFileInfoList fil = dir.entryInfoList(nf);
 if (fil.size()) return fil.at(0);
 else return QFileInfo();
};

QString myStatusDir(const QString &fn){
 QString dp = qApp->applicationDirPath();
 QString fp = dp; fp.append("/").append(fn);
 
 QFileInfo fi = myFileInfo(fp);
 if (fi.exists() && !fi.isWritable()) 
  showMessage(QString("File is not writable %1").arg(fp));
 return fp;
};

QString fileContent(const QString &fn, const QString &codec){
   QFile file(fn);
   if (!file.open(QFile::ReadOnly | QFile::Text)) {
      showMessage(QApplication::tr("Cannot read file %1:\n%2.")
         .arg(fn)
         .arg(file.errorString())
      );
      return "";
   }
   QTextStream in(&file);
   QApplication::setOverrideCursor(Qt::WaitCursor);
   in.setCodec(QTextCodec::codecForName(codec.toLatin1()));
   QString r = QString(in.readAll());
   file.close();
   QApplication::restoreOverrideCursor();
   return r;
};

QString fileContent(QFile &fl, const QString &codec){
   if (!fl.open(QFile::ReadOnly | QFile::Text)) {
      showMessage(QApplication::tr("Cannot read file %1:\n%2.")
         .arg("fl.fileName()")
         .arg(fl.errorString())
      );
      return "";
   }
   QTextStream in(&fl);
   QApplication::setOverrideCursor(Qt::WaitCursor);
   in.setCodec(QTextCodec::codecForName(codec.toLatin1()));
   QString r = in.readAll(); //showMessage(r);
   fl.close();
   QApplication::restoreOverrideCursor();
   return r;
};

void saveToFile(const QString &fn, const QString &fc, const QString &codec){
   QFile file(fn);
   if (!file.open(QFile::WriteOnly | QFile::Text)) {
      showMessage(QApplication::tr("Cannot write to file: %1.\n%2.")
         .arg(fn)
         .arg(file.errorString())
      );
      return;
   }
   QTextStream in(&file);
   QApplication::setOverrideCursor(Qt::WaitCursor);
   in.setCodec(QTextCodec::codecForName(codec.toLatin1()));
   in << fc;
   file.close();
   QApplication::restoreOverrideCursor();
};

void appendToFile(const QString &fn, const QString &fc, const QString &codec){
   QFile file(fn);
   if (!file.open(QIODevice::Append | QFile::Text)) {
      showMessage(QApplication::tr("Cannot write to file: %1.\n%2.")
         .arg(fn)
         .arg(file.errorString())
      );
      return;
   }
   QTextStream in(&file);
   QApplication::setOverrideCursor(Qt::WaitCursor);
   in.setCodec(QTextCodec::codecForName(codec.toLatin1()));
   in << fc;
   file.close();
   QApplication::restoreOverrideCursor();
};

