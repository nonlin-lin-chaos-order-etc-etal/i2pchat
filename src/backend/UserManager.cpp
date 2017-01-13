/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "UserManager.h"

#include "Protocol.h"
#include "UserBlockManager.h"
#include "Core.h"


CUserManager::CUserManager(CCore& Core, QString UserFileWithPath,CUnsentChatMessageStorage& UnsentChatMessageStorage)
:
  mCore(Core),mUserFileWithPath(UserFileWithPath),mUnsentMessageStorage(UnsentChatMessageStorage),rosterModel(nullptr)
{
  
}

CUserManager::~CUserManager()
{
     
      for(int i=0;i<this->mUsers.count();i++){
	    //delete mUsers.at(i);
	    mUsers.at(i)->deleteLater();
      }
      
      mUsers.clear();
      if(rosterModel)rosterModel->invalidateContents();
}

void CUserManager::loadUserList(){
    if(rosterModel)rosterModel->load();
}

void CUserManager::saveUserList()const{
    if(rosterModel)rosterModel->save();
}

CUser* CUserManager::getUserByI2P_ID(qint32 ID)const{

	for(int i=0;i<mUsers.size();i++){
		if(mUsers.at(i)->getI2PStreamID()==ID){
			
			return mUsers.at(i);
		}
	}
	
	return NULL;
}
CUser* CUserManager::getUserByI2P_Destination(QString Destination)const{
	for(int i=0;i<mUsers.size();i++){
		if(mUsers.at(i)->getI2PDestination()==Destination){
			return mUsers.at(i);
		}
	}
	
	return NULL;
}


QString CUserManager::getUserInfosByI2P_Destination(QString Destination)const
{
	QString Infos="No Informations recived";

	for(int i=0;i<mUsers.size();i++){
		if(mUsers.at(i)->getI2PDestination()==Destination){
			CUser* theUser=mUsers.at(i);

			Infos= "Nickname:\t\t"  		+theUser->getName()+"\n";
			Infos+="Clientname:\t\t"		+theUser->getClientName()+"\n";
			Infos+="Clientversion:\t\t"		+theUser->getClientVersion()+"\n";
			Infos+="Prot. version:\t\t"		+theUser->getProtocolVersion()+"\n";
			Infos+="Prot. version min (Filetransfer):\t"+theUser->getMinProtocolVersionFiletransfer()+"\n";
			Infos+="Prot. version max (Filetransfer):\t"+theUser->getMaxProtocolVersionFiletransfer()+"\n";
			
			if(theUser->getProtocolVersion_D()>=0.3){
				CRecivedInfos recivedInfos=theUser->getRecivedUserInfos();
				QString sAge;
				sAge.setNum(recivedInfos.Age,10);

				Infos+="\nRecived Userinfos:\n";
				Infos+="Nickname:\t\t"		+recivedInfos.Nickname+"\n";
				Infos+="Gender:\t\t"		+recivedInfos.Gender+"\n";
				Infos+="Age:\t\t"		+sAge+"\n";
				Infos+="Interests:\t\t"		+recivedInfos.Interests;
			}
		}
	}
	return Infos;
}

const QList<CUser*> CUserManager::getUserList()const{
	return mUsers;
}

