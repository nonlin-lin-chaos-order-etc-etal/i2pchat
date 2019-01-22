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
#include "AbstractUserLocalImage.h"

#include "AppContext.h"
#include "ChatMessageChangerUtilUtil.h"
#include "Connector.h"
#include "FileTransferSend.h"
#include "UserManager.h"

AbstractUserLocalImage::AbstractUserLocalImage(	AppContext&	appCtx,
		Connector& Protocol,
		QString Name,
		QString I2PDestination,
		qint32 I2PStream_ID
		):mCore(appCtx),mProtocol(Protocol),mI2PDestination(I2PDestination),mChatMessageChangerUtil(*(ChatMessageChangerUtil::exemplar(appCtx)))		
{
	QSettings settings(mCore.getConfigPath()+"/application.ini",QSettings::IniFormat);

	this->mName=Name;
	this->mI2PStream_ID=I2PStream_ID;
	this->mConnectionStatus=OFFLINE;
	this->mClientName="";
	this->mClientVersion="";
	this->mCurrentOnlineState=USEROFFLINE;
	this->mHaveNewUnreadMessages=false;
	this->mHaveNewUnreadChatmessage=false;

	settings.beginGroup("Chat");
		this->mTextFont.fromString(settings.value("DefaultFont","Comic Sans MS,10").toString());
		this->mTextColor.setNamedColor(settings.value("DefaultColor","#000000").toString());
		this->mLogOnlineStateOfUsers=(settings.value("LogOnlineStatesOfUsers",true).toBool());
	settings.endGroup();
	settings.sync();
	this->mInvisible=false;
	this->mRecivedNicknameToUserNickname=false;
	this->mProtocolVersion="0.2";
	this->mMaxProtocolVersionFiletransfer="0.1";
	this->mMinProtocolVersionFiletransfer="0.1";
	this->mRecivedUserInfos.Age=0;
	
	if(mI2PDestination.length()==60){
	    mUseB32Dest=true;
	}else{
	    mUseB32Dest=false;
	}
}
AbstractUserLocalImage::~AbstractUserLocalImage()
{	

    emit signUserDeleted();
}

void AbstractUserLocalImage::setName(QString newName){
	this->mName=newName;
	mCore.getUserManager()->saveUserList();
}
void AbstractUserLocalImage::setConnectionStatus(CONNECTION_TO_USER Status){
	if(mConnectionStatus==Status){return;}  
  
	mConnectionStatus=Status;
	
	if(Status==ONLINE){
		if(getUsedB32Dest()==true){
		    mCore.doNamingLookUP(mI2PDestination);
		}
		//get some Infos from the CHATSYSTEM - client
		mProtocol.send(GET_CLIENTNAME		,mI2PStream_ID);
		mProtocol.send(GET_CLIENTVERSION	,mI2PStream_ID);
		mProtocol.send(GET_USER_ONLINESTATUS	,mI2PStream_ID);
		

		if(getProtocolVersion_D()>=0.3){
			mProtocol.send(GET_MAX_PROTOCOLVERSION_FILETRANSFER,mI2PStream_ID);
			mProtocol.send(GET_USER_INFOS,mI2PStream_ID);
		}
		if(getProtocolVersion_D()>=0.4){
			mProtocol.send(GET_MIN_PROTOCOLVERSION_FILETRANSFER,mI2PStream_ID);
		}
		if(getProtocolVersion_D()>=0.5){
			if(mRecivedUserInfos.AvatarImage.isEmpty()==true){
			  mProtocol.send(GET_AVATARIMAGE,mI2PStream_ID);
			}
		}
	}

    if(Status==OFFLINE ||Status==CONNECTION_ERROR){
		mI2PStream_ID=0;
		
		if(mCurrentOnlineState!=USERBLOCKEDYOU){
		    setOnlineState(USEROFFLINE);
		}
	}
	emit signOnlineStateChanged();
}

void AbstractUserLocalImage::setI2PStreamID(qint32 ID){
	this->mI2PStream_ID=ID;
}

void AbstractUserLocalImage::setProtocolVersion(QString Version){
	this->mProtocolVersion=Version;
}

