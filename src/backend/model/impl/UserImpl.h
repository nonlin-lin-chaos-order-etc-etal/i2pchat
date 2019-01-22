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
#ifndef I2P_CHAT_USER_IMPL_H
#define I2P_CHAT_USER_IMPL_H

#include <QtGui>
#include <QStringList>
#include <QTime>
#include <QtGlobal>
#include <QByteArray>

#include "I2PStream.h"

#include "SwarmType1LocalImage.h"

class AppContext;
class Connector;
class ChatMessageChangerUtil;
class MessageAckManager;
class AbstractUserLocalImage: public QObject
{
	Q_OBJECT
	public:
	enum CONNECTION_TO_USER{
		OFFLINE,
		ONLINE,
		TRYTOCONNECT,
        CONNECTION_ERROR
	};

	enum ONLINE_STATE{
		USERONLINE,
		USEROFFLINE,
		USERINVISIBLE,
		USERWANTTOCHAT,
		USERAWAY,
		USERDONT_DISTURB,
		USERTRYTOCONNECT,
		USERBLOCKEDYOU
	};

	enum USER_INFO_FIELDS{
		NICKNAME,
		GENDER,
		AGE,
		INTERESTS
	};

	struct UserInfo{
		QString Nickname;
		QString Gender;
		qint32  Age;
		QString Interests;
		QByteArray AvatarImage;
	};
					AbstractUserLocalImage(	AppContext&	appCtx,
						Connector& Protocol,
						QString Name,
						QString I2PDestination,
						qint32 I2PStream_ID
					);
					~AbstractUserLocalImage();

					//forbid some operators
					AbstractUserLocalImage(const AbstractUserLocalImage&)=delete;
					AbstractUserLocalImage& operator=(const AbstractUserLocalImage&)=delete;
	
	const 	QString 		getName()		const{return mName;}
	const 	QString 		getI2PDestination()	const{return mI2PDestination;}
	      	qint32  		getI2PStreamID()	const{return mI2PStream_ID;}
	const 	QString 		getProtocolVersion()	const{return mProtocolVersion;}
		double			getProtocolVersion_D()	const;
	const 	QString 		getClientName()		const{return mClientName;}
	const	QString 		getClientVersion()	const{return mClientVersion;}
	      	QColor  		getTextColor()		const{return mTextColor;}
	      	QFont   		getTextFont()		const{return mTextFont;}
		CONNECTION_TO_USER 	getConnectionStatus()	const{return mConnectionStatus;}
		ONLINE_STATE 	 	getOnlineStatus()	const{return mCurrentOnlineState;}
	const 	QStringList&		getAllChatMessages();
	const	QStringList		getNewMessages(bool haveFocus);
	const	UserInfo		getRecivedUserInfos()	const{return mRecivedUserInfos;}

	const	QString			getHighestUsableProtocolVersionFiletransfer()  const;
		double			getHighestUsableProtocolVersionFiletransfer_D()const;
	const	QString			getMaxProtocolVersionFiletransfer()	const {return mMaxProtocolVersionFiletransfer;};
	 	double			getMaxProtocolVersionFiletransfer_D()	const;
	const	QString			getMinProtocolVersionFiletransfer()	const {return mMinProtocolVersionFiletransfer;};
		double			getMinProtocolVersionFiletransfer_D()	const;
		bool 			getHaveNewUnreadMessages()		const {return mHaveNewUnreadMessages;}
		bool			getHaveNewUnreadChatmessages()		const {return mHaveNewUnreadChatmessage;}
		bool			getIsInvisible()			const {return mInvisible;};
		bool			getUsedB32Dest()			const {return mUseB32Dest;};
	const 	QStringList		getUnsentedMessages()			const {return mUnsentedMessages;};

		void 			setConnectionStatus(CONNECTION_TO_USER Status);
		void 			setOnlineState(const ONLINE_STATE newState);
		void			setName(QString newName);
		void 			setI2PStreamID(qint32 ID);
		void 			setProtocolVersion(QString Version);
		void 			setClientName(QString Name);
		void 			setClientVersion(QString Version);
		void 			setTextColor(QColor textColor);
		void 			setTextFont(QFont textFont);
		void 			setInvisible(bool b);
		void 			setMaxProtocolVersionFiletransfer(QString Version){mMaxProtocolVersionFiletransfer=Version;};
		void 			setMinProtocolVersionFiletransfer(QString Version){mMinProtocolVersionFiletransfer=Version;};
		void			setRecivedUserInfos(USER_INFO_FIELDS Tag,QString value);
		void			setRecivedNicknameToUserNickname();
		void			setReplaceB32WithB64(QString b64Dest);
		void			setAvatarImage(QByteArray& avatarImage);
		void			setUnsentedMessages(QStringList& newMessages);

	
	public slots:
		void  			slotSendChatMessage(QString Message);
        void            transferMessageToSwarmWhiteListUser(SwarmType1LocalImage& swarm, QString& message);
		void 			slotIncomingNewChatMessage(QString newMessage);
		void 			slotIncomingMessageFromSystem(QString newMessage,bool indicateWithSoundAndIcon=false);

	signals:
		void 			signOnlineStateChanged();
		void 			signNewMessageRecived();
		void 			signNewMessageSound();
		void 			signConnectionOnline();
		void 			signConnectionOffline();
		void			signUserDeleted();
		void			signNewAvatarImage();
		void			signSaveUnsentMessages(QString I2PDest);
		
	private:
		AppContext&			mCore;
		Connector& 		mProtocol;
		QString 		mName;
	const 	QString 		mI2PDestination;
		qint32 			mI2PStream_ID;	
		
		bool			mInvisible;
		bool			mRecivedNicknameToUserNickname;
		bool			mHaveNewUnreadMessages;
		bool			mHaveNewUnreadChatmessage;
		bool			mLogOnlineStateOfUsers;
		CONNECTION_TO_USER 	mConnectionStatus;
		ONLINE_STATE	 	mCurrentOnlineState;
		QString 		mProtocolVersion;
		QString			mMaxProtocolVersionFiletransfer;
		QString			mMinProtocolVersionFiletransfer;
		QString 		mClientName;
		QString 		mClientVersion;
		QStringList 		mAllMessages;
		QStringList		mNewMessages;
        QMap<SwarmType1LocalIdType,QStringList> swarmLocalId2allMessages;
        QMap<SwarmType1LocalIdType,QStringList> swarmLocalId2newMessages;
        QStringList 		mUnsentedMessages;
		
		UserInfo		mRecivedUserInfos;
		bool			mUseB32Dest;

	//<Settings for the chatwindow>
		QColor 			mTextColor;
		QFont  			mTextFont;
	  	ChatMessageChangerUtil&	mChatMessageChangerUtil;
	//</Settings for the chatwindow>
		void 			SendAllunsendedMessages();
};
#endif //of I2P_CHAT_USER_IMPL_H