bool CUserManager::validateI2PDestination(const QString I2PDestination) const
{
    auto validateB64 = [] (QString Dest) {
      if(Dest.length() == 516 && Dest.right(4).contains("AAAA",Qt::CaseInsensitive))
	return true;
      else
	return false;
    };
  
    auto validateB32 = [] (QString Dest) {
      if(Dest.length() == 60 && Dest.right(8).contains(".b32.i2p",Qt::CaseInsensitive))
	return true;
      else
	return false;
    };
    
    auto validateECDSA_SHA256_P256 = [] (QString Dest) {
      if(Dest.length() == 524 && Dest.right(10).contains("AEAAEAAA==",Qt::CaseInsensitive))
	return true;
      else
	return false;
    };
    
    auto validateECDSA_SHA384_P384 = [] (QString Dest) {
      if(Dest.length() == 524 && Dest.right(10).contains("AEAAIAAA==",Qt::CaseInsensitive))
	return true;
      else
	return false;
    };
    
    auto validateECDSA_SHA512_P512 = [] (QString Dest) {
      if(Dest.length() == 528 && Dest.right(1).contains("=",Qt::CaseInsensitive) && 
	Dest.mid(512,9).contains("BQAIAAMAA",Qt::CaseInsensitive) 
      )
	return true;
      else
	return false;
    };
    
    
    if(I2PDestination.right(4).contains("AAAA",Qt::CaseInsensitive))		{return validateB64(I2PDestination);}
    else if(I2PDestination.right(8).contains(".b32.i2p",Qt::CaseInsensitive))	{return validateB32(I2PDestination);}
    else if(I2PDestination.right(10).contains("AEAAEAAA==",Qt::CaseInsensitive)){return validateECDSA_SHA256_P256(I2PDestination);}
    else if(I2PDestination.right(10).contains("AEAAIAAA==",Qt::CaseInsensitive)){return validateECDSA_SHA384_P384(I2PDestination);}
    else if(I2PDestination.length()==528 && I2PDestination.mid(512,9).contains("BQAIAAMAA",Qt::CaseInsensitive)) 
      {return validateECDSA_SHA512_P512(I2PDestination);}
    else
      return false;
}

bool CUserManager::addNewUser(QString Name,QString I2PDestination,qint32 I2PStream_ID,bool SaveUserList,
                              CUser**resultingNewUser, bool /*isSwarmUser*/){
    if(resultingNewUser){
        (*resultingNewUser)=nullptr;
    }
	CUserBlockManager& UserBlockManager=*(mCore.getUserBlockManager());
	CProtocol& Protocol=*(mCore.getProtocol());
	
	bool isValid=validateI2PDestination(I2PDestination);
	
	if(isValid==false){
		qCritical()<<"File\t"<<__FILE__<<endl
			   <<"Line:\t"<<__LINE__<<endl
			   <<"Function:\t"<<"CUserManager::addNewUser"<<endl
			   <<"Message:\t"<<"Destination is not valid"<<endl
			   <<"Destination:\t"<<I2PDestination<<endl
			   <<"action add new User ignored"<<endl;
			   
		return false;	   
	}
      
    
	if(UserBlockManager.isDestinationInBlockList(I2PDestination)==true){
		qCritical()<<"File\t"<<__FILE__<<endl
			   <<"Line:\t"<<__LINE__<<endl
			   <<"Function:\t"<<"CUserManager::addNewUser"<<endl
			   <<"Message:\t"<<"The Destination: "<<I2PDestination<<endl
			   <<"is on the blocklist"<<endl
			   <<"action add new User ignored"<<endl;
			   
		return false;
	}
	
	if(this->checkIfUserExitsByI2PDestination(I2PDestination)==true){
		qCritical()<<"File\t"<<__FILE__<<endl
			   <<"Line:\t"<<__LINE__<<endl
			   <<"Function:\t"<<"CUserManager::addNewUser"<<endl
			   <<"Message:\t"<<"there is allready a user with this destination"<<endl
			   <<"action add new User ignored"<<endl;

		return false;
	}

	if(I2PDestination==mCore.getMyDestination()){
		    	qCritical()<<"File\t"<<__FILE__<<endl
				   <<"Line:\t"<<__LINE__<<endl
				   <<"Function:\t"<<"CUserManager::addNewUser"<<endl
				   <<"Message:\t"<<"It's not allowed to add yourself"<<endl
				   <<"action add new User ignored"<<endl;

		return false;
	}
	

	//add newuser
	CSoundManager& SoundManager=*(mCore.getSoundManager());
	CUser* newuser=new CUser(mCore, Protocol,Name,I2PDestination,I2PStream_ID);

    if(resultingNewUser) {
        (*resultingNewUser)=newuser;
    }
	connect(newuser,SIGNAL(signNewMessageSound()),&SoundManager,
		SLOT(slotNewChatMessage()));

	connect(newuser,SIGNAL(signConnectionOnline()),&SoundManager,
		SLOT(slotUserGoOnline()));

	connect(newuser,SIGNAL(signConnectionOffline()),&SoundManager,
		SLOT(slotUserGoOffline()));

	connect(newuser,SIGNAL(signOnlineStateChanged()),this,
		SIGNAL(signUserStatusChanged()));
	
	connect(newuser,SIGNAL(signSaveUnsentMessages(QString)),this,
		SLOT(slotSaveUnsentMessageForDest(QString)));


    this->mUsers.append(newuser);
	if(SaveUserList==true){
		saveUserList();
	}

	if(mCore.getOnlineStatus()!=USEROFFLINE && mCore.getOnlineStatus()!=USERTRYTOCONNECT){
		if(I2PStream_ID==0){
			mCore.createStreamObjectForUser(*newuser);
		}
	}
	emit signUserStatusChanged();
	return true;
}

