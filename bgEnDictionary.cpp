#include "bgEnDictionary.h"
#include "myFileRoutines.h"
#include "showMessage.h"

BgEnDictionary::BgEnDictionary(const QString &dn){
   dirName = dn;
};

QString BgEnDictionary::translation(const QString &w){
   if (!w.size()) return "";
   ushort uc = w.at(0).toLower().unicode() - 1071;
   QString fn = QString::number(uc);
   if (uc<10) fn = "0"+fn;
   fn = dirName + "d" + fn + ".txt";
   QString tx = fileContent(fn);
   int i;
   if (tx.startsWith(w+"\n") ) i = 0;
   else i = tx.indexOf("\n"+w+"\n");
   if (i>-1){
      int j = tx.indexOf("\n\n",i);
      QString r = tx.mid(i,j-i);
      return r;
   }
   return "";
};
