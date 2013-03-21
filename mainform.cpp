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

#include <QtGui>
#include <QApplication>
#include <QFont>
#include <QFontDialog>

#include "mainform.h"
#include "showMessage.h"
#include "langDicModel.h"
#include "bgEnDictionary.h"
#include "myFileRoutines.h"

QString progVersion="0.3";
QString progURL="http://vanyog.com/grammar-bg";

MyMainWindow::MyMainWindow(QWidget *parent)
   : QMainWindow(parent)
{
   ui.setupUi(this);
   setMyStatusBar(statusBar());
   llView = new LargeListView(ui.centralwidget);
   ui.vboxLayout->addWidget(llView);
   searchDialog = new SearchDialog(llView,this);
   cRfp = 0; cRoot = 0;
   bgEnDic = new BgEnDictionary("dic-data/bg-en/");
   thesDic = new BgEnDictionary("dic-data/thesaurus/");
   wordAdded = false;
   readSettings();

   connect(ui.radioButton, SIGNAL(toggled(bool)), this, SLOT(changeRootsAndForms(bool)) );
   connect(ui.radioButton_2, SIGNAL(toggled(bool)), this, SLOT(changeRootsAndForms(bool)) );
   connect(llView->lineEdit(), SIGNAL(textChanged(QString)), this, SLOT(onWordEditChanged(QString)) );
   connect(llView->listView(), SIGNAL(clicked(QModelIndex)), this, SLOT(onWordClicked(QModelIndex)) );
   connect(ui.listView, SIGNAL(clicked(QModelIndex)), this, SLOT(onWordFormClicked(QModelIndex)) );
   connect(ui.pushButton_3, SIGNAL(pressed()), this, SLOT(onAddButtonPressed()));

   connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTabChanged(int)) );
   connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onToolsContinueSpelling()) );

// меню
   connect(ui.actionSave_as, SIGNAL(triggered()), this, SLOT(onFileSaveAs()) );
   connect(ui.actionOpen_dat_file, SIGNAL(triggered()), this, SLOT(onFileOpenDat()) );
   connect(ui.actionExplore_data_directory, SIGNAL(triggered()), this, SLOT(onFileExploreData()) );
   connect(ui.actionDelete_Index_Files, SIGNAL(triggered()), this, SLOT(onFileDeleteIndexFiles()) );
   connect(ui.actionLoad_IDI_Dictionary, SIGNAL(triggered()), this, SLOT(onFileIDIDic()) );

   connect(ui.action_Search, SIGNAL(triggered()), this, SLOT(onEditSearch()) );

   connect(ui.actionFont, SIGNAL(triggered()), this, SLOT(onFontAction()) );
   connect(ui.actionAutoCSpellcheck, SIGNAL(triggered()), this, SLOT(onAutoCSPellcheck()) );

   connect(ui.action_Spell_check_the_clipboard, SIGNAL(triggered()), this, SLOT(onToolsSpellCheckClipboard()) );
   connect(ui.action_Spell_check_file, SIGNAL(triggered()), this, SLOT(onToolsSpellCheckFile()) );
   connect(ui.actionContinue_check, SIGNAL(triggered()), this, SLOT(onToolsContinueSpelling()) );

   connect(ui.actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()) );
   connect(ui.actionAbout_Program, SIGNAL(triggered()), this, SLOT(onAboutPr()) );
   connect(ui.actionDictionary_Info, SIGNAL(triggered()), this, SLOT(onDictionaryInfo()) );
}

QStringList * MyMainWindow::cList(){
   if (ui.radioButton->isChecked()) return langDic.WordRootsList();
   else return langDic.WordFormsList();
};

WordRootFormPairList * MyMainWindow::rfpList(){
   if (ui.radioButton->isChecked()) return langDic.rootPairs();
   else return langDic.formPairs();
};

void MyMainWindow::changeRootsAndForms(bool yes){
   LangDicModel *model = new LangDicModel(rfpList());
//   return;
   llView->listView()->setModel(model);
   statusBar()->showMessage( tr("%1 words")
      .arg(QString::number(rfpList()->size())) );
};

