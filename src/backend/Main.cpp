/***************************************************************************
 *   Copyright (C) 2008 by I2P-Messenger   				   *
 *   Messenger-Dev@I2P-Messenger   					   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QtGui>
#include <QApplication>

#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QTime>

#include "form_Main.h"

QString Path;

void enableDebugLogging();
void myMessageHandler(QtMsgType type, const char *msg);
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	enableDebugLogging();
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	form_MainWindow* mainForm= new form_MainWindow();
	mainForm->show();
        app.exec();
	app.closeAllWindows();

	return 0;
}

void enableDebugLogging()
{
	if(QFile::exists(QApplication::applicationDirPath()+"/UseHomeForConfigStore")==true){
	      Path=QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
	      Path+="/.I2P-Messenger";
	}
	else{
	   Path=QApplication::applicationDirPath();
	}
	
	//is DebugPrint = enabled ?
	QSettings settings(Path+"/application.ini",QSettings::IniFormat);
	settings.beginGroup("General");
	  if(settings.value("DebugLogging","true").toBool()==true){
	    qInstallMsgHandler(myMessageHandler); 
	  }
	settings.endGroup();
}

void myMessageHandler(QtMsgType type, const char *msg)
{
	QString txt;
	
	txt.append(QTime::currentTime().toString("hh:mm:ss"));
	
	switch (type) {
	case QtDebugMsg:
	    txt.append(QString(" Debug: %1").arg(msg));;
	    break;
	case QtWarningMsg:
	    txt.append(QString(" Warning: %1").arg(msg));;
	break;
	case QtCriticalMsg:
	    txt.append(QString(" Critical: %1").arg(msg));;
	break;
	case QtFatalMsg:
	    txt.append(QString(" Fatal: %1").arg(msg));;
	}
	QFile outFile(Path+"/DebugLog.txt");
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << txt << endl;
}