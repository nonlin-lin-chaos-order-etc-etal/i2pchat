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

#ifndef CORE_IMPL_H
#define CORE_IMPL_H

#include <QtGui>
#include <QSettings>
#include <QTextStream>
#include <QList>
#include <QIODevice>

#include "../controller/RosterController.h"
#include "Connector.h"
#include "AbstractUserLocalImage.h"
#include "I2PSamMessageAnalyser.h"
#include "DebugMessageManager.h"
#include "SoundManager.h"
#include "UserBlockManager.h"
#include "FileTransferManager.h"
#include "SwarmsController.h"
#include "UnsentChatMessagesStorage.h"

using namespace SAM_Message_Types;
using namespace AbstractUserLocalImage;

class UserManager;
class ConnectionManager;
class FileTransferManager;
class CPacketManager;
class Roster;
class RosterController;

class AppContext :public QObject
{
    Q_OBJECT
public:
    AppContext(QString configPath);
    ~AppContext();

    //forbid some operators
    AppContext(const AppContext&)=delete;
    AppContext& operator=(const AppContext&)=delete;

    QString 		getDestinationByID(qint32 ID) 	const;
    const 	QString 		getMyDestination() 		const;
    const 	QString 		getMyDestinationB32() 		const;
    ONLINE_STATE 		getOnlineStatus()		const;
    QString 		getClientName()			const 	{return "I2PChat (Qt), hypnotic fork";};
    QString 		getClientVersion()		const	{return QString("0.3.2-")+GIT_VERSION;};
    QString 		getProtocolVersion()		const	{return mProtocol->getProtocolVersion();};
    CI2PStream* 		getI2PStreamObjectByID(qint32 ID)const;
    const UserInfo	getUserInfos()			const;
    QString 		    getConnectionDump()		const;
    const 	QString		getConfigPath()			const	{return mConfigPath;};

    //<SUBSYSTEMS>
    DebugMessageManager* 	getDebugMessageHandler()	const	{return mDebugMessageHandler;};
    ConnectionManager*	    getConnectionManager()		const	{return mConnectionManager;};
    UserBlockManager*      getUserBlockManager()		const	{return mUserBlockManager;};
    Connector*              getProtocol()               const	{return mProtocol;};
    SoundManager*          getSoundManager()           const	{return mSoundManager;};
    UserManager*           getUserManager()            const	{return mUserManager;};
    FileTransferManager*	getFileTransferManager()	const	{return mFileTransferManager;};
    //</SUBSYSTEMS>

    void			setUserProtocolVersionByStreamID ( qint32 ID,QString Version );
    void 			setOnlineStatus(const ONLINE_STATE newStatus);
    void 			setStreamTypeToKnown(qint32 ID,const QByteArray Data,bool isFileTransfer_Recive=false);
    void			setMyDestinationB32(QString B32Dest);

    bool 			useThisChatConnection(const QString Destination,const qint32 ID);

    void 			doNamingLookUP ( QString Name )const;
    void			doConvertNumberToTransferSize(quint64 inNumber,QString& outNumber,QString& outType,bool addStoOutType=true)const;

    void 			deletePacketManagerByID ( qint32 ID );
    void 			createStreamObjectsForAllUsers();
    void 			createStreamObjectForUser(AbstractUserLocalImage& AbstractUserLocalImage);
    void 			loadUserInfos();
    QString			calcSessionOptionString()const;

    QString         canonicalizeTopicId(QString topicIdNonCanonicalized);

private slots:
    // <SIGNALS FROM CONNECTIONMANAGER>
    void slotStreamStatusRecived ( const SAM_Message_Types::RESULT result,const qint32 ID,QString Message );
    void slotNamingReplyRecived ( const SAM_Message_Types::RESULT result,QString Name,QString Value="",QString Message="" );
    void slotStreamControllerStatusOK(bool Status);
    void slotIncomingStream(CI2PStream* stream);
    void slotNewSamPrivKeyGenerated(const QString SamPrivKey);
    // </SIGNALS FROM CONNECTIONMANAGER>

signals:
    void signUserStatusChanged();
    void signOnlineStatusChanged();
    void signOwnAvatarImageChanged();
    void signNicknameChanged();

private:
    ConnectionManager*         mConnectionManager;
    DebugMessageManager*       mDebugMessageHandler;
    SoundManager*              mSoundManager;
    Connector*                  mProtocol;
    UserBlockManager*          mUserBlockManager;
    UserManager*               mUserManager;
    FileTransferManager*       mFileTransferManager;
    UnsentChatMessagesStorage*  mUnsentChatMessageStorage;

    UserInfo               mUserInfos;
    QString                     mMyDestination;
    QString                     mMyDestinationB32;
    QString                     mConfigPath;
    QList<CPacketManager*>      mDataPacketsManagers;
    ONLINE_STATE                 mCurrentOnlineStatus;
    ONLINE_STATE                 mNextOnlineStatus;

    RosterController* rosterController;
    SwarmsController * swarmsManager;

    void init();
    void stopCore();
    void restartCore();
    void closeAllActiveConnections();

public:
    Roster& getRosterModel();
    RosterController& getRosterController();
    SwarmsController * getSwarmsManager(){return swarmsManager;}
};
#endif //