//---------private----------

void MyMainWindow::closeEvent(QCloseEvent *event){
   writeSettings();
   statusBar()->showMessage(tr("Saving files..."));
   if (!wordAdded) langDic.saveFiles();
   QWidget::closeEvent(event);
}

void MyMainWindow::showInfo(int i){
   cRoot = cRfp->root();
   QString tx = cRoot->info();
   if (i>0) tx.append("<BR><BR>").append(cRoot->form(i)->info());
   ui.textBrowser->setHtml(tx);
};

void MyMainWindow::showFormList(){
   QStringListModel *model = new QStringListModel();
   model->setStringList(*cRoot->wordForms());
   ui.listView->setModel(model);
};

void MyMainWindow::showByTab(int tab, int i){
   if (tab==3) ui.frame->show();
   else ui.frame->hide();
   if (i<0) return;
   cRfp = rfpList()->at(i);
   QString w;
   switch (tab){
   case 0:  // Морфология
      showInfo(cRfp->formIndex());
      showFormList();
      break;
   case 1:  // Превод на английски
      w = cRfp->root()->word();
      ui.textBrowser->setText(bgEnDic->translation(w));
      break;
   case 2:  // Синоними
      w = cRfp->root()->word();
      ui.textBrowser->setText(thesDic->translation(w));
      break;
   }
};

void MyMainWindow::writeSettings(){
   statusBar()->showMessage(tr("Saving status..."));
   QSettings *settings = new QSettings("VanyoG","grammar-bg");
   settings->setValue("mainWindowGeometry",saveGeometry());
   settings->setValue("fontFamily",font().family());
   settings->setValue("fontPointSize",font().pointSize());
   settings->setValue("fontWeight",font().weight());
   settings->setValue("fontItalic",font().italic());
   settings->setValue("sp2State",ui.splitter_2->saveState());
   settings->setValue("spState",ui.splitter->saveState());
   settings->setValue("tabIndex",ui.comboBox->currentIndex());
   settings->setValue("autoCSpellcheck",ui.actionAutoCSpellcheck->isChecked());
   settings->setValue("lastWord",llView->lineEdit()->text());
};

void MyMainWindow::readSettings(){
   QSettings *settings = new QSettings("VanyoG","grammar-bg");
   restoreGeometry(settings->value("mainWindowGeometry").toByteArray());
   setFont(QFont(
      settings->value("fontFamily").toString(),
      settings->value("fontPointSize").toInt(),
      settings->value("fontWeight").toInt(),
      settings->value("fontItalic").toBool()
   ));
   ui.splitter_2->restoreState(settings->value("sp2State").toByteArray());
   ui.splitter->restoreState(settings->value("spState").toByteArray());
   int i = settings->value("tabIndex").toInt();
   ui.comboBox->setCurrentIndex(i);
   onTabChanged(i);
   ui.actionAutoCSpellcheck->setChecked(settings->value("autoCSpellcheck").toBool());
   if (ui.actionAutoCSpellcheck->isChecked())
      connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(onToolsSpellCheckClipboard()) );
   else
      disconnect(QApplication::clipboard(), SIGNAL(dataChanged()), 0, 0);
   QString w = settings->value("lastWord").toString();
   llView->lineEdit()->setText(w);
//   onWordEditChanged(w);
};

void MyMainWindow::spellCheckFile(){
   QStringList a = qApp->arguments();
   if (a.size()>1) spellCheckFile(a.at(1));
};

void MyMainWindow::spellCheckFile(const QString &fn){
      QFile file(fn);
      if (file.exists()){
         QString fc = fileContent(file.fileName());
//         showMessage(fc);
         ui.textBrowser->setPlainText(fc);
         ui.comboBox->setCurrentIndex(3);
         onToolsContinueSpelling();
      }
};

void MyMainWindow::onWordClicked(const QModelIndex &mi){
   int i = mi.row();
   showByTab(ui.comboBox->currentIndex(),i);
};

void MyMainWindow::onWordFormClicked(const QModelIndex &mi){
   int i = mi.row();
   if (cRoot->size()>1) i++;
   showInfo(i);
};