/*
void AbstractUserLocalImage::slotIncomingNewChatMessageAtSwarm(QString newMessage,SwarmType1LocalIdType swarmRemoteId){
    newMessage=mChatMessageChangerUtil.changeChatMessage(newMessage);

    //TODO fix this in OOP way
    this->mAllMessages.push_back(mName+" ( "+ QTime::currentTime().toString("hh:mm:ss") +" ): "+newMessage+"<br>");
    this->mNewMessages.push_back(mName+" ( "+ QTime::currentTime().toString("hh:mm:ss") +" ): "+newMessage+"<br>");

    mHaveNewUnreadMessages=true;
    mHaveNewUnreadChatmessage=true;

    emit signNewMessageRecived();
    emit signNewMessageSound();
    emit signOnlineStateChanged();
}
*/

void AbstractUserLocalImage::slotIncomingNewChatMessage(QString newMessage){
	newMessage=mChatMessageChangerUtil.changeChatMessage(newMessage);

    //TODO fix this in OOP way
	this->mAllMessages.push_back(mName+" ( "+ QTime::currentTime().toString("hh:mm:ss") +" ): "+newMessage+"<br>");
	this->mNewMessages.push_back(mName+" ( "+ QTime::currentTime().toString("hh:mm:ss") +" ): "+newMessage+"<br>");
	
	mHaveNewUnreadMessages=true;
	mHaveNewUnreadChatmessage=true;

	emit signNewMessageRecived();
	emit signNewMessageSound();
	emit signOnlineStateChanged();
}

void AbstractUserLocalImage::slotSendChatMessage(QString Message){
	using namespace PROTOCOL_TAGS;
	QString Nickname;

	if(mConnectionStatus==ONLINE && 
		mCurrentOnlineState != USEROFFLINE &&
		mCurrentOnlineState != USERINVISIBLE
	){
		QByteArray ByteMessage=Message.toUtf8();
        bool dataWasTruncatedTo0xffffMinus4Bool;
        mProtocol.send(CHATMESSAGE,mI2PStream_ID,ByteMessage,dataWasTruncatedTo0xffffMinus4Bool);
        if(dataWasTruncatedTo0xffffMinus4Bool){
            QMessageBox::warning(0, QObject::tr("Warning: Data was truncated"),QObject::tr("Data was truncated to 0xffff-4 bytes."));//FIXME
        }
		
		if(mCore.getUserInfos().Nickname.isEmpty()==true){
          Nickname=tr("Me");
		}else{
		  Nickname=mCore.getUserInfos().Nickname;
		}
		  
		this->mAllMessages.push_back(Nickname+" ("+QTime::currentTime().toString("hh:mm:ss")  +"): "+Message+"<br>");
		this->mNewMessages.push_back(Nickname+" ("+QTime::currentTime().toString("hh:mm:ss")  +"): "+Message+"<br>");
		
		mHaveNewUnreadMessages=true;
		emit signNewMessageRecived();
	}
	else{
	  	mUnsentedMessages.push_back(Message+"<br>");
		slotIncomingMessageFromSystem(tr("Sending the message when the user comes online.<br>If you close the client, the message will be lost."));
	}
}


const QStringList& AbstractUserLocalImage::getAllChatMessages()
{
	mNewMessages.clear();
	mHaveNewUnreadMessages=false;
	mHaveNewUnreadChatmessage=false;

	emit signOnlineStateChanged();
	return mAllMessages;
}
void AbstractUserLocalImage::SendAllunsendedMessages()
{ 
	using namespace PROTOCOL_TAGS;
	if(mUnsentedMessages.empty())return;


    for(int i=0;i<mUnsentedMessages.count();i++){
        bool dataWasTruncatedTo0xffffMinus4Bool;
        mProtocol.send(CHATMESSAGE,mI2PStream_ID,mUnsentedMessages.at(i),dataWasTruncatedTo0xffffMinus4Bool);
        if(dataWasTruncatedTo0xffffMinus4Bool){
            QMessageBox::warning(0, QObject::tr("Warning: Data was truncated"),QObject::tr("Data was truncated to 0xffff-4 bytes."));//FIXME
        }
    }
	
	mUnsentedMessages.clear();
	slotIncomingMessageFromSystem("All previously unsent messages have been sent.",true);
}


void AbstractUserLocalImage::setClientName(QString Name)
{
	mClientName=Name;
}

void AbstractUserLocalImage::setClientVersion(QString Version)
{
	this->mClientVersion=Version;
	if(mClientName=="I2P-Messenger (QT)" && mClientVersion=="0.2.15 Beta"){
	    setMaxProtocolVersionFiletransfer("0.2");
	}
}

