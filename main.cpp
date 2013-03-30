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

#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>

#include "mainform.h"
#include "showMessage.h"
#include "myFileRoutines.h"
#include "grammar.h"

// Директория с данни
QString dataDir = "data";

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    QTranslator translator;
    translator.load("grammar-bg_"+QLocale::system().name());
    app.installTranslator(&translator);

    MyMainWindow window;
    window.show();

    // Зареждане на речник
    
    // Функция, дефинирана в showMessage.h - показва надпис върху status bar
    showStatus(app.translate("","Loading dictionary..."));
    
    // namespace BGDicdata, дефиниран в grammar.h и bgdicdata.cpp
    BGDicdata::loadTo(dataDir,&window.langDic);

    window.changeRootsAndForms(true);
    window.setPList();

    window.spellCheckFile();

    return app.exec();

    return 0;

}
