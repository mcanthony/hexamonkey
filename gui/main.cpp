//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <QtGui>
#include <QApplication>
#include <QIcon>

#include "moduleloader.h"
#include "interpreterconstructor.h"
#include "mainwindow.h"

#include "standardmodule.h"
#include "mp4module.h"
#include "ebmlmodule.h"
#include "hmcmodule.h"
#include "mkvmodule.h"
#include "tsmodule.h"
#include "fromfilemodule.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("HexaMonkey");
    QCoreApplication::setApplicationName("HexaMonkey");

    ModuleLoader moduleLoader;

    moduleLoader.addModule("bestd", new StandardModule(true));
    moduleLoader.addModule("lestd", new StandardModule(false));
    moduleLoader.addModule("mp4",   new Mp4Module);
    moduleLoader.addModule("ebml",  new EbmlModule);
    moduleLoader.addModule("mkv",   new MkvModule);
    moduleLoader.addModule("hmc",   new HmcModule);
    moduleLoader.addModule("ts",    new TsModule);

    InterpreterConstructor interpreterConstructor(static_cast<const HmcModule&>(moduleLoader.loadModule("hmc")));

#ifdef __MINGW32__
    moduleLoader.addFolder(".\\scripts\\", interpreterConstructor);
    moduleLoader.addFolder("..\\scripts\\", interpreterConstructor);
#else
    moduleLoader.addFolder("./scripts/", interpreterConstructor);
    moduleLoader.addFolder("../scripts/", interpreterConstructor);
#endif

    MainWindow window(moduleLoader, interpreterConstructor);

    window.setWindowTitle("HexaMonkey");
    window.setWindowIcon(QIcon(":/logo.svg"));
    window.showMaximized();

    for(int i = 1; i < argc; ++i)
    {
        window.openFile(argv[i]);
    }
    
    return a.exec();
}