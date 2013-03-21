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

#include "grammar.h"
#include "showMessage.h"
#include "myFileRoutines.h"

#include <QApplication>
#include <QDataStream>

//--------WordForm--------------

WordForm::~WordForm(){};

QString WordForm::word(const QString &wr){
   return wr;
};

QString WordForm::word(const QString &wr, QChar c){
   return wr+c;
};

int WordForm::qmIndex(){
   return -1;
};

QString WordForm::info(){
   return QString::fromUtf8("Базов клас за словоформа");
};

//--------WordForms--------------

WordForms::~WordForms(){};

//--------WordRoot--------------

WordRoot::WordRoot(const QString &r, WordForms *wfs){
   root=r; wforms=wfs;
};

WordRoot::~WordRoot(){};

QString WordRoot::info(bool withList){
   int n = 0; if (size()>1) n = 1;
   QString r = root + "<br>" + toString(); // { int i=r.indexOf("<br>"); r.insert(i," - "+root); }
   if (!withList) return r;
   if (r.size()) r.append("<br><br>");
   for(int i=n;i<size();i++){
      r.append(word(i)).append("<br>");
   }
   return r;
};

QString WordRoot::word(){
   return root;
}

char* WordRoot::cword(){
   return root.toAscii().data();
};

QString WordRoot::word(int i){
   if (i==0) return root;
   int qmi = form(0)->qmIndex();
   if (qmi<0)
      return form(i)->word(root);
   else {
      int j = root.size()-qmi;
      QChar c = root.at(j);
      return form(i)->word(root,c);
   }
};

char *WordRoot::cword(int i){
   return word(i).toAscii().data();
};

QStringList *WordRoot::wordForms(){
   if (!size()) return 0;
   else {
      QStringList *wfs = new QStringList;
      int n = 0; if (size()>1) n = 1;
      for (int i=n;i<size();i++) wfs->append(word(i));
      return wfs;
   }
};

WordForm* WordRoot::form(const QString &wf){
   int n = 0; if (size()>1) n=1;
   for(int i=n;i<wforms->size();i++)
      if (wf==word(i)) return form(i);
   return 0;
};

WordForm* WordRoot::form(int i){
   return wforms->value(i);
};


WordForms* WordRoot::forms(){
   return wforms;
};

void WordRoot::setForms(WordForms *wfs){
   wforms=wfs;
};

int WordRoot::size(){
   return wforms->size();
};

//--------WordSuffix--------------

WordSuffix::WordSuffix(int an, const QString &asf)
{
   n=an; sf=asf;
};

QString WordSuffix::word(const QString &wr){
/*   QString s = sf;
   int k = wr.size() - n;
   int i = s.indexOf("?");
   if (i>-1){
      showMessage(wr+"-"+s);
      int j=k+i;
      s[i]=wr[j];
   }
   QString r = wr.left(k);
   return r.append(s);*/
   return wr.left(wr.size()-n)+sf;
};

QString WordSuffix::word(const QString &wr, QChar c){
   QString s = sf;
   int k = wr.size() - n;
   int j = s.indexOf("?");
   if (j>-1) s[j] = c;
   return wr.left(k)+s;
};

int WordSuffix::qmIndex(){
   int r = sf.indexOf("?");
   if (r<0) return r;
   else return n-r;
};

QString WordSuffix::info(){
   QString r = toString();
   if (sf.size()){
      r.append("<br><br>").append(QString::fromUtf8("Образува се чрез "));
      if (n) r.append( QString::fromUtf8("заместване на последните %1 букви с ")
         .arg(QString::number(n)) );
      else r.append(QString::fromUtf8("прибавяне на "));
      r.append( QString::fromUtf8("окончанието \"%1\"").arg(sf) );
   }
   return r;
};

//--------WordPreffix--------------

WordPreffix::WordPreffix(WordRoot *ar, const QString &ap, int n)
   :WordRoot("")
{
   QString r = ar->word();
   int k = r.size() - n;
   root = r.right(k).prepend(ap);
   wforms = ar->forms();
};

//--------WordRootFormPair--------------

WordRootFormPair::WordRootFormPair(WordRoot *wr, WordForm *wf, int ri, int fi){
   wRoot=wr; wForm=wf; wrIndex=ri; wfIndex=fi;
};

