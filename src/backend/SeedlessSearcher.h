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
#ifndef SEEDLESSSEARCHER_H
#define SEEDLESSSEARCHER_H

#include <QStringList>
#include <QByteArray>
#include <QList>

#include "Core.h"

class CCore;
class CHttpOverStreamObject;
class CSeedlessSearcher: public QObject{

	Q_OBJECT
	
	public:
		CSeedlessSearcher(CCore& Core,QString ID);
		~CSeedlessSearcher();
		QString getID()const {return mID;};
		QString getDestination() const {return mDestination;};

		void doSearch(QString ServerAddress,QString ServiceName,QByteArray DataForSearch);
	signals:
		void signDoneSuccessfully(bool b ,QStringList header,QList<QByteArray> data,QString ID);

	private slots:
		void slotHttpOverStreamObjectFinished(bool, QByteArray);

	private:
		CCore&			mCore;
		const QString		mID;
		CHttpOverStreamObject*	mHttpOverStreamObject;
		QString 		mDestination;
		
		void ExtractAndRemoveHeaderFromData(QStringList& Header,QByteArray& Data);
};		
#endif 
