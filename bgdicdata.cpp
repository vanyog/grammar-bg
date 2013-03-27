/*
grammar-bg - open source Bulgarian grammar
Copyright (C) 2012  Vanyo Georgiev <info@vanyog.com>

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

#include <QString>
#include <QTextCodec>

#include "grammar.h"
#include "showMessage.h"
#include "myFileRoutines.h"

namespace BGDicdata{

QString dD;
QHash<int, Thing *> props; // Двойките име-стойност от таблица w_properties
QHash<int, Thing *> forms; // Групи от двойки име-стойност (форми) от таблица w_forms
QHash<int, WordForms *> tables; // Таблици за образуване форми на думи от таблица w_tables
QHash<int, Thing *> tableP; // Свойства на таблиците за образуване форми на думи от таблица w_table_props
LangDictionary *cDic = 0; // Речник, в който се добавят думите


// Помощна функция, която разделя ред, от csv файл на QStringList от стойности 
QStringList *splitLine(const QString &l){
  QStringList *sl = new QStringList();
  QStringList s = l.trimmed().split(",");
  for(int i=0; i<s.size(); i++){
    QString q = s.at(i).trimmed();
    if (q.at(0)==QString("\"").at(0)) q = q.mid(1,q.size()-2);
    sl->append(q);
  }
  return sl;
}

// Зареждане на двойките име-стойност от файл w_properties.csv
void loadProps(){
  QString fn = dD+"/w_properties.csv";
  cDic->setLastModified(QFileInfo(fn).lastModified());
  QStringList fc = fileContent(fn,"UTF-8").split("\n");
  for(int i=0; i<fc.size(); i++){
    QStringList *p = splitLine(fc.at(i));
    if (p->size()==4){
      ThingNamedValue *t = new ThingNamedValue(p->at(1),p->at(2));
      props[p->at(0).toInt()] = t;
    }
  }
}

// Зареждане на групите двойки име-стойност (формите) от файл w_forms.csv
void loadForms(){
  QString fn = dD+"/w_forms.csv";
  cDic->setLastModified(QFileInfo(fn).lastModified());
  QStringList fc = fileContent(fn,"UTF-8").split("\n");
  for(int i=0; i<fc.size(); i++){
    QStringList *p = splitLine(fc.at(i));
    if (p->size()==4){
      int j = p->at(2).toInt(); // Номер на група свойства (форма)
      int v = p->at(3).toInt(); // Номер на двойка име-стойност от таблица w_properties
      Thing *t = props.value(v);
      if (t){
        if (!forms.value(j)) forms[j] = new ThingNamedValue(t->name(),t->value());
        else forms[j]->append(t);
      }
    }
  }
}

// Зареждане на таблиците за образуване форми на думите от файл w_tables.csv
void loadTables(){
  QString fn = dD+"/w_tables.csv";
  cDic->setLastModified(QFileInfo(fn).lastModified());
  QStringList fc = fileContent(fn,"UTF-8").split("\n");
  for(int i=0; i<fc.size(); i++){
    QStringList *p = splitLine(fc.at(i));
    if (p->size()==6){
      WordSuffix *f = new WordSuffix(p->at(3).toInt(),p->at(4)); // Нова словоформа с окончание
      int j = p->at(2).toInt();  // Номер на таблицата
      int k = p->at(5).toInt();  // Номер на групата свойства
      Thing *t = forms.value(k); // Указател към групата свойства
      f->append(t);              // Добавяне на групата свойства към словоформата с окончание
       // Създаване на таблица, ако не съществува
      if (!tables.value(j)){ tables[j] = new WordForms(); tables[j]->append(f); }
      tables[j]->append(f);      // Добавяне на словоформата към таблицата
    }
  }
}

// Зареждане на общите свойства на таблиците за форми на думите от файл w_table_props.csv
void loadTableProps(){
  QString fn = dD+"/w_table_props.csv";
  cDic->setLastModified(QFileInfo(fn).lastModified());
  QStringList fc = fileContent(fn,"UTF-8").split("\n");
  for(int i=0; i<fc.size(); i++){
    QStringList *p = splitLine(fc.at(i));
    if (p->size()==5){
      int j = p->at(1).toInt(); // Номер на таблицата
      int k = p->at(2).toInt(); // Номер на група свойства
      Thing *t = new ThingNamedValue("Таблица",QString::number(j));
      tableP[j] = t;
      tableP[j]->append(forms.value(k));
    }
  }
}

// Зареждане на основните форми на думите от файл w_words.csv или от друг файл
void loadMainForms(const QString &f = "w_words.csv"){
  QString fn = dD+"/"+f;
  cDic->setLastModified(QFileInfo(fn).lastModified());
  QStringList fc = fileContent(fn,"UTF-8").split("\n");
  for(int i=0; i<fc.size(); i++){
    QStringList *p = splitLine(fc.at(i));
    if (p->size()==5){
      int j = p->at(2).toInt(); // Номер на таблицата на думата
      WordRoot *r = new WordRoot(p->at(1),tables.value(j)); // Създаване на коренова форма
      if (tableP.value(j)) r->append(tableP.value(j)); // Добавяне на общите свойства към кореновата форма
      cDic->append(r);  // Добавяне на кореновата форма в речника
    }
  }
}

// Зареждане на речник от директория dicDir в речник langDic
void loadTo(const QString &dicDir, LangDictionary *langDic){
  dD = dicDir; 
  cDic = langDic;
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  loadProps(); //showMessage(props.size());
  loadForms(); //showMessage(forms.size());
  loadTables(); //showMessage(tables.size());
  loadTableProps(); //showMessage(tableP.size());
  loadMainForms();
  loadMainForms("w_words_local.csv");
};

};