/*void MyMainWindow::wordIndexesMoved(const QModelIndexList &mil){
   showMessage("OK");
};

void MyMainWindow::wordActivated(const QModelIndex &index){
   showMessage("Activated");
};*/

void MyMainWindow::onWordEditChanged(const QString &tx){
   int i = -1;
   if (ui.radioButton->isChecked()) i = langDic.indexOfRoot(tx);
   else i = langDic.indexOfForm(tx);
//   showMessage(i);
   if (i>-1){
      QModelIndex in = llView->listView()->model()->index(i,0,QModelIndex());
      llView->listView()->setCurrentIndex(in);
      onWordClicked(in);
   }
};

void MyMainWindow::onAddButtonPressed(){
   if (!cRoot){
      showMessage(tr("No word is selected."));
      return;
   }
   QString w0 = cRoot->word();
   bool ok;
   QString w = QInputDialog::getText(this, 
     tr("Ward to add"), tr("in the same group with <strong>%1</strong>: ").arg(w0), QLineEdit::Normal, "", &ok);
   if (!ok || !w.size()) return;
   QString fn = cRoot->value(QString::fromUtf8("таблица"));
   QStringList fc = fileContent(fn).split("\n");
   QString ln0 = "";
   QString ln  = "";
   int i;  ok  = false;
   for(i=0; i<fc.size(); i++){
     ln = fc.at(i).trimmed();
     if (ln=="Думи:"){ ok=true; continue;}
     if (ok){
        if ( ((w>ln0) && (w<ln)) || !ln.size()) break;
        ln0 = ln;
     }
   }
   fc.insert(i,w);
   i = fc.size()-1;
   while (!fc.at(i).trimmed().size()){ fc.removeAt(i); i = fc.size()-1; }
   ln = fc.join("\n");
   saveToFile(fn,ln);
   wordAdded = true;
};

void MyMainWindow::onTabChanged(int tab){
   int i = llView->index();
   showByTab(tab,i);
};

void MyMainWindow::onFontAction(){
   bool ok;
   QFont font = QFontDialog::getFont(&ok, this);
   if (ok) {
      setFont(font);
   }
};

void MyMainWindow::onAutoCSPellcheck(){
   if (ui.actionAutoCSpellcheck->isChecked())
      connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(onToolsSpellCheckClipboard()) );
   else
      disconnect(QApplication::clipboard(), SIGNAL(dataChanged()), 0, 0);
};

void MyMainWindow::onAboutPr(){
   showMessage(tr("grammar-bg v%1<br>Copyright (C) 2006  Vanyo Georgiev &lt;<A HREF=mailto:info@vanyog.com>info@vanyog.com</A>&gt;<br><A HREF=%2>%2</A><br><br>This program is free software; you can redistribute it and/or<br>modify it under the terms of the GNU General Public License<br>as published by the Free Software Foundation; either version 2<br>of the License, or (at your option) any later version.<br><br>This program is distributed in the hope that it will be useful,<br>but WITHOUT ANY WARRANTY; without even the implied warranty of<br>MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the<br>GNU General Public License for more details.<br><br>You should have received a copy of the GNU General Public License<br>along with this program; if not, write to the Free Software<br><br>Foundation, Inc., 51 Franklin Street, Fifth Floor,<BR>Boston, MA  02110-1301, USA.").arg(progVersion).arg(progURL) );
};

void MyMainWindow::onDictionaryInfo(){
   showMessage(langDic.info());
};