WordRoot *WordRootFormPair::root(){
   return wRoot;
};

WordForm *WordRootFormPair::form(){
   return wForm;
};

int WordRootFormPair::rootIndex(){
   return wrIndex;
};

int WordRootFormPair::formIndex(){
   return wfIndex;
};

//--------WordRootFormPairList--------------

QString WordRootFormPairList::word(int i){
   WordRootFormPair *rfp = at(i);
   if (!rfp)  return "";
   WordRoot *r = rfp->root();
   int j = rfp->formIndex();
   QString rz = r->word(j);
   return rz;
};

int WordRootFormPairList::indexOf(const QString &w){
   int i2 = size()-1;
   if (i2<0) return 0;
   int i1 = 0;
   QString w0 = w.toLower();
   while (i2-i1>1){
      int i = (i1+i2)/2;
      QString w1 = word(i).toLower();
      if (w0>w1) i1=i; else i2=i;
   }
   QString w1 = word(i1).toLower();
   if (w0<=w1) return i1;
   QString w2 = word(i2).toLower();
   if (w0>w2) return i2+1;
   else return i2;
};

/*void WordRootFormPairList::add(WordRootFormPair *rfp){
   WordRoot *r = rfp->root();
   showMessage(r->word());
   WordForm *f = rfp->form();
   QString w = f->word(r->word());
   int i = indexOf(w);
   insert(i,rfp);
};*/

void WordRootFormPairList::sort(){
   if (size()) sort(0,size()-1);
}

void WordRootFormPairList::sort(int i1,int i2){
   int k1 = i1; int k2 = i2;
   QString s = word( (i1+i2)/2 ).toLower();
   do {
      while (word(k1).toLower()<s) k1++;
      while (word(k2).toLower()>s) k2--;
      if (k1<k2){
         WordRootFormPair *rfp=at(k1);
         replace(k1,at(k2));
         replace(k2,rfp);
         k1++; k2--;
      }
      else if (k1==k2) { k1++; k2--; }
   } while (k1 <= k2);
   if (k2>i1) sort(i1,k2);
   if (k1<i2) sort(k1,i2);
}

void WordRootFormPairList::loadFromFile(const QString &fn){};

void WordRootFormPairList::saveToFile(const QString &fn, QDateTime lm){
   if (lm<myFileInfo(fn).lastModified()) return;
   QFile file(fn);
   if (file.open(QIODevice::WriteOnly)){
      QDataStream out(&file);
      for(int i=0;i<size();i++){
         out << at(i)->rootIndex() << at(i)->formIndex();
      }
      file.close();
   }
};

//--------SpellingMistake-------------

QString SpellingMistake::note(){
   switch (mistakeType){
   case NoMistake: return QApplication::tr("No more spelling mistakes found.");
   case NoTextToCheck: return QApplication::tr("There is no text to check.");
   case NotAWord: return QApplication::tr("Word \"%1\" is not in the dictionary.").arg(text);
   case WrongCase: return QApplication::tr("Wrong letter case in word \"%1\" .").arg(text);
   case TowWords: return QApplication::tr("May be \"%1\" are tow words.").arg(text);
   }
   return "";
};

//--------LangDictionary--------------

LangDictionary::LangDictionary():QList<WordRoot*>(){
   rList = 0; fList = 0; rfpList0 = 0; rfpList = 0; wfSet = 0;
   lModified = QDateTime();
   rootsFileName = myStatusDir("roots.dat");
   formsFileName = myStatusDir("forms.dat");
   language = new HumanLanguage(bulgarian);
};

QString LangDictionary::info(){
   return QString("Bulgarian dictionary\n%1 word roots\n%2 word forms\n%3 form patterns")
      .arg(size()).arg(formPairs()->size()).arg(wordFormSet()->size());
}

char *LangDictionary::cinfo(){
   return info().toAscii().data();
};

int LangDictionary::indexOfRoot(const QString &w){
   WordRootFormPairList *rfpl = rootPairs();
   if (rfpl==0) return -1;
   int i = rfpl->indexOf(w);
   if (i==rfpl->size()) return -1;
   return i;
};