void AbstractUserLocalImage::setOnlineState(const ONLINE_STATE newState)
{
	if(mCurrentOnlineState==newState)return;
  
	if(newState!=USEROFFLINE && 
	   newState!=USERINVISIBLE &&
	   newState!=USERBLOCKEDYOU){		
		if(mCurrentOnlineState==USEROFFLINE || mCurrentOnlineState==USERINVISIBLE ||  mCurrentOnlineState==USERBLOCKEDYOU){
			if(mLogOnlineStateOfUsers==true){
			  slotIncomingMessageFromSystem(tr("%1 is online").arg(mName));
			}
			emit signConnectionOnline();
			
		}
		this->SendAllunsendedMessages();
	}
	else if(newState==USEROFFLINE || newState==USERINVISIBLE || newState==USERBLOCKEDYOU){
		if(newState!=mCurrentOnlineState){
			if(mLogOnlineStateOfUsers==true){
			  slotIncomingMessageFromSystem(tr("%1 is offline").arg(mName));
			}
			emit signConnectionOffline();
		}
	}

	this->mCurrentOnlineState=newState;
	emit signOnlineStateChanged();
}

void AbstractUserLocalImage::setTextColor(QColor textColor)
{
	this->mTextColor=textColor;
}

void AbstractUserLocalImage::setTextFont(QFont textFont)
{
	this->mTextFont=textFont;
}


void AbstractUserLocalImage::slotIncomingMessageFromSystem(QString newMessage,bool indicateWithSoundAndIcon)
{
	this->mAllMessages.push_back(tr("[System] ")+"( "+ QTime::currentTime().toString("hh:mm:ss") +" ): "+newMessage+"<br><br>");	
	this->mNewMessages.push_back(tr("[System] ")+"( "+ QTime::currentTime().toString("hh:mm:ss") +" ): "+newMessage+"<br><br>");	
	
	
	mHaveNewUnreadMessages=true;	

	emit signNewMessageRecived();
	
	if(indicateWithSoundAndIcon==true){
	    emit signNewMessageSound();
	    mHaveNewUnreadChatmessage=true;
	    emit signNewMessageRecived();
	}
	
	emit signOnlineStateChanged();
}

void AbstractUserLocalImage::setInvisible(bool b)
{
	mInvisible=b;
	if(mConnectionStatus==ONLINE){
		QByteArray Data("1003");//GET_USER_ONLINESTATUS = send the new OnlineStatus
		mProtocol.slotInputKnown(mI2PStream_ID,Data);
	}
	emit signOnlineStateChanged();
}

const QStringList AbstractUserLocalImage::getNewMessages(bool haveFocus)
{
	QStringList tmp(mNewMessages);
	mNewMessages.clear();
	
	if(haveFocus==true){
		mHaveNewUnreadMessages=false;
		mHaveNewUnreadChatmessage=false;
		emit signOnlineStateChanged();
	}

	return tmp;
}

double AbstractUserLocalImage::getProtocolVersion_D() const
{
	bool OK=false;
	bool tmp=mProtocolVersion.toDouble(&OK);

	if(OK==false){
		qCritical()<<"File\t"<<__FILE__<<endl
			   <<"Line:\t"<<__LINE__<<endl
			   <<"Function:\t"<<"CUser::getProtocolVersion_D"<<endl
			   <<"Message:\t"<<"Can't convert QString to double"<<endl
			   <<"QString:\t"<<mProtocolVersion<<endl;
	}
	return tmp;
}

double AbstractUserLocalImage::getMaxProtocolVersionFiletransfer_D() const
{
	bool OK=false;
	double tmp=mMaxProtocolVersionFiletransfer.toDouble(&OK);

	if(OK==false){
		qCritical()<<"File\t"<<__FILE__<<endl
			   <<"Line:\t"<<__LINE__<<endl
			   <<"Function:\t"<<"CUser::getMaxProtocolVersionFiletransfer_D"<<endl
			   <<"Message\t"<<"Can't convert QString to double"<<endl
			   <<"QString:\t"<<mMaxProtocolVersionFiletransfer<<endl;
	}
	return tmp;
}

