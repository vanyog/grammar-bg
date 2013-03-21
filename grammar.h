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

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "thing.h"
#include "humanLanguage.h"

#include <QSet>
#include <QList>
#include <QString>
#include <QHash>
#include <QDateTime>
#include <QObject>

// базов клас, прадстевящ дума
class WordForm:public Thing{
public:
   ~WordForm();
   virtual QString word(const QString &wr); // генерира словоформа от корен wr
   virtual QString word(const QString &wr, QChar c); // генерира словоформа от корен wr
   virtual int qmIndex();
   virtual QString info(); // връща информация за класа
};

// словоформа с окончание
class WordSuffix:public WordForm{
public:
   WordSuffix(int an, const QString &asf);
   QString word(const QString &wr); // генерира словоформата, като премахва n букви от края на корена wr и добавя окончание sf
   QString word(const QString &wr, QChar c);
   virtual int qmIndex();
   virtual QString info(); // връща информация за класа
private:
   int n;      // брой символи, които се премахват преди добавяне на окончанието
   QString sf; // окончание
};

// списък от словоформи
class WordForms : public QList<WordForm *>{
public:
   ~WordForms();
};

// основна форма на дума със списък от словоформи
class WordRoot:public WordForm{
public:
   WordRoot(const QString &r, WordForms *wfs=0); // създава основна форма с корен r
   ~WordRoot();                                  // и списък от словоформи wfs
   virtual QString info(bool withList=false); // връща информация за класа
   QString word(); // генерира основната форма
   char* cword();  // връща основната форма като char*
   QString word(int i); // генерира i-тата форма
   char* cword(int i);  // връща i-тата форма като char*
   QStringList* wordForms(); // списък на словоформите
   WordForm* form(int i); // връща указател към i-тата словоформа
   WordForm* form(const QString &wf); // връща указател към словоформата wf или 0
   WordForms* forms();    // връща указател към списъка от словоформи
   void setForms(WordForms *wfs); // задава списък от словоформи
   int size();            // връща броя на словоформите
protected:
   QString root;      // основна форма(корен) на думата
   WordForms *wforms; // списък от словоформи
};

// основна форма с представка
class WordPreffix:public WordRoot{
public:
   WordPreffix(WordRoot *ar, const QString &ap, int n = 0);
};

// клас, съдържащ двойка указатели - към словоформа и към нейния корен
class WordRootFormPair{
public:
   WordRootFormPair(WordRoot *wr, WordForm *wf, int ri, int fi);
   WordRoot *root();
   WordForm *form();
   int rootIndex();
   int formIndex();
private:
   WordRoot *wRoot;
   WordForm *wForm;
   int wrIndex, wfIndex;
};

// списък от WordRootFormPair*
class WordRootFormPairList:public QList<WordRootFormPair*>{
public:
   QString word(int i);
   int indexOf(const QString &w);
//   void add(WordRootFormPair *rfp);
   void sort();
   void sort(int i1,int i2);
   void loadFromFile(const QString &fn);
   void saveToFile(const QString &fn,QDateTime lm=QDateTime());
};

// информация за допусната правописна грешка
class SpellingMistake{
public:
   enum MistakeType { NoMistake, NoTextToCheck, NotAWord, WrongCase, TowWords }; // тип на грешката
   MistakeType mistakeType;
   int start,end; // позиции на началото и края на думата в текста
   QString text; // сгрешената дума
   QString note(); // обяснение относно грешката
};

// речник на езика
class LangDictionary:public QList<WordRoot*>{
public:
   LangDictionary();
   QString info(); // дава информация за речника
   char *cinfo();  // информация за речника като char*
   int indexOfRoot(const QString &w); // номер на корен w или номер на мястото, на което трябва да се вмъкне
   int indexOfForm(const QString &w); // номер на словоформа w или -1 ако няма такава
   WordForm *form(int i);            // словоформа с номер i
   WordForm *form(const QString &w); // словоформа w
   QStringList *WordRootsList(); // създава и връща списък на основните форми
   QStringList *WordFormsList(); // създава и връща списък на словоформите
   WordRoot *root(int i);        // обект, който описва корен с номер i
   WordRoot *root(const QString &r); // указател към корен r
   WordRoot *rootOfForm(int i);  // корен на словоформа с номер i
   void setLastModified(const QDateTime &dt); // задава време на последната промяна
   QDateTime lastModified(); // време на последната промяна
   QSet<WordForms *> *wordFormSet(); // Създава и връща списък на шаблоните за образуване на словоформи
   WordRootFormPairList *rootPairs(); // създава или чете индекс на корените
   WordRootFormPairList *formPairs(); // създава или чете индекс на словоформите
   WordRootFormPairList *loadRFPList(const QString &fn); // чете индекси от файл fn
   void saveFiles(); // записва индексите
   void setRootIndexFileName(const QString &fn); // Задава име на файла с индексите на корените
   void setFormIndexFileName(const QString &fn); // Задава име на файла с индексите на всички форми
   SpellingMistake::MistakeType isWordCorrect(const QString &w); // проверява дали w е правилно изписана дума   
   SpellingMistake::MistakeType isSameWord(const QString &w, const QString &w0);  // проверява дали w и w1 са една и съща дума
   SpellingMistake findMistake(const QString &s, int i0 = 0);  // търси първата правописна грешка в текста s, започвайки от позиция i0
   bool isAbreviation(const QString &w, QChar pm); // връща true ако w е съкращение на дума, pm е знака след w, трябва да е "."
   bool towWords(const QString &w); // проверява дали w са две слепени думи
   HumanLanguage *language;  // обект представящ езика
private:
   WordRootFormPairList *rfpList, *rfpList0;
   QStringList *rList; // списък на основните форми
   QStringList *fList; // списък на словоформите
   QSet<WordForms *> *wfSet; // списък на шаблоните за образуване на словоформи
   QDateTime lModified; // време на последната промяна
   QString rootsFileName; // име на файла с индекси за корените
   QString formsFileName; // име на файла с индекси за словоформите
};


/* Функция за зереждане на речник langDic от bgoffice
   Виж: http://bgoffice.sourceforge.net
   dicDir е data директорията на bgoffice
   Рекурсивно влиза във всяка поддиректория и вика loadDir,
   която чете информацията от файловете в дадена поддиректория
*/
namespace BGOffice{
   void loadTo(const QString &dicDir, LangDictionary *langDic);
};

namespace IDIDictionary{
   void loadTo(const QString &dicDir, LangDictionary *langDic);
};

namespace BGDicdata{
   void loadTo(const QString &dicDir, LangDictionary *langDic);
};

#endif

