/* Copyright (C) 2007  Vanyo Georgiev <info@vanyog.com>
*  
*  This program is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  as published by the Free Software Foundation; either version 2
*  of the License, or (at your option) any later version.
*  
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*  
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
*  USA. */

#include <QFileDialog>
#include <QTextStream>
#include <QTextCodec>

#include "searchDialog.h"
#include "showMessage.h"

SearchDialog::SearchDialog(LargeListView *llView, QWidget *parent)
  :QDialog(parent)
{
 ui.setupUi(this);
 ss0="";
 largeListView = llView;
 ui.progressBar->reset();
 connect(ui.radioButton, SIGNAL(clicked()), this, SLOT(onRadioButton()) );
 connect(ui.radioButton_2, SIGNAL(clicked()), this, SLOT(onRadioButton()) );
 connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onFindNext()) );
 connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(onSaveToFile()) );
}

void SearchDialog::resetProgressBar(){
 int i = largeListView->count();
 ui.progressBar->setMaximum(i);
 ui.progressBar->reset();
 step100 = i / 1000;
};

void SearchDialog::setupLisr(){
 QString ss = ui.lineEdit->text();
 if (ss!=ss0) {
  ss0 = ss;
  sw = ss.split(" ");
  ui.progressBar->reset();
 }
};

void SearchDialog::onRadioButton(){
 ui.progressBar->reset();
};

void SearchDialog::onFindNext(){
 setupLisr();
 if (ss0.size()<1) return;
 int i=ui.progressBar->value();
 int j;
 if (ui.radioButton->isChecked()) j=findNext(i,&sw);
 else j=findNext2(i,&sw);
 if (j>i) {
  ui.progressBar->setValue(j);
  QString s = largeListView->item(j);
  largeListView->lineEdit()->setText(s);
  showMessage(largeListView->item(j));
 }
 else {
  showMessage(tr("No more found.<BR>Next search will start from the begining."));
  ui.progressBar->reset();
 }
};

void SearchDialog::onSaveToFile(){
 setupLisr();
 if (ss0.size()<1) return;
 QString fileName = QFileDialog::getSaveFileName(
         this, tr("Save as"), "", tr("Text files (*.txt)") );
 QFile file(fileName);
 if (file.open(QIODevice::WriteOnly)){
  ui.progressBar->reset();
  QTextStream out(&file);
  out.setCodec(QTextCodec::codecForName("cp1251"));
  int j=ui.progressBar->value(), i, n = 0;
  QString s0 = "";
  do {
   i=j;
   if (ui.radioButton->isChecked()) j=findNext(i,&sw);
   else j=findNext2(i,&sw);
   QString s = largeListView->item(j);
   if (s!=s0){
    out << s << "\r\n";
	n++;
   }
   s0 = s;
  } while (j>i);
  file.close();
  showMessage(tr("%1 items were saved").arg(QString::number(n)));
  ui.progressBar->reset();
 }
 else showMessage(tr("Can't write to file:<br>%1").arg(fileName));
};

int SearchDialog::findNext(int i, QStringList *sw){
 i++;
 int j;
 for(j=i; j<largeListView->count(); j++){
  QString s=largeListView->item(j);
  bool yes = true;
  for(int k=0; k < sw->size(); k++){
   QString w = sw->at(k);
   if (w.size()>0) yes = yes && s.contains(w);
   if (!yes) break;
  }
  if (j % step100 == 0) ui.progressBar->setValue(j);
  if (yes) return j;
 }
 ui.progressBar->setValue(j);
 return i-1;
};

int SearchDialog::findNext2(int i, QStringList *sw){
// notReady(); return i;
 i++;
 int j;
 for(j=i; j < largeListView->count(); j++){
  if (j % step100 == 0) ui.progressBar->setValue(j);
  QString s = largeListView->item(j);
  int k=0;
  bool yes = true;
  while (k < s.size()){
   int k0 = k;
   for(int l=0; l < sw->size(); l++){
    int p = s.indexOf(sw->at(l),k);
	if (p==k) { k = k + sw->at(l).size(); break; }
   }
   if (k==k0) { yes=false; break; }
  }
  if (yes) return j;
 }
 ui.progressBar->setValue(j);
 return i-1;
};

void SearchDialog::notReady(){
 showMessage(tr("This function is not realized yet."));
};
