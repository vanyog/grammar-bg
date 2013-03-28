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

#ifndef MYFILERUTINES_H
#define MYFILERUTINES_H

#include <QString>
#include <QFileInfo>

QFileInfo myFileInfo(const QString &fn);
QString myDirName(const QString &fn);
QString myStatusDir(const QString &fn);

QString fileContent(const QString &fn, const QString &codec = "cp1251");
QString fileContent(QFile &fl, const QString &codec = "cp1251");

void saveToFile(const QString &fn, const QString &fc, const QString &codec = "cp1251");
void appendToFile(const QString &fn, const QString &fc, const QString &codec = "cp1251");

#endif