int LangDictionary::indexOfForm(const QString &w){
   WordRootFormPairList *rfpl = formPairs();
   if (rfpl==0) return -1;
   int i = rfpl->indexOf(w);
   if (i==rfpl->size()) return -1;
   return i;
};

WordRoot *LangDictionary::rootOfForm(int i){
   WordRootFormPairList *rfpl = formPairs();
   return rfpl->at(i)->root();
};

WordForm *LangDictionary::form(int i){
   return formPairs()->at(i)->form();
};

WordForm *LangDictionary::form(const QString &w){
   int i = indexOfForm(w);
   if (i<0) return 0;
   return formPairs()->at(i)->form();
};

WordRootFormPairList *LangDictionary::loadRFPList(const QString &fn){
   WordRootFormPairList *rfpl = new WordRootFormPairList;
//   showMessage(lastModified().toString()+"\n"+QFileInfo(fn).lastModified().toString());
   if ( (QDateTime()<lastModified())
      && (lastModified()<QFileInfo(fn).lastModified()) )
   {
      showStatus(qApp->translate("LangDictionary","Reading file...") );
      QFile file(fn);
      if (file.open(QIODevice::ReadOnly)){
         QDataStream in(&file);
         while (!in.atEnd()){
            int i,j;
            in >> i; in >> j;
            WordRoot *r = at(i);
            WordRootFormPair *rfp = new WordRootFormPair(r,r->form(j),i,j);
            rfpl->append(rfp);
         }
      }
   }
   return rfpl;
};

QStringList *LangDictionary::WordRootsList(){
   if (rList==0){
      rfpList0 = rootPairs();
      showStatus(qApp->translate("LangDictionary","Transferring to string list..."));
      rList = new QStringList;
      for(int i=0;i<size();i++) rList->append(rfpList0->word(i));
   }
   return rList;
};

QStringList *LangDictionary::WordFormsList(){
   if (fList==0){
      rfpList = formPairs();
      showStatus(qApp->translate("LangDictionary","Transferring to string list..."));
      fList = new QStringList;
      for(int i=0;i<rfpList->size();i++)
         fList->append( rfpList->word(i) );
   };
   return fList;
};

WordRoot *LangDictionary::root(int i){
   WordRootFormPairList *rfpl = rootPairs();
   return rfpl->at(i)->root();
}

WordRoot *LangDictionary::root(const QString &r){
   for(int i=0;i<size();i++)
      if (r==at(i)->word()) return at(i);
   return 0;
};

void LangDictionary::setLastModified(const QDateTime &dt){
   if (dt>lModified) lModified = dt;
};

QDateTime LangDictionary::lastModified(){
   return lModified;
};

QSet<WordForms *> *LangDictionary::wordFormSet(){
   if (!wfSet){
      wfSet = new QSet<WordForms *>;
      for(int i=0; i<size(); i++) wfSet->insert(at(i)->forms());
   }
   return wfSet;
};

WordRootFormPairList *LangDictionary::rootPairs(){
//   showMessage(size());
   if (rfpList0==0){
      rfpList0 = loadRFPList(rootsFileName);
      if (rfpList0->size()==0){
         showStatus(QApplication::tr("Creating root list..."));
         for(int i=0;i<size();i++){
            WordRoot *r = at(i);
            WordRootFormPair *rfp = new WordRootFormPair(r,r->form(0),i,0);
            rfpList0->append(rfp);
         }
         showStatus(QApplication::tr("Sorting..."));
         rfpList0->sort();
      }
   }
   return rfpList0;
};

WordRootFormPairList *LangDictionary::formPairs(){
   if (rfpList==0){
      rfpList = loadRFPList(formsFileName);
      if (rfpList->size()==0){
         showStatus(QApplication::tr("Creating form list..."));
         for(int i=0;i<size();i++){
            WordRoot *r = at(i);
            int n=0; if (r->size()>1) n=1;
            for(int j=n; j<r->size(); j++){
               WordRootFormPair *rfp = new WordRootFormPair(r,r->form(j),i,j);
               rfpList->append(rfp);
            }
         }
         showStatus(QApplication::tr("Sorting..."));
         rfpList->sort();
      }
   }
   return rfpList;
};

