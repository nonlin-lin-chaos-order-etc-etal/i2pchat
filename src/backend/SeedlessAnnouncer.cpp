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


#include "SeedlessAnnouncer.h"
#include "Core.h"
#include "HttpOverStreamObject.h"

CSeedlessAnnouncer::CSeedlessAnnouncer(CCore& Core,QString ID)
:mCore(Core),mID(ID)

{
	mHttpOverStreamObject= new CHttpOverStreamObject(*(mCore.getConnectionManager()));
	
	connect(mHttpOverStreamObject,SIGNAL(signDoneSuccessfully(bool, QByteArray)),this,
		SLOT(slotHttpOverStreamObjectFinished(bool,QByteArray)));
	
}

CSeedlessAnnouncer::~CSeedlessAnnouncer()
{
	delete mHttpOverStreamObject;
}

void CSeedlessAnnouncer::slotHttpOverStreamObjectFinished(bool b, QByteArray data)
{
	QStringList	header;
	ExtractAndRemoveHeaderFromData(header,data);

	if(b==false){
		emit signDoneSuccessfully(false,header,data,mID);
	}
	
	if(header.isEmpty() || header.at(0).contains("HTTP/1.1 200 OK")==false){
		emit signDoneSuccessfully(false,header,data,mID);
		return;
	}
	emit signDoneSuccessfully(true,header,data,mID);
}

void CSeedlessAnnouncer::doAnnounce(QString ServerAddress,QString ServiceName,QByteArray DataForSend)
{
	QString		MetaDataForAnnouncing;
	QByteArray	dataBase64Encoded;
	QByteArray 	data;
	QStringList	header;  

	mDestination=ServerAddress;
	
	MetaDataForAnnouncing =ServiceName;
	MetaDataForAnnouncing+=" ";
	MetaDataForAnnouncing+=DataForSend;	
	data.append(MetaDataForAnnouncing);
	dataBase64Encoded=data.toBase64();

	header.append("/Seedless/seedless HTTP/1.1\n");
	header.append("Accept: text/html\n");
	header.append("Accept-Charset: utf-8\n");
	header.append("X-Seedless: announce "+dataBase64Encoded);
	header.append("\n\n");
	
	mHttpOverStreamObject->doHttpRequest(CHttpOverStreamObject::GET,ServerAddress,header);
}

void CSeedlessAnnouncer::ExtractAndRemoveHeaderFromData(QStringList& Header, QByteArray& Data)
{
    Header.clear();
    QString CurrentLine;
    
    QTextStream	in(Data);
    in.setCodec(QTextCodec::codecForName("utf-8")); 
    
    while(!in.atEnd()){
	    CurrentLine=in.readLine();
	    if(CurrentLine.isEmpty()==false){
		Header.append(CurrentLine);
	    }else{
		//is empty
		Data.remove(0,in.pos());
		return;
	    }
    } 
}