void MyMainWindow::onFileSaveAs(){
   QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save as"), "", tr("Text files (*.txt)"));
   if (fileName.size()==0) return;
   QFileInfo fileInfo(fileName);
   QString secondName = fileInfo.path()+"/"+fileInfo.baseName()+".index";
   QFile file(fileName);
   QFile secondFile(secondName);
   bool ok = secondFile.open(QIODevice::WriteOnly);
   if (file.open(QIODevice::WriteOnly)){
      statusBar()->showMessage( tr("Writing..."));
      QTextStream out(&file);
      QDataStream out1;
      if (ok) out1.setDevice(&secondFile); 
      out.setCodec(QTextCodec::codecForName("cp1251"));
      int count = llView->count();
      QString s0 = "";
      unsigned int pos = 0;
      short int len;
      for(int i=0;i<count;i++){
         QString s = llView->item(i);
         if (s!=s0){
            out << s << "\r\n";
            if (ok) {
               len = s.size();
               out1 << pos << len;
               pos += len + 2;
            }
         }
         s0 = s;
      }
      file.close();
      if (ok) secondFile.close();
      statusBar()->showMessage( tr("%1 words").arg(QString::number(count)) );
   }
   else showMessage(tr("Can't write to file:<br>%1").arg(fileName));
};

void MyMainWindow::onFileOpenDat(){
  if (!cRoot) return;
  QString w = cRoot->word();
  QApplication::clipboard()->setText(w);
  QString fn = cRoot->value(QString::fromUtf8("таблица"));
  if (!QFileInfo(fn).exists()) return;
  QProcess *p = new QProcess();
  QStringList a;
  a << fn;
#ifdef Q_WS_MAC
  p->start("open",a);
#else
  if (QFileInfo("C:/Program Files/Windows NT/Accessories/wordpad.exe").exists())
     p->start("C:/Program Files/Windows NT/Accessories/wordpad.exe",a);
  else
     p->start("D:/Program Files/Windows NT/Accessories/wordpad.exe",a);
#endif
};

void MyMainWindow::onFileExploreData(){
/*  if (!cRoot) return;
  QString dn = QFileInfo(cRoot->value(QString::fromUtf8("таблица"))).absolutePath();
  QProcess *p = new QProcess();
  QStringList a;
  a << dn;
#ifdef Q_WS_MAC
  p->start("open",a);
#else
  showMessage("Not implemented for Windows");
#endif*/

   QStringList a;
   QString d = QFileInfo(cRoot->value(QString::fromUtf8("таблица"))).absolutePath();
   d = d.replace("/",QDir::separator());
   QProcess *p = new QProcess();
#ifdef Q_WS_WIN
   a << "/n,/e," << d;
   p->start("explorer",a);
#else
   a << d;
   p->start("open",a);
#endif

};

void MyMainWindow::onFileDeleteIndexFiles(){
   
};

void MyMainWindow::onFileIDIDic(){
  IDIDictionary::loadTo("_D2_ungrouped_sorted.txt",&idiDic);
  showMessage(tr("Loaded"));
  for(int i=0; i<5; i++){
     QString w = idiDic.at(i)->word();
     int j = langDic.indexOfRoot(w);
     bool e = (w == langDic.root(j)->word() );
     if (!e) showMessage( w );
  }
  
};

void MyMainWindow::onEditSearch(){
   searchDialog->resetProgressBar();
   searchDialog->exec();
};

void MyMainWindow::onToolsSpellCheckClipboard(){
   QString s = QApplication::clipboard()->text();
   ui.textBrowser->setPlainText(s);
   ui.comboBox->setCurrentIndex(3);
   onToolsContinueSpelling();
};

void MyMainWindow::onToolsSpellCheckFile(){
   QString fn = QFileDialog::getOpenFileName(this);
   spellCheckFile(fn);
};

void MyMainWindow::onToolsContinueSpelling(){
   if (ui.comboBox->currentIndex()!=3) return;
   QString s = ui.textBrowser->toPlainText();
   QTextCursor tc = ui.textBrowser->textCursor();
   SpellingMistake sm = langDic.findMistake(s,tc.position());
   if (sm.mistakeType != SpellingMistake::NoMistake){
     tc.setPosition(sm.start);
     tc.setPosition(sm.end,QTextCursor::KeepAnchor);
     llView->lineEdit()->setText(sm.text);
     ui.textBrowser->setTextCursor(tc);
     QApplication::clipboard()->setText(sm.text);
   }
   else ui.textBrowser->moveCursor(QTextCursor::Start);
   showStatus( sm.note() );
};

void MyMainWindow::onToolsReplace(){

};
