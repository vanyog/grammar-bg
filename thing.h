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

#ifndef THING_H
#define THING_H

#include <QHash>
#include <QString>

class Thing; // Базов клас за представяне на обект, който се описва чрез
// набор от свойства, а всяко свойство е обект от същия клас

class ThingList:public QList<Thing*>{ // Списък от Thing*
};

class Thing:public ThingList{
public:
   virtual ~Thing();
   virtual QString name();
   virtual QString value();
   virtual QString value(const QString &n);
   bool is(const QString &v, const QString &n="");
   QString toString();
private:
};

class ThingValue:public Thing{ // обект, представляващ само текстова стойност
public:
   ThingValue(const QString &v);
   virtual QString value();
private:
   QString strValue;
};

class ThingNamedValue:public Thing{ // обект, представляващ свойство с име
public:
   ThingNamedValue(const QString &n, const QString &v);
   virtual QString name();
   virtual QString value();
private:
   QString strName,strValue;
};

#endif

