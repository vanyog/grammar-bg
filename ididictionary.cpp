/* 
grammar-bg - open source Bulgarian grammar
Copyright (C) 2010  Vanyo Georgiev <info@vanyog.com>

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

#include "grammar.h"
#include "showMessage.h"
#include "myFileRoutines.h"

#include <QDir>
#include <QTextStream>
#include <QTextCodec>
#include <QApplication>
#include <QDateTime>

namespace IDIDictionary{

void loadTo(const QString &dicDir, LangDictionary *langDic){
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  langDic->setRootIndexFileName("roots-idi.dat");
  langDic->setFormIndexFileName("forms-idi.dat");
  langDic->setLastModified(QFileInfo(dicDir).lastModified());
  QStringList fc = fileContent(dicDir).split("\n");
  QHash<QString,WordRoot*> rh;
  for(int i=1; i<fc.size(); i++){
    QStringList ln = fc.at(i).trimmed().split(" ");
    if (ln.size()==4){
      QString f = ln.at(0).trimmed(); // форма на думата
      QString r = ln.at(1).trimmed(); // основна форма
      QString tn = ln.at(2).trimmed(); // номер на таблицата за образуване на форми
      QString fn = ln.at(3).trimmed(); // номер на формата в таблицата
      int j=0;
      int n = r.size(); if (n<f.size()) n=f.size(); // по-малката от двете дължини - на формата и на основната форма
      while( (f.at(j)==r.at(j)) && (j<n) ) j++;  // намира номера на първата различна буква
      n = r.size()-j;                            // брой букви в края, които трябва да се заменят с окончание
      QString sf = f.right(f.size()-j);          // окончание
      WordSuffix *wf = new WordSuffix(n,sf);     // создава се нова словоформа
      wf->append( new ThingNamedValue("номер форма",fn) );
      WordRoot *wr = rh.value(r);       // намира корена, ако вече е създаден
      if (!wr){                         // създаване на корен, ако още не е създаден
        WordForms *wfs = new WordForms; // нов списък от словоформи
        wr = new WordRoot(r,wfs);       // нов корен
        wr->append( new ThingNamedValue("таблица",tn) );
        rh[r] = wr;
      }
      wr->forms()->append(wf);              // добавя се формата на думата към корена
      if (fn=="0") wr->forms()->append(wf); // за съвместимост с bgoffice формата с номер 0 се вмъква още веднъж
//      showMessage(f+"<br /><br />"+wr->wordForms()->join(",")+"<br />"+wr->toString()+"<br />"+wr->form(f)->toString());
    }
  }
  QStringList k = rh.keys();
  for(int i=0; i<k.size(); i++){
     WordRoot *r = rh.value(k.at(i));
//     showMessage( r->size() );
     langDic->append(r);
  }
}

} // namespace IDIDictionary
