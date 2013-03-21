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

#include <QDir>
#include <QTextStream>
#include <QTextCodec>
#include <QApplication>
#include <QDateTime>

namespace BGOffice{

int fileCount=0; // Брой на обработените файлове с думи

Thing *f0=0; // Общи характеристики на думите, които се зареждат от файл description.dat
QList<Thing*> *fList; // Списък от характеристики на думите

// интерпрепиране на секция "Група:" на description.dat файл
void setGroup(const QString &line){
 QStringList parts=line.split(",");
 QStringListIterator i(parts);
 while (i.hasNext()){
  QString s=i.next().trimmed();
  if (s=="прилагателно име") 
   f0 = new ThingNamedValue("част на речта","прилагателно име"); else {
  if (s=="съществително име") 
   f0 = new ThingNamedValue("част на речта","съществително име"); else {
  if (s=="мъжки род") 
   f0->append( new ThingNamedValue("род","мъжки") ); else {
  if (s=="женски род") 
   f0->append( new ThingNamedValue("род","женски") ); else {
  if (s=="среден род") 
   f0->append( new ThingNamedValue("род","среден") ); else {

  if (s=="месец"){
   f0 = new ThingNamedValue("част на речта","съществително име");
   f0->append( new ThingNamedValue("род","мъжки") ); 
   f0->append( new ThingNamedValue("означава","месец") );
  } else {
  if (s=="българско населено място"){
   f0 = new ThingNamedValue("част на речта","съществително име");
   f0->append( new ThingNamedValue("вид","собствено") );
   f0->append( new ThingNamedValue("означава","българско населено място") );
  } else {
  if (s=="столица"){
   f0 = new ThingNamedValue("част на речта","съществително име");
   f0->append( new ThingNamedValue("вид","собствено") );
   f0->append( new ThingNamedValue("означава","столица") );
  } else {
  if (s=="световноизвестен град"){
   f0 = new ThingNamedValue("част на речта","съществително име");
   f0->append( new ThingNamedValue("вид","собствено") );
   f0->append( new ThingNamedValue("означава","световноизвестен град") );
  } else {
  if (s=="държава"){
   f0 = new ThingNamedValue("част на речта","съществително име");
   f0->append( new ThingNamedValue("вид","собствено") );
   f0->append( new ThingNamedValue("означава","държава") );
  } else {

  if (s=="географско понятие"){
   f0 = new ThingNamedValue("част на речта","съществително име");
   f0->append( new ThingNamedValue("вид","собствено") );
   f0->append( new ThingNamedValue("означава","геогравско понятие") );
  } else {
  if (s=="фамилно име или презиме"){
   f0 = new ThingNamedValue("част на речта","съществително");
/*   f0->append( new ThingNamedValue("вид","относително") );
   f0->append( new ThingNamedValue("за","лична принадлежност") );*/
   f0->append( new ThingNamedValue("означава","фамилно име или презиме") );
  } else {
  if (s=="лично име"){
   f0 = new ThingNamedValue("част на речта","съществително име");
   f0->append( new ThingNamedValue("вид","собствено") );
   f0->append( new ThingNamedValue("означава","българско лично име") );
  } else {
  if (s=="лично име или фамилия"){
   f0 = new ThingNamedValue("означава","лично име или фамилия");
  } else {
  if (s=="българско географско понятие"){
   f0 = new ThingNamedValue("част на речта","съществително име");
   f0->append( new ThingNamedValue("вид","собствено") );
   f0->append( new ThingNamedValue("означава","българско географско име") );
  } else {

  if (s=="търговска марка"){
   f0 = new ThingNamedValue("част на речта","съществително име");
   f0->append( new ThingNamedValue("вид","собствено") );
  } else {
  if (s=="световноизвестна личност или неизменяем термин"){} else {
  if (s=="световноизвестно географско понятие"){
   f0 = new ThingNamedValue("част на речта","съществително име");
   f0->append( new ThingNamedValue("вид","собствено") );
   f0->append( new ThingNamedValue("означава","световноизвестно географско понятие") );
  } else {
  if (s=="числително име"){
   f0 = new ThingNamedValue("част на речта","числително име"); 
   } else {
  if (s=="числително бройно име"){
   f0 = new ThingNamedValue("част на речта","числително име"); 
   f0->append( new ThingNamedValue("вид","бройно") );
   } else {

  if (s=="редно числително име"){
   f0 = new ThingNamedValue("част на речта","числително име"); 
   f0->append( new ThingNamedValue("вид","редно") );
   } else {
  if (s=="числително редно име"){
   f0 = new ThingNamedValue("част на речта","числително име"); 
   f0->append( new ThingNamedValue("вид","редно") );
   } else {
  if (s=="показателно местоимение"){
   f0 = new ThingNamedValue("част на речта","местоимение"); 
   f0->append( new ThingNamedValue("вид","показателно") );
   } else {
  if (s=="обобщително местоимение"){
   f0 = new ThingNamedValue("част на речта","местоимение"); 
   f0->append( new ThingNamedValue("вид","обобщително") );
   } else {
  if (s=="неопределително местоимение"){
   f0 = new ThingNamedValue("част на речта","местоимение"); 
   f0->append( new ThingNamedValue("вид","неопределително") );
   } else {

  if (s=="въпросително местоимение"){
   f0 = new ThingNamedValue("част на речта","местоимение"); 
   f0->append( new ThingNamedValue("вид","въпросително") );
   } else {
  if (s=="отрицателно местоимение"){
   f0 = new ThingNamedValue("част на речта","местоимение"); 
   f0->append( new ThingNamedValue("вид","отрицателно") );
   } else {
  if (s=="лично местоимение"){
   f0 = new ThingNamedValue("част на речта","местоимение"); 
   f0->append( new ThingNamedValue("вид","лично") );
   } else {
  if (s=="притежателно местоимение"){
   f0 = new ThingNamedValue("част на речта","местоимение"); 
   f0->append( new ThingNamedValue("вид","притежателно") );
   } else {
  if (s=="относително местоимение"){
   f0 = new ThingNamedValue("част на речта","местоимение"); 
   f0->append( new ThingNamedValue("вид","относително") );
   } else {

  if (s=="наречие") 
   f0 = new ThingNamedValue("част на речта","наречие"); else {
  if (s=="съюз") 
   f0 = new ThingNamedValue("част на речта","съюз"); else {
  if (s=="междуметие") 
   f0 = new ThingNamedValue("част на речта","междуметие"); else {
  if (s=="частица") 
   f0 = new ThingNamedValue("част на речта","частица"); else {
  if (s=="предлог") 
   f0 = new ThingNamedValue("част на речта","предлог"); else {
  if (s=="съкращение") 
   f0 = new ThingNamedValue("част на речта","съкращение"); else {

  if (s=="глагол") 
   f0 = new ThingNamedValue("част на речта","глагол"); else {
  showMessage(qApp->translate("BGOffice::QApplication","Unknown word group<br>%1<br>%2").arg(line).arg(s));
  }}}}} }}}}} }}}}} }}}}} }}}}} }}}}} }}}}}} }
 }
 fList = new QList<Thing*>; // Саздаване на нов списък от характеристики за думите от поредната директория 
// countedMessage( QString(line).append("<BR>").append(f0->toString()) );
};

// Функция, която създава ново свойство, ако fr е нула, или добавя ново свойство
// към fr. Новото свойство е с име n и стойност v.
Thing *newOrAppend(Thing *fr, const QString &v, const QString &n=""){
 Thing *nfr;
 if (n.size()) nfr = new ThingNamedValue(n,v);
 else nfr = new ThingValue(v);
 if (fr==0) return nfr;
 else { fr->append(nfr); return fr; }
};

QString cDir = ""; // Път на текущата директория

// интерпрепиране на 1 ред от секция "Форми:" на description.dat файл
void setForm(const QString &line){
 Thing *fr = 0;
 QStringList parts=line.split(",");
 QStringListIterator i(parts);
 while (i.hasNext()){
  QString s = i.next().trimmed();
  if (s=="основна форма")
    fr = newOrAppend(fr,"основна форма"); else {
  if (s=="м.р.")
    fr = newOrAppend(fr,"мъжки","род"); else {
  if (s=="ж.р.")
    fr = newOrAppend(fr,"женски","род"); else {
  if (s=="ср.р.")
    fr = newOrAppend(fr,"среден","род"); else {
  if (s=="ед.ч.")
    fr = newOrAppend(fr,"единствено","число"); else {

  if (s=="мн.ч.")
    fr = newOrAppend(fr,"множествено","число"); else {
  if (s=="непълен член")
    fr = newOrAppend(fr,"непълен","член"); else {
  if (s=="пълен член")
    fr = newOrAppend(fr,"пълен","член"); else {
  if (s=="членувано")
    fr = newOrAppend(fr,"има","член"); else {
  if (s=="разширена форма = (мн.ч.)")
    fr = newOrAppend(fr,"множествено","число"); else {

  if (s=="кратка форма")
    fr = newOrAppend(fr,"кратка","форма"); else {
  if (s=="звателна форма")
    fr = newOrAppend(fr,"звателна","форма"); else {
  if (s=="мъжка звателна форма"){
    fr = newOrAppend(fr,"звателна","форма");
    fr->append(new ThingNamedValue("род","мъжки"));
  } else {
  if (s=="женска звателна форма"){
    fr = newOrAppend(fr,"звателна","форма");
    fr->append(new ThingNamedValue("род","женски"));
  } else {
  if (s=="бройна форма")
    fr = newOrAppend(fr,"бройна","форма"); else {
  if (s=="мъжколична форма")
    fr = newOrAppend(fr,"мъжколична","форма"); else {
  if (s=="приблизителен брой")
    fr = newOrAppend(fr,"приблизителен брой","означава"); else {

  if (s=="именителен падеж")
    fr = newOrAppend(fr,"именителен","падеж"); else {
  if (s=="винителен падеж")
    fr = newOrAppend(fr,"винителен","падеж"); else {
  if (s=="дателен падеж")
    fr = newOrAppend(fr,"дателен","падеж"); else {
  if (s=="предложна форма (на ?)")
    fr = newOrAppend(fr,"предложна (на ?)","форма"); else {
  if (s=="мъжко")
    fr = newOrAppend(fr,"мъжко име","означава"); else {

  if (s=="женско")
    fr = newOrAppend(fr,"женско име","означава"); else {
  if (s=="сег.вр.")
    fr = newOrAppend(fr,"сегашно","време"); else {
  if (s=="мин.св.вр.")
    fr = newOrAppend(fr,"минало свършено","време"); else {
  if (s=="мин.несв.вр.")
    fr = newOrAppend(fr,"минало несвършено","време"); else {
  if ((s=="бъд.вр.")||(s=="мин.неопр.вр.")||(s=="мин.пред.вр.")||
     (s=="бъд.пред.вр.")||(s=="бъд.вр. в мин.")||(s=="бъд.вр. в мин.")||
     (s=="бъд.пред.вр. в мин.")||(s=="условно наклонение")||
     (s=="пр.накл.")||(s=="възвр.страд.")||(s=="пр.страд. форма")||
     (s=="възвр.страд. форма")
    )
   break; else {

  if (s=="1л.")
    fr = newOrAppend(fr,"1","лице"); else {
  if (s=="2л.")
    fr = newOrAppend(fr,"2","лице"); else {
  if (s=="3л.")
    fr = newOrAppend(fr,"3","лице"); else {
  if (s=="повелително наклонение")
    fr = newOrAppend(fr,"повелително","наклонение"); else {
  if (s=="мин.деят.св.прич."){
    fr = newOrAppend(fr,"причастие","форма");
    fr->append(new ThingNamedValue("време","минало свършено"));
    fr->append(new ThingNamedValue("вид","диятелно"));
  } else {

  if (s=="мин.деят.несв.прич."){
    fr = newOrAppend(fr,"причастие","форма");
    fr->append(new ThingNamedValue("време","минало несвършено"));
    fr->append(new ThingNamedValue("вид","диятелно"));
  } else {
  if (s=="мин.страд.прич."){
    fr = newOrAppend(fr,"причастие","форма");
    fr->append(new ThingNamedValue("време","минало"));
    fr->append(new ThingNamedValue("вид","страдателно"));
  } else {
  if (s=="сег.деят.прич."){
    fr = newOrAppend(fr,"причастие","форма");
    fr->append(new ThingNamedValue("време","сегашно"));
    fr->append(new ThingNamedValue("вид","деятелно"));
  } else {
  if (s=="деепричастие")
    fr = newOrAppend(fr,"деепричастие","форма"); else {
   showMessage(qApp->translate("BGOffice::QApplication","Unknown word form<br>%1<br>%2<br>%3")
    .arg(s).arg(line).arg(cDir));
  }}}}} }}}}} }}}}}}} }}}}} }}}}} }}}}} }}}}
 }
 if (fr!=0){
  fr->append(f0);    // Добавяне на общите свойства
  fList->append(fr); // Добавяне на характеристиката fr към списъка
  //countedMessage( QString(line).append("<br>").append(fr->toString()) );
 }
};

LangDictionary *cLangDic = 0; // Указател към речника, към който се добавят думи

// интерпрепиране на description.dat файл
void loadDescription(const QString &subDir){
 QString filePath=subDir; filePath.append("/description.dat");
 QFile file(filePath);
 if (file.open(QIODevice::ReadOnly)){
  cDir = subDir;
  cLangDic->setLastModified(myFileInfo(filePath).lastModified());
  QTextStream in(&file);
//  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("cp1251"));
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  in.setCodec(QTextCodec::codecForName("cp1251"));
  QString group = "Група:";
  QString form = "Форми:";
  bool toSetForm = false;
  while (!in.atEnd()){
   QString line = in.readLine().trimmed();
   if ( (line.size()>0) && (line.at(0)!=QChar('#')) ){
    if (line==group) setGroup(in.readLine());
    if (toSetForm) setForm(line.trimmed());
    toSetForm = toSetForm || (line==form);
   }
  }
  file.close();
 }
 else showMessage( qApp->translate("BGOffice::QApplication","Error opening file<br>%1").arg(filePath) );
};

int sfxIndex=0;        // пореден номер на текущо обработваното окончание
WordForms *wForms = 0; // списък от словоформи с общ корен
QString cFile = "";    // име на текущо обработвания bg*.dat файл
QString cFilePath = "";// абсолютно име на текущо обработвания bg*.dat файл

// интерпретиране на 1 окончаниe от секция "Окончания:" от bg*.dat файл
void setSuffix(const QString &line){
 static int n = 0;
 WordSuffix *wsf;
 Thing *th = fList->at(sfxIndex);
 if (th->is("основна форма")){
  QStringList parts = line.split(",");
  QString sf = parts.at(0);
  n = sf.size();
  int n1 = sf.indexOf("[");
  if (n1>-1){
   int n2 = sf.indexOf("]");
   n -= n2-n1;
   sf.replace(n1,n2-n1+1,"?");
  }
  if (sf=="0"){ n=0; sf = ""; }
  wsf = new WordSuffix(n,sf);
  wsf->append(th);
  wsf->append(new ThingNamedValue("таблица",QFileInfo(cFilePath).absoluteFilePath()) );
  wForms->append(wsf);
 } else if (line.at(0)!=QChar('-')) {
  QStringListIterator i(line.split(","));
  while (i.hasNext()){
   QString sf = i.next().trimmed();
   if (sf=="0") sf = "";
   wsf = new WordSuffix(n,sf);
   wsf->append(th);
   wForms->append(wsf);
  }
 }
 sfxIndex++;
};

void setRoot(const QString &line){
 WordRoot *wr = new WordRoot(line,wForms);
 wr->append(wForms->at(0));
 cLangDic->append(wr);
};

// четене на думи от bg*.dat файл
void loadWords(const QString &filePath){
 fileCount++;
 QFile file(filePath);
 if (file.open(QIODevice::ReadOnly)){
  cFilePath = filePath;
  cLangDic->setLastModified(myFileInfo(filePath).lastModified());
  QTextStream in(&file);
  in.setCodec(QTextCodec::codecForName("cp1251"));
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  QString sfx("Окончания:");
  QString test("Тест:");
  QString words("Думи:");
  int what = 0;
  sfxIndex = 0;
  wForms = new WordForms;
  while (!in.atEnd()){
   QString line = in.readLine().trimmed();
   line = line.remove(QChar(32));
   int i = line.indexOf("#");
   if (i>0){ line = line.left(i); }
   if ( (line.size()>0) && (line.at(0)!=QChar('#')) ){
    if (line==sfx) what=1;
    if (line==test) what=2;
    if (line==words) what=3;
    switch (what){
     case 1:if (line!=sfx) setSuffix(line); break;
     case 3:if (line!=words) setRoot(line); break;
    }
   }
  }
  file.close();
 }
};

// зарежда в речника данните от поддиректория subDir
void loadDir(const QString &subDir){
 QDir dir(subDir);
 QStringList filters; // Филтър за имената на файловете
 filters << "bg*.dat";
 dir.setNameFilters(filters);
 QStringList fileList = dir.entryList(QDir::Files,QDir::Name); // Списък на имената на файловете в директорията
 if (fileList.size()>0){
  loadDescription(subDir); // Четене на информацията от файл description.dat
  QStringListIterator i(fileList);
  while (i.hasNext()){
   cFile = i.next();
   QString filePath = subDir;
   filePath.append("/").append(cFile);
   loadWords(filePath); // Четене на думите от поредния файл
  }
 }
};

// рекурсивно обхожда поддиректориите от dicDir и във всяка изпълнява loadDir 
void loadTo(const QString &dicDir, LangDictionary *langDic){ 
 QDir dir(dicDir);
 if (!dir.exists()){ // Ако директорията не съществува - прекратяване
  showMessage( qApp->translate("BGOffice::QApplication","Directory not exist<br>%1").arg(dicDir) );
  return;
 }
 QStringList dirList = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot); // Списък на поддиректориите
 QStringListIterator i(dirList);
 while (i.hasNext()){
  QString subDir = dicDir;
  subDir.append("/").append(i.next());
  loadTo(subDir,langDic);  // Рекурсивно извикване за четене на данните от поддиректория subDir
 }
 cLangDic = langDic;
 loadDir(dicDir); // Зареждане на данните от файловете в директорията
}

} // namespace BGOffice
