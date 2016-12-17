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

#ifndef SEEDLESSANNOUNCER_H
#define SEEDLESSANNOUNCER_H

#include <QtGlobal>
#include <QtDebug>
#include <QStringList>

class CHttpOverStreamObject;
class CCore;
class CSeedlessAnnouncer:public QObject{

	Q_OBJECT
	
	public:
		CSeedlessAnnouncer(CCore& Core,QString ID);
		~CSeedlessAnnouncer();
		QString getID()const {return mID;};
		QString getDestination()const {return mDestination;};

		void doAnnounce(QString ServerAddress,QString ServiceName,QByteArray DataForSend);

	signals:
		void signDoneSuccessfully(bool b ,QStringList header,QByteArray data,QString ID);

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
