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

#include "SeedlessSearcher.h"

#include "HttpOverStreamObject.h"

CSeedlessSearcher::CSeedlessSearcher(CCore& Core,QString ID)
:mCore(Core),mID(ID)
{
   mHttpOverStreamObject= new CHttpOverStreamObject(*(mCore.getConnectionManager()));
   connect(mHttpOverStreamObject,SIGNAL(signDoneSuccessfully(bool,QByteArray)),this,
	   SLOT(slotHttpOverStreamObjectFinished(bool,QByteArray)));
	   
   
}

CSeedlessSearcher::~ CSeedlessSearcher()
{
    delete mHttpOverStreamObject;
}


void CSeedlessSearcher::slotHttpOverStreamObjectFinished(bool b, QByteArray data)
{
	
	QStringList	header;
	QList<QByteArray> dataForEmit;	
	
	ExtractAndRemoveHeaderFromData(header,data);

	if(b==false ) {
		emit signDoneSuccessfully(false,header,dataForEmit,mID);
		return;
	}
	
	if(header.isEmpty() || header.at(0).contains("HTTP/1.1 200 OK")==false){
		emit signDoneSuccessfully(false,header,dataForEmit,mID);
		return;
	}
	


	QTextStream	in(data);
	QByteArray tmp;
	while(!in.atEnd()){
	    tmp.clear();
	    tmp.append(in.readLine());
	    dataForEmit.append(tmp);
	}
	
	for(int i=0;i<dataForEmit.count();i++){
	    dataForEmit[i] = QByteArray::fromBase64(dataForEmit.at(i));
	    
	    if(dataForEmit.at(i).isEmpty()==true){
		dataForEmit.removeAt(i);
	    }
	}
	emit signDoneSuccessfully(true,header,dataForEmit,mID);
}

void CSeedlessSearcher::doSearch(QString ServerAddress, QString ServiceName, QByteArray DataForSearch)
{
	mDestination=ServerAddress;
	DataForSearch.insert(0,ServiceName+" ");
	
	QStringList header;
	header.append("/Seedless/seedless HTTP/1.1\n");
	header.append("Accept: text/html\n");
	header.append("Accept-Charset: utf-8\n");
	header.append("X-Seedless: locate "+DataForSearch.toBase64());
	header.append("\n\n");
	
	mHttpOverStreamObject->doHttpRequest(CHttpOverStreamObject::GET,ServerAddress,header);
}


void CSeedlessSearcher::ExtractAndRemoveHeaderFromData(QStringList& Header, QByteArray& Data)
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
