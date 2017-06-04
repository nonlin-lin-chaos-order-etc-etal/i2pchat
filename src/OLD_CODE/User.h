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
#ifndef I2P_CHAT_USER_H
#define I2P_CHAT_USER_H

#include <memory>

class User  : public std::enable_shared_from_this<User>  {
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
					User(){};
					virtual ~User(){};

					//forbid some operators
					User(const User&)=delete;
					User& operator=(const User&)=delete;
	
    virtual const 	QString 		getName()=0;
    virtual const 	QString 		getI2PDestination()=0;
    virtual       	qint32  		getI2PStreamID()=0;
    virtual const 	QString 		getProtocolVersion()=0;
    virtual const 	QString 		getClientName()=0;
    virtual const	QString 		getClientVersion()=0;
    virtual CONNECTION_TO_USER 		getConnectionStatus()=0;
    virtual ONLINE_STATE 	 		getOnlineState()=0;
    virtual const	UserInfo		getUserInfo()=0;

    virtual const	QString			getHighestUsableProtocolVersionFiletransfer()=0;
    virtual const	QString			getMaxProtocolVersionFiletransfer()=0;
    virtual const	QString			getMinProtocolVersionFiletransfer()=0;
    virtual bool			getIsInvisible()=0;
    virtual bool			getUsedB32Dest()=0;

    virtual void 			setConnectionStatus(CONNECTION_TO_USER Status)=0;
    virtual void 			setOnlineState(const ONLINE_STATE newState)=0;
    virtual void			setName(QString newName)=0;
    virtual void 			setI2PStreamID(qint32 ID)=0;
    virtual void 			setProtocolVersion(QString Version)=0;
    virtual void 			setClientName(QString Name)=0;
    virtual void 			setClientVersion(QString Version)=0;
    virtual void 			setInvisible(bool b)=0;
    virtual void 			setMaxProtocolVersionFiletransfer(QString Version){mMaxProtocolVersionFiletransfer=Version;};
    virtual void 			setMinProtocolVersionFiletransfer(QString Version){mMinProtocolVersionFiletransfer=Version;};
    virtual void			setUserInfoTaggedValue(USER_INFO_FIELDS Tag,QString value)=0;
    virtual void			setRecivedNicknameToUserNickname();
    virtual void			setReplaceB32WithB64(QString b64Dest);
    virtual void			setAvatarImage(QByteArray& avatarImage);
};
#endif //of ifdef I2P_CHAT_USER_H