bool CUserManager::checkIfUserExitsByI2PDestination(const QString I2PDestination)const{
	if(I2PDestination==mCore.getMyDestination()) return true;

	for(int i=0;i<mUsers.count();i++){
		if(mUsers.at(i)->getI2PDestination()==I2PDestination){
			return true;
		}
	}
	
	return false;
}
void CUserManager::changeUserPositionInUserList(int oldPos, int newPos)
{
	mUsers.swap(oldPos,newPos);
	saveUserList();
	emit signUserStatusChanged();
}

bool CUserManager::deleteUserByI2PDestination(QString I2PDestination){
	for(int i=0;i<mUsers.count();i++){
		if(mUsers.at(i)->getI2PDestination()==I2PDestination){
			if(mUsers.at(i)->getConnectionStatus()==ONLINE ||mUsers.at(i)->getConnectionStatus()==TRYTOCONNECT){
				mCore.deletePacketManagerByID(mUsers.at(i)->getI2PStreamID());
				mCore.getConnectionManager()->doDestroyStreamObjectByID(mUsers.at(i)->getI2PStreamID());
			}
			
			if(mCore.getConnectionManager()->isComponentStopped()==false){
				mCore.getConnectionManager()->doDestroyStreamObjectByID(mUsers.at(i)->getI2PStreamID());
			}
            mCore.getRosterController().removeActorByI2PDestination(I2PDestination);
			mUsers.at(i)->deleteLater();
			mUsers.removeAt(i);
			saveUserList();
			emit signUserStatusChanged();
			return true;
		}
	}
	return false;
}

bool CUserManager::renameUserByI2PDestination(const QString Destination, const QString newNickname){
	for(int i=0;i<mUsers.size();i++){
		if(mUsers.at(i)->getI2PDestination()==Destination){
			mUsers.at(i)->setName(newNickname);
			saveUserList();
			emit signUserStatusChanged();
			return true;
		}
	}
	return false;
}

void CUserManager::avatarImageChanged()
{
	for(int i=0;i<mUsers.count();i++){
	  CUser* User=mUsers.at(i);
	  
	  if(User->getOnlineState()!= USEROFFLINE &&  
	     User->getOnlineState()!= USERTRYTOCONNECT &&
	     User->getOnlineState()!= USERBLOCKEDYOU &&
	     User->getProtocolVersion_D()>=0.6){
		CProtocol& Protocol=*(mCore.getProtocol());
        bool dataWasTruncatedTo0xffffMinus4Bool;
        Protocol.send(AVATARIMAGE_CHANGED,mUsers.at(i)->getI2PStreamID(),QString(),dataWasTruncatedTo0xffffMinus4Bool);
	     }	  
	}
}

void CUserManager::slotSaveUnsentMessageForDest(QString I2PDest)
{
  CUser* theUser=getUserByI2P_Destination(I2PDest);
  if(theUser!=NULL){	
    const QStringList Messages=theUser->getUnsentedMessages();
    mUnsentMessageStorage.saveChatMessagesForDest(I2PDest,Messages);
  }
  else{
    qWarning()  <<"File\t"<<__FILE__<<endl
	        <<"Line:\t"<<__LINE__<<endl
	        <<"Function:\t"<<"CUserManager::slotSaveUnsentMessageForDest"<<endl
		<<"Message:\t"<<"No User found with this dest"<<endl
		<<"I2PDest.:\t"<<I2PDest<<endl;
  }
}

void CUserManager::setRosterModel(RosterModel* rosterModel_) {
    rosterModel = rosterModel_;
}
