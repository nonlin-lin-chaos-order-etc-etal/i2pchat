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

#ifndef SEEDLESSMANAGER_H
#define SEEDLESSMANAGER_H

#include <QtGlobal>
#include <QtDebug>
#include <QByteArray>
#include <QTextStream>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QTimer>
#include <QSettings>
#include <QTime>

#include "User.h"
#include "SeedlessAnnouncer.h"
#include "SeedlessSearcher.h"
#include "HttpOverStreamObject.h"


#define SEEDLESS_SERVICE_NAME "i2p-messenger"
#define SEEDLESS_SERVICE_STRUCT_VERSION "X01X"


#define MAX_FAILCOUNT 3

using namespace User;
class CSeedlessAnnouncer;
class CSeedlessSearcher;
class CCore;
class CSeedlessManager :public QObject{

public: struct SeedlessServerInformation{
        QString b32Dest;
        int	failCount;
    };

public: struct SeedlessSearchStruct {
        QString NickName;
        int	Age;
        QString Gender;
        QString Interests;
        QString b32Dest;
        QString b64Dest;
        int	TTL;
        SeedlessSearchStruct& operator =(const SeedlessSearchStruct& str) {
            this->NickName=str.NickName;
            this->Age=str.Age;
            this->Gender=str.Gender;
            this->Interests=str.Interests;
            this->b32Dest=str.b32Dest;
            this->b64Dest=str.b64Dest;
            this->TTL=str.TTL;
            return *this;
        }
    };
public: struct SeedlessTopicSubscribeStruct {
        QString topicId;
        //QString b32Dest;
        //QString b64Dest;
        int	ttl;
        SeedlessTopicSubscribeStruct & operator = (const SeedlessTopicSubscribeStruct& str) {
            this->topicId=str.topicId;
            //this->b32Dest=str.b32Dest;
            //this->b64Dest=str.b64Dest;
            this->ttl=str.ttl;
            return *this;
        }
    };

    Q_OBJECT
public:
    CSeedlessManager( CCore& Core,
                      QString FilePathToSeedlessList,
                      QString FilePathForSettings);

    ~CSeedlessManager();


    void 		doSeedlessMessangerSearch(SeedlessSearchStruct str);
    void        doSeedlessSubscribeToTopic(SeedlessTopicSubscribeStruct subscribeStruct);

    void		doStart();
    void		doStop();

    void 		printSeedlessServerList();
    QMap<QString, SeedlessServerInformation> getSeedlessServer() { return mSeedlessServerList; };

    bool		addNewUser(const SeedlessSearchStruct& str);
    bool        openTopic(SeedlessTopicSubscribeStruct topicSubscribeStruct);




private slots:
    void slotSeedlessAnnouncerFinished   (bool t,QStringList Header,QByteArray Data,QString ID);
    void slotSeedlessSearcherFinished    (bool t,QStringList Header,QList<QByteArray> Data,QString ID);
    void slotSeedlessServerSearchFinished(bool t,QStringList Header,QList<QByteArray> Data,QString ID);
    void slotTimerForReAnncounging();

signals:
    void DebugMessages(QString);
    void signSeedlessSearchResults(QMap<QString,CSeedlessManager::SeedlessSearchStruct>);
    void signSeedlessSearchNothingFound();
    void signSearchState(QString);

private:
    bool ExtractInteressingInformation(QString dataStringIn, QString& Destination, QString& TTL, const QString Service);
    SeedlessSearchStruct ExtractMessengerInformation(QByteArray data,bool &OK );

    void doSeedlessAnnounce();
    void doSeedlessSearch(QByteArray Data);
    void LoadSeedlessServerList();
    void SaveSeedlessServerList();
    void LoadFallBackSeedlessServerList();
    void SearchForMoreUseFullSeedlessServer();
    void CheckForMax_FailCount();
    void CheckAndFilterResult();
    int doCheckForValidB32(const QString B32Address)const;
    QString getMetaDataForAnnouncing();
    int getCountForSeedlessserverSearch()const;
    int getCountForSeedlessAnncounce()const{return getCountForSeedlessserverSearch();}
    int getCountForSeedlessMessengerSearch()const{return getCountForSeedlessserverSearch();}

    CCore& mCore;
    const 	QString			   mFilePathToSeedlessList;
    const 	QString			   mFilePathForSettings;

    QMap<QString, CSeedlessSearcher*>  mSeedlessServerSearchList;
    QMap<QString, CSeedlessSearcher*>  mSeedlessUserSearcherList;
    QMap<QString, CSeedlessAnnouncer*> mSeedlessAnnouncerList;
    QMap<QString, SeedlessServerInformation> mSeedlessServerList;

    bool				   mFirstServerSearch;
    SeedlessSearchStruct		   mCurrentSearch;
    QMap<QString,SeedlessSearchStruct> mSearchResult;
    QTimer				   mReAnnouncingTimer;
};
#endif
