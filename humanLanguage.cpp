#include "humanLanguage.h"

QSet<QChar> bgLetters(){
   static QSet<QChar> r;
   for(int i=0x410; i<=0x42A; i++) r << QChar(i);
   r << QChar(0x42C);
   for(int i=0x42E; i<=0x44A; i++) r << QChar(i);
   r << QChar(0x44C);
   for(int i=0x44E; i<=0x44F; i++) r << QChar(i);
   return r;
};

QSet<QChar> bgPMarks(){
   static QSet<QChar> r;
   r << QChar('.') << QChar(',') << QChar('-') << QChar('(') << QChar(')');
   r << QChar('!') << QChar('?') << QChar('"');
   return r;
};

bool HumanLanguage::isLetter(QChar c){
   return letters.contains(c);
};

bool HumanLanguage::isPMark(QChar c){
   return pMarks.contains(c);
};

QChar HumanLanguage::nextPMark(const QString &s, int i){
   QChar r = QChar();
   for(int j=i; j<s.size(); j++){
      if ( isLetter(s.at(j)) ) return r;
      if ( isPMark(s.at(j)) ) return s.at(j);
   }
   return r;
};

HumanLanguage::HumanLanguage(HumanLanguageID id){
   switch (id){
   case bulgarian:
      letters = bgLetters();
      pMarks = bgPMarks();
   }
};
