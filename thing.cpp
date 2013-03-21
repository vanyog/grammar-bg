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

#include "thing.h"

//------Thing-------

Thing::~Thing(){};

QString Thing::name(){
   return "";
};

QString Thing::value(){
   return "";
};

QString Thing::value(const QString &n){
   if (name()==n) return value();
   for(int i=0; i<size(); i++){
      QString r = at(i)->value(n);
      if (r.size()) return r;
   }
   return "";
};

bool Thing::is(const QString &v, const QString &n){
   bool r = ( (n.size()==0)||(n==name()) ) && (v==value());
   if (r) return r;
   for(int i=0;i<size();i++){
      r = at(i)->is(v,n);
      if (r) return r;
   }
   return false;
};

QString Thing::toString(){
   QString r = name();
   QString v = value();
   if (r.size()) r.append(" - ").append(v);
   else r=v;
   for(int i=0;i<size();i++){
      if (r.size()) r.append("<br>");
      r.append(at(i)->toString());
   }
   return r;
}

//------ThingValue-------

ThingValue::ThingValue(const QString &v){
   strValue = v;
}

QString ThingValue::value(){
   return strValue;
};

//------ThingNamedValue-------

ThingNamedValue::ThingNamedValue(const QString &n, const QString &v){
   strName=n; strValue = v;
}

QString ThingNamedValue::name(){
   return strName;
};

QString ThingNamedValue::value(){
   return strValue;
};