double AbstractUserLocalImage::getMinProtocolVersionFiletransfer_D() const
{
	bool OK=false;
	double tmp=mMinProtocolVersionFiletransfer.toDouble(&OK);

	if(OK==false){
		qCritical()<<"File\t"<<__FILE__<<endl
			   <<"Line:\t"<<__LINE__<<endl
			   <<"Function:\t"<<"CUser::getMinProtocolVersionFiletransfer_D"<<endl
			   <<"Message:\t"<<"Can't convert QString to double"<<endl
			   <<"QString:\t"<<mMinProtocolVersionFiletransfer<<endl;
	}
	return tmp;
}

void AbstractUserLocalImage::setRecivedUserInfos(USER_INFO_FIELDS Tag, QString value)
{
	switch (Tag){
		case NICKNAME:
		{
			mRecivedUserInfos.Nickname=value;
			if(mRecivedNicknameToUserNickname==true){
				if(value.isEmpty()==true){
					setName(tr("No Nickname"));	
				}
				else{
					setName(value);
				}
				mRecivedNicknameToUserNickname=false;
				emit signOnlineStateChanged();
			}
			break;
		}
		case GENDER:
		{
			mRecivedUserInfos.Gender=value;
			break;
		}
		case AGE:
		{
			bool OK=false;
			mRecivedUserInfos.Age=value.toInt(&OK);
			if(OK==false){
				qCritical()<<"File\t"<<__FILE__<<endl
					   <<"Line:\t"<<__LINE__<<endl
					   <<"Function:\t"<<"Can't convert QString to qint32"<<endl
					   <<"QString:\t"<<value<<endl;
			}
			break;
		}
		case INTERESTS:
		{
			mRecivedUserInfos.Interests=value;
			break;
		}
		default:
		{
			qWarning()<<"File\t"<<__FILE__<<endl
				  <<"Line:\t"<<__LINE__<<endl
				  <<"Function:\t"<<"CUser::setRecivedUserInfos"<<endl
				  <<"Message:\t"<<"unknown Tag"<<endl;
			break;
		}
	}
}

void AbstractUserLocalImage::setRecivedNicknameToUserNickname()
{
	if(getProtocolVersion_D()>=0.3){
		mRecivedNicknameToUserNickname=true;
	}
	else{
		qWarning()<<"File\t"<<__FILE__<<endl
			  <<"Line:\t"<<__LINE__<<endl
			  <<"Function:\t"<<"setRecivedNicknameToUserNickname"<<endl
			  <<"Message:\t"<<"Protocolversion <0.3, action ignored"<<endl;
	}
}

const QString AbstractUserLocalImage::getHighestUsableProtocolVersionFiletransfer() const
{
      return QString::number(getHighestUsableProtocolVersionFiletransfer_D(),'g',2);
}

double AbstractUserLocalImage::getHighestUsableProtocolVersionFiletransfer_D() const 
{
      double maxVersion=getMaxProtocolVersionFiletransfer_D();
      
      while(maxVersion> FileTransferProtocol::MAXPROTOCOLVERSION_D){
	maxVersion-=0.1;
      }
      return maxVersion;
}
void AbstractUserLocalImage::setReplaceB32WithB64(QString b64Dest)
{
    if(mUseB32Dest==true){
	QString& dest = const_cast< QString & >(mI2PDestination);
	dest=b64Dest;
    }else{
	qCritical()<<"File\t"<<__FILE__<<endl
		   <<"Line:\t"<<__LINE__<<endl
		   <<"Function:\t"<<"CUser::setReplaceB32WithB64"<<endl
		   <<"Message:\t"<<"Current Destination is not a b32 dest"<<endl;
    }
}

void AbstractUserLocalImage::setAvatarImage(QByteArray& avatarImage)
{
    mRecivedUserInfos.AvatarImage.clear();
  
    QPixmap tmpPixmap;
    tmpPixmap.loadFromData(avatarImage);
    tmpPixmap=tmpPixmap.scaled(90,90,Qt::KeepAspectRatio);
	
	  
    QBuffer buffer(&mRecivedUserInfos.AvatarImage);
    buffer.open(QIODevice::WriteOnly);
    tmpPixmap.save(&buffer, "PNG");
    
    emit signNewAvatarImage();
}


void AbstractUserLocalImage::setUnsentedMessages(QStringList& newMessages)
{
  mUnsentedMessages=newMessages;
  emit signSaveUnsentMessages(mI2PDestination);
}