void LangDictionary::saveFiles(){
   if (rfpList0!=0) rfpList0->saveToFile(rootsFileName,lModified);
   if (rfpList!=0 ) rfpList ->saveToFile(formsFileName,lModified);
};

void LangDictionary::setRootIndexFileName(const QString &fn){
   rootsFileName = fn;
}

void LangDictionary::setFormIndexFileName(const QString &fn){
   formsFileName = fn;
}
   
SpellingMistake::MistakeType LangDictionary::isWordCorrect(const QString &w){
   int i = indexOfForm(w);
   SpellingMistake::MistakeType r = SpellingMistake::NotAWord;
   if ( (i<0) || (i>formPairs()->size()-1) ) return SpellingMistake::NotAWord;
   do{
      QString w1 = formPairs()->word(i);
      r = isSameWord(w,w1);
      if (r==SpellingMistake::NoMistake) return r;
      i++;
      if (i>formPairs()->size()-1) return SpellingMistake::NotAWord;
   } while ( r!=SpellingMistake::NotAWord );
   return SpellingMistake::NotAWord;
};

SpellingMistake::MistakeType LangDictionary::isSameWord(const QString &w, const QString &w1){
   if (w==w1) return SpellingMistake::NoMistake;
   if (w.toLower()!=w1.toLower()) return SpellingMistake::NotAWord;
   bool firstUpper = false;
   bool secondUpper = false;
   bool allUpper = true;
   for(int j=0; j<w.size(); j++){
      allUpper = allUpper && w.at(j).isUpper();
      if (j==0){
         firstUpper = w1.at(0).isUpper();
         if (firstUpper && w.at(0).isLower() ) return SpellingMistake::WrongCase;
      }
      if (j==1){
         secondUpper = w.at(1).isUpper();
         if (!firstUpper && secondUpper && !allUpper) return SpellingMistake::WrongCase;
      }
      if ((j>1) && secondUpper && !allUpper ) return SpellingMistake::WrongCase;
   }
   return SpellingMistake::NoMistake;
};

SpellingMistake LangDictionary::findMistake(const QString &s, int i0){
   SpellingMistake r;
   r.start=0; r.end=0;
   r.text = "";
   if (!s.size()){ r.mistakeType=SpellingMistake::NoTextToCheck; return r; }
   r.mistakeType=SpellingMistake::NoMistake;
   bool inWord = false;
   QString w;
   for(int i=i0; i<s.size(); i++){
      if (language->isLetter(s.at(i)) )
         if (inWord){}
         else {
            inWord=true;
            r.start=i;
         }
      else
         if (inWord){
            inWord=false;
            r.end=i;
            w = s.mid(r.start,r.end-r.start);
            SpellingMistake::MistakeType mt = isWordCorrect(w);
            if ( mt != SpellingMistake::NoMistake ){
               r.text = w;
               r.mistakeType = mt;
               if (mt == SpellingMistake::NotAWord){
                  QChar pm = language->nextPMark(s,i);
                  if (isAbreviation(w,pm)){
                     r.mistakeType=SpellingMistake::NoMistake;
                     continue;
                  }
                  if (towWords(w)){
                     r.mistakeType=SpellingMistake::TowWords;
                     return r;
                  }
               }
               return r;
            }
         }
         else {}
   };
   if (inWord){
      w = s.mid(r.start,s.size()-r.start);
      r.text = w;
      r.mistakeType = isWordCorrect(w);
   }
   return r;
};

bool LangDictionary::isAbreviation(const QString &w, QChar pm){
   int i = indexOfRoot(w);
   if ( (pm!=QChar('.')) || (i>=rootPairs()->size()) ) return false;
   QString r = root(i)->word();
   return ( r.startsWith(w,Qt::CaseInsensitive) );
};

bool LangDictionary::towWords(const QString &w){
   for(int i=0; i<w.size(); i++){
      QString w1 = w.left(i+1);
      QString w2 = w.right(w.size()-i-1);
//      showMessage(w1+"\n"+w2);
      bool r = ( (isWordCorrect(w1)==SpellingMistake::NoMistake) && 
                      (isWordCorrect(w2)==SpellingMistake::NoMistake) );
      if (r) return true;
   }
   return false;                
};