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
#include <QSettings>
#include <QErrorMessage>

#include "AbstractUserLocalImage.h"
#include "I2PStream.h"
#include "FileTransferSend.h"
#include "UserManager.h"

#include <QtNumeric>

#include <iostream>

#include "AppContext.h"
#include "Connector.h"
using namespace std;

Connector::Connector(AppContext& appCtx)
:mCore(appCtx)
{
}

void Connector::newConnectionChat(const qint32 ID){
	using namespace Protocol_Info;
	CI2PStream* stream=mCore.getI2PStreamObjectByID(ID);

	//send the ChatSystem\tProtocolVersion
	if(stream->getFIRSTPAKETCHAT_allreadySended()==false){
		//sometime StreamStatusRecived is called again with streamstatus Ok
		stream->setFIRSTPAKETCHAT_allreadySended(true);
		*(stream)<<(QString)FIRSTPAKETCHAT;
	}
}

void Connector::slotInputKnown(const qint32 ID, const QByteArray Data){
using namespace Protocol_Info;
	
	if(Data.length()<4) 
		return;

	QString ProtocolInfoTag(Data.left(4));
	
		//COMMANDS
			if(ProtocolInfoTag=="1000"){//PING:
                bool dataWasTruncatedTo0xffffMinus4Bool;
                send(ECHO_OF_PING,ID,QString(""),dataWasTruncatedTo0xffffMinus4Bool);
			}
			else if(ProtocolInfoTag=="1001"){//GET_CLIENTVERSION:
                bool dataWasTruncatedTo0xffffMinus4Bool;
                send(ANSWER_OF_GET_CLIENTVERSION,ID,mCore.getClientVersion(),dataWasTruncatedTo0xffffMinus4Bool);
			}
			else if(ProtocolInfoTag=="1002"){//GET_CLIENTNAME:
                bool dataWasTruncatedTo0xffffMinus4Bool;
                send(ANSWER_OF_GET_CLIENTNAME,ID,mCore.getClientName(),dataWasTruncatedTo0xffffMinus4Bool);
			}
			else if(ProtocolInfoTag=="1003"){//GET_USER_ONLINESTATUS:
                bool dataWasTruncatedTo0xffffMinus4Bool;
                if(mCore.getUserManager()->getUserByI2P_ID(ID)->getIsInvisible()==true){
                    send(USER_ONLINESTATUS_OFFLINE,ID,QString(""),dataWasTruncatedTo0xffffMinus4Bool);
				}
				else{
                    bool dataWasTruncatedTo0xffffMinus4Bool;
					switch(mCore.getOnlineStatus())
					{
						case USERONLINE:
                        {
                            send(USER_ONLINESTATUS_ONLINE,ID,QString(""),dataWasTruncatedTo0xffffMinus4Bool);
							break;
						}
						case USEROFFLINE:
						case USERINVISIBLE:
						{
                            send(USER_ONLINESTATUS_OFFLINE,ID,QString(""),dataWasTruncatedTo0xffffMinus4Bool);
							break;
						}
						case USERAWAY:
						{
                            send(USER_ONLINESTATUS_AWAY,ID,QString(""),dataWasTruncatedTo0xffffMinus4Bool);
							break;
							
						}
						case USERWANTTOCHAT:
						{
                            send(USER_ONLINESTATUS_WANTTOCHAT,ID,QString(""),dataWasTruncatedTo0xffffMinus4Bool);
							break;
						}
						case USERDONT_DISTURB:
						{
                            send(USER_ONLINESTATUS_DONT_DISTURB,ID,QString(""),dataWasTruncatedTo0xffffMinus4Bool);
							break;
						}
						default:
						{
							QMessageBox* msgBox= new QMessageBox(NULL);
							msgBox->setIcon(QMessageBox::Warning);
							msgBox->setText(tr("cProtocol(inputKnown)"));
							msgBox->setInformativeText(tr("Unknown USERSTATE"));
							msgBox->setStandardButtons(QMessageBox::Ok);
							msgBox->setDefaultButton(QMessageBox::Ok);
							msgBox->setWindowModality(Qt::NonModal);
							msgBox->show();
	
						}
					}//switch
				}//else

			}
			else if(ProtocolInfoTag=="1004"){//GET_PROTOCOLVERSION,
                bool dataWasTruncatedTo0xffffMinus4Bool;
                send(ANSWER_OF_GET_PROTOCOLVERSION,ID,mCore.getProtocolVersion(),dataWasTruncatedTo0xffffMinus4Bool);
			}
			else if(ProtocolInfoTag=="1005"){//GET_MAX_PROTOCOLVERSION_FILETRANSFER
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				
				if(thisUser!=NULL){
                    bool dataWasTruncatedTo0xffffMinus4Bool;
				     if(thisUser->getProtocolVersion()=="0.3"){
					  //BUG in Messenger_0.2.15 BETA :(
                      send(ANSWER_OF_GET_MAX_PROTOCOLVERSION_FILETRANSFER,ID,QString("0.2"),dataWasTruncatedTo0xffffMinus4Bool);
				      }else{
                      send(ANSWER_OF_GET_MAX_PROTOCOLVERSION_FILETRANSFER,ID,FileTransferProtocol::MAXPROTOCOLVERSION,dataWasTruncatedTo0xffffMinus4Bool);
				      }
				}
			}
			else if(ProtocolInfoTag=="1006"){//GET_USER_INFOS
                bool dataWasTruncatedTo0xffffMinus4Bool;
				using namespace AbstractUserLocalImage;
				QString sAge;
				UserInfo Infos= mCore.getUserInfos();
                send(USER_INFO_NICKNAME	,ID,Infos.Nickname,dataWasTruncatedTo0xffffMinus4Bool);
                send(USER_INFO_GENDER	,ID,Infos.Gender,dataWasTruncatedTo0xffffMinus4Bool);
                send(USER_INFO_INTERESTS,ID,Infos.Interests,dataWasTruncatedTo0xffffMinus4Bool);
				
				sAge.setNum(Infos.Age,10);
                send(USER_INFO_AGE	,ID,sAge,dataWasTruncatedTo0xffffMinus4Bool);
			}
			else if(ProtocolInfoTag=="1007"){//GET_MIN_PROTOCOLVERSION_FILETRANSFER
                bool dataWasTruncatedTo0xffffMinus4Bool;
                send(ANSWER_OF_GET_MIN_PROTOCOLVERSION_FILETRANSFER,ID,FileTransferProtocol::MINPROTOCOLVERSION,dataWasTruncatedTo0xffffMinus4Bool);
			}
			else if(ProtocolInfoTag=="1008"){//GET_AVATARIMAGE
                bool dataWasTruncatedTo0xffffMinus4Bool;
                send(ANSWER_OF_GET_AVATARIMAGE_IMAGE,ID, mCore.getUserInfos().AvatarImage,dataWasTruncatedTo0xffffMinus4Bool);
			}

			
			
		//end of commands
		
		//Messages
			else if(ProtocolInfoTag=="0001"){//ANSWER_OF_GET_CLIENTVERSION
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){	
					QString ClientVersion=Data.mid(4);
					thisUser->setClientVersion(ClientVersion);
				}
			
			}
			else if(ProtocolInfoTag=="0002"){//ANSWER_OF_GET_CLIENTNAME
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){	
					QString Clientname=Data.mid(4);
					thisUser->setClientName(Clientname);
				}
			
			}
			else if(ProtocolInfoTag=="0003"){//chatmessage
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){	
					QByteArray temp=Data.mid(4);
					thisUser->slotIncomingNewChatMessage(temp);
				}	
			}
			else if(ProtocolInfoTag=="0004"){//USER_ONLINESTATUS_ONLINE
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){	
					thisUser->setOnlineState(USERONLINE);
				}
			}
			else if(ProtocolInfoTag=="0005"){//USER_ONLINESTATUS_OFFLINE || USER_ONLINESTATUS_INVISIBLE
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){	
					thisUser->setOnlineState(USEROFFLINE);
				}
			}
			else if(ProtocolInfoTag=="0006"){//USER_ONLINESTATUS_WANTTOCHAT
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){	
					thisUser->setOnlineState(USERWANTTOCHAT);
				}
			}
			else if(ProtocolInfoTag=="0007"){//USER_ONLINESTATUS_AWAY
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){	
					thisUser->setOnlineState(USERAWAY);
				}
			}
			else if(ProtocolInfoTag=="0008"){//USER_ONLINESTATUS_DONT_DISTURB
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){	
					thisUser->setOnlineState(USERDONT_DISTURB);
				}
			}
			else if(ProtocolInfoTag=="0009"){//ANSWER_OF_GET_PROTOCOLVERSION,
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){	
					QString temp=Data.mid(4);
					thisUser->setProtocolVersion(temp);
				}
			}
			else if(ProtocolInfoTag=="0010"){//ANSWER_OF_GET_MAX_PROTOCOLVERSION_FILETRANSFER,
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){	
					QString temp=Data.mid(4);
					thisUser->setMaxProtocolVersionFiletransfer(temp);
				}
			}
			else if(ProtocolInfoTag=="0011"){//USER_INFO_NICKNAME
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){
					QString temp=Data.mid(4);
					thisUser->setRecivedUserInfos(AbstractUserLocalImage::NICKNAME,temp);
				}
			}
			else if(ProtocolInfoTag=="0012"){//USER_INFO_GENDER
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){
					QString temp=Data.mid(4);
					thisUser->setRecivedUserInfos(AbstractUserLocalImage::GENDER,temp);
				}
			}
			else if(ProtocolInfoTag=="0013"){//USER_INFO_AGE
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){
					QString temp=Data.mid(4);
					thisUser->setRecivedUserInfos(AbstractUserLocalImage::AGE,temp);
				}
			}
			else if(ProtocolInfoTag=="0014"){//USER_INFO_INTERESTS
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){
					QString temp=Data.mid(4);
					thisUser->setRecivedUserInfos(AbstractUserLocalImage::INTERESTS,temp);
				}
			}
			else if(ProtocolInfoTag=="0015"){//USER_BLOCK_INVISIBLE
				//SHOW USER AS OFFLINE
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){
				    mCore.deletePacketManagerByID(ID);
				    if( mCore.getConnectionManager()->isComponentStopped()==false){
					    mCore.getConnectionManager()->doDestroyStreamObjectByID(ID);
				    }
				    thisUser->setConnectionStatus(OFFLINE);
				}
			}
			else if(ProtocolInfoTag=="0016"){//USER_BLOCK_NORMAL
				//SHOW USER AS BLOCKED
				
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){
				    thisUser->setOnlineState(USERBLOCKEDYOU);
				    thisUser->setConnectionStatus(OFFLINE);
				    mCore.deletePacketManagerByID(ID);

				    if( mCore.getConnectionManager()->isComponentStopped()==false){
					    mCore.getConnectionManager()->doDestroyStreamObjectByID(ID);
				    } 
				}
			}
			else if(ProtocolInfoTag=="0017"){//ANSWER_OF_GET_MIN_PROTOCOLVERSION_FILETRANSFER:
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){
				      QString temp=Data.mid(4);
				      thisUser->setMinProtocolVersionFiletransfer(temp);
				}
			}
			else if(ProtocolInfoTag=="0018"){//ANSWER_OF_GET_AVATARIMAGE_IMAGE
				AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
				if(thisUser!=NULL){
				    QByteArray temp=Data.mid(4);
				    thisUser->setAvatarImage(temp);
				}
			}
			else if(ProtocolInfoTag=="0019"){//AVATARIMAGE_CHANGED
				send(GET_AVATARIMAGE,ID);
			}
            else if(ProtocolInfoTag=="0020"){ //CHAT_MESSAGE_SWARM
                AbstractUserLocalImage* thisUser=mCore.getUserManager()->getUserByI2P_ID(ID);
                if(thisUser!=NULL){
                    QByteArray temp=Data.mid(4);
                    int nIndex = temp.indexOf("\n");
                    if(nIndex>0){
                        QByteArray swarmStr = temp.mid(0,nIndex);
                        int tIndex = temp.indexOf("\t")+1;
                        QByteArray swarmIdStr = swarmStr.mid(tIndex);
                        QString swarmIdRemoteStr = QString.fromLatin1(swarmIdStr);
                        bool ok;
                        SwarmType1LocalIdType swarmRemoteId = swarmIdRemoteStr.toInt(&ok);
                        if(!ok){
                            qCritical()<<"Malformed incoming message: bad swarm remote id in CHAT_MESSAGE_SWARM:"<<swarmIdRemoteStr;
                        }else{
                            temp=temp.mid(nIndex+1);
                            qDebug()<<"slotIncomingNewChatMessageAtSwarm: message:"<<message<<", swarmRemoteId: "<<swarmRemoteId;

                            SwarmManager ...->slotIncomingNewChatMessageAtSwarm(temp,swarmRemoteId);
                        }
                    }else{
                        qCritical()<<"Malformed incoming message: nIndex on CHAT_MESSAGE_SWARM is:"<<nIndex;
                    }
                }
            }
            //end Messages
			else{
				qWarning()<<"File\t"<<__FILE__<<endl
					<<"Line:\t"<<__LINE__<<endl
					<<"Function:\t"<<"CProtocol::slotInputKnown"<<endl
					<<"Message:\t"<<"Unknown ProtocolInfoTag: "<<ProtocolInfoTag<<endl<<"Packet ignored"<<endl;
			}
}

void Connector::slotInputUnknown(const qint32 ID, const QByteArray Data){
using namespace Protocol_Info;

	CI2PStream* stream=mCore.getI2PStreamObjectByID(ID);

    if(stream==0) {
        auto msg = "Protocol.cpp : stream object is null, ignoring incoming data\n";
        qDebug() << msg;
        QErrorMessage * box = new QErrorMessage(); box->showMessage(msg);
        return;
    }

	if(stream->getConnectionType()==UNKNOWN){
	//check if First Paket = from a other CHATSYSTEM
		if(Data.contains("CHATSYSTEM\t")==true){
			QByteArray temp=Data.mid(Data.indexOf("\t")+1,Data.indexOf("\n")-Data.indexOf("\t")-1);
			QString version(temp);
			bool	OK=false;
			double  versiond=version.toDouble(&OK);
			if(OK==false){
				qCritical()<<"File\t"<<__FILE__<<endl
					   <<"Line:\t"<<__LINE__<<endl
					   <<"Function:\t"<<"CProtocol::slotInputUnknown"<<endl
					   <<"Message:\t"<<"Can't convert QString to double"<<endl
					   <<"QString:\t"<<version<<endl;
				
			}


			//dont send the firstpacket if you have connected someone
			//(the firstpacket is sended from core::StreamStatusRecived)			
			if(ID < 0){
				newConnectionChat(ID);//someone connect you
				
				if(mCore.getUserBlockManager()->isDestinationInBlockList(stream->getDestination())==true){
				    //is blocked
				    
				    if(versiond<0.4){
                      bool dataWasTruncatedTo0xffffMinus4Bool;
                      send(CHATMESSAGE,ID,QString("You were blocked,all Packets will be ignored !"),dataWasTruncatedTo0xffffMinus4Bool);
					  mCore.getConnectionManager()->doDestroyStreamObjectByID(ID);
					  return;
				    }
				    else {
				     	QSettings settings(mCore.getConfigPath()+"/application.ini",QSettings::IniFormat);
					settings.beginGroup("Security");	
                        bool dataWasTruncatedTo0xffffMinus4Bool;
					      if(settings.value("BlockStyle","Normal").toString()=="Normal"){
                            send(CHATMESSAGE,ID,QString("You were blocked ,all Packets will be ignored !"),dataWasTruncatedTo0xffffMinus4Bool);
                            send(USER_BLOCK_NORMAL,ID,QString(""),dataWasTruncatedTo0xffffMinus4Bool);
					      }else{
						    //Block-Style Invisible
                            send(USER_BLOCK_INVISIBLE,ID,QString(""),dataWasTruncatedTo0xffffMinus4Bool);
					      }
					settings.endGroup();
					settings.sync();
					mCore.getConnectionManager()->doDestroyStreamObjectByID(ID);
					return;
				    }
				}
			}

			
			//remove Firstpacket
			QByteArray Data2=Data;
			Data2=Data2.remove(0,Data.indexOf("\n")+1);
			if(mCore.getUserManager()->checkIfUserExitsByI2PDestination(stream->getDestination())==false){
				
				if(versiond>=0.3){
					mCore.getUserManager()->addNewUser("Receiving",stream->getDestination(),ID);
				}
				else{
					mCore.getUserManager()->addNewUser("Unknown",stream->getDestination(),ID);
				}

				AbstractUserLocalImage* AbstractUserLocalImage=mCore.getUserManager()->getUserByI2P_Destination(stream->getDestination());
				AbstractUserLocalImage->setI2PStreamID(ID);
				AbstractUserLocalImage->setProtocolVersion(version);
				AbstractUserLocalImage->setConnectionStatus(ONLINE);
				mCore.setStreamTypeToKnown(ID,Data2,false);
				if(versiond>=0.3){
					AbstractUserLocalImage->setRecivedNicknameToUserNickname();
				}
			}
			else{
				if(mCore.useThisChatConnection(stream->getDestination(),ID)==true){
					AbstractUserLocalImage* AbstractUserLocalImage=mCore.getUserManager()->getUserByI2P_Destination(stream->getDestination());
					AbstractUserLocalImage->setI2PStreamID(ID);
					AbstractUserLocalImage->setProtocolVersion(version);
					AbstractUserLocalImage->setConnectionStatus(ONLINE);
					mCore.setStreamTypeToKnown(ID,Data2,false);
				}
			}
			//return Data2;
		}
		else if(Data.contains("CHATSYSTEMFILETRANSFER\t")==true){
			//FIRSTPAKET ="CHATSYSTEMFILETRANSFER\t"+PROTOCOLVERSION+"\nSizeinBit\nFileName";
			QByteArray Data2=Data;

			QString ProtovolVersion=Data2.mid(Data.indexOf("\t")+1,Data2.indexOf("\n")-Data2.indexOf("\t")-1);
				Data2.remove(0,Data2.indexOf("\n")+1);//CHATSYSTEMFILETRANSFER\tPROTOCOLVERSION
			
			QString FileSize=Data2.mid(0,Data2.indexOf("\n"));
				Data2.remove(0,Data2.indexOf("\n")+1);

			QString FileName=Data2;
			QString Destination;
			
			Destination=stream->getDestination(); 
			
			if(mCore.getUserBlockManager()->isDestinationInBlockList(stream->getDestination())==true){
			      mCore.getConnectionManager()->doDestroyStreamObjectByID(ID);
			      Data2.clear();
			      return;
			}

			Data2.clear();
			mCore.setStreamTypeToKnown(ID,Data2,true);
			mCore.setUserProtocolVersionByStreamID(ID,ProtovolVersion);
			mCore.getFileTransferManager()->addNewFileRecive(ID,FileName,FileSize,Destination,ProtovolVersion);
		}
		else{
			// not from a other CHATSYSTEM
			if(mCore.getUserBlockManager()->isDestinationInBlockList(stream->getDestination())==true){
			    mCore.getConnectionManager()->doDestroyStreamObjectByID(ID);
			}else{
			    *(stream)<<(QString)HTTPPAGE;
			    mCore.getConnectionManager()->doDestroyStreamObjectByID(ID);
			}
		}
	}
}

void Connector::send(const COMMANDS_TAGS TAG,const qint32 ID)const 
{
	using namespace Protocol_Info;

	CI2PStream* stream=mCore.getI2PStreamObjectByID(ID);

	QString ProtocolInfoTag;
	QString Data="";
	switch(TAG){
		case PING:{				ProtocolInfoTag="1000";		break;}
		case GET_CLIENTVERSION:{		ProtocolInfoTag="1001";		break;}
		case GET_CLIENTNAME:{			ProtocolInfoTag="1002";		break;}
		case GET_USER_ONLINESTATUS:{		ProtocolInfoTag="1003";		break;}
		case GET_PROTOCOLVERSION:{		ProtocolInfoTag="1004";		break;}
		case GET_MAX_PROTOCOLVERSION_FILETRANSFER:{ProtocolInfoTag="1005";	break;}
		case GET_USER_INFOS:{			ProtocolInfoTag="1006";		break;}
		case GET_MIN_PROTOCOLVERSION_FILETRANSFER:{ProtocolInfoTag="1007";	break;}
		case GET_AVATARIMAGE:{			ProtocolInfoTag="1008";		break;}
		default:
		{
			qCritical()<<"File\t"<<__FILE__<<endl
				<<"Line:\t"<<__LINE__<<endl
				<<"Function:\t"<<"CProtocol::send"<<endl
				<<"Message:\t"<<"unhandeld Command-TAG"<<"exit"<<endl;
			exit(-1);
		}

	}
	Data.insert(0,ProtocolInfoTag);
	Data.insert(0,"0004");//No PaketData
	*(stream)<<Data;
}




void Connector::send(const MESSAGES_TAGS TAG,const qint32 ID,QString Data,bool&dataWasTruncatedTo0xffffMinus4Bool) const
{
	QByteArray t="";
	t.insert(0,Data);

    send(TAG,ID,t,dataWasTruncatedTo0xffffMinus4Bool);
}
void Connector::send(const MESSAGES_TAGS TAG,const qint32 ID,QByteArray Data,bool&dataWasTruncatedTo0xffffMinus4Bool) const
{
	QString ProtocolInfoTag;

	CI2PStream* stream=mCore.getI2PStreamObjectByID(ID);
	
	switch(TAG)
	{
		case ECHO_OF_PING:{			ProtocolInfoTag="0000";		break;}
		case ANSWER_OF_GET_CLIENTVERSION:{	ProtocolInfoTag="0001";		break;}
		case ANSWER_OF_GET_CLIENTNAME:{		ProtocolInfoTag="0002";		break;}
		case CHATMESSAGE:{			ProtocolInfoTag="0003";		break;}
		case USER_ONLINESTATUS_ONLINE:{		ProtocolInfoTag="0004";		break;}
		case USER_ONLINESTATUS_OFFLINE:
		case USER_ONLINESTATUS_INVISIBLE:{	ProtocolInfoTag="0005";		break;}
		case USER_ONLINESTATUS_WANTTOCHAT:{	ProtocolInfoTag="0006";		break;}
		case USER_ONLINESTATUS_AWAY:{		ProtocolInfoTag="0007";		break;}
		case USER_ONLINESTATUS_DONT_DISTURB:{	ProtocolInfoTag="0008";		break;}
		case ANSWER_OF_GET_PROTOCOLVERSION:{	ProtocolInfoTag="0009";		break;}
		case ANSWER_OF_GET_MAX_PROTOCOLVERSION_FILETRANSFER:{ProtocolInfoTag="0010";break;}
		case USER_INFO_NICKNAME:{		ProtocolInfoTag="0011";		break;}
		case USER_INFO_GENDER:{			ProtocolInfoTag="0012";		break;}
		case USER_INFO_AGE:{			ProtocolInfoTag="0013";		break;}
		case USER_INFO_INTERESTS:{		ProtocolInfoTag="0014";		break;}
		case USER_BLOCK_INVISIBLE:{		ProtocolInfoTag="0015";		break;}
		case USER_BLOCK_NORMAL:{		ProtocolInfoTag="0016";		break;}
		case ANSWER_OF_GET_MIN_PROTOCOLVERSION_FILETRANSFER:{ ProtocolInfoTag="0017";	break;}
		case ANSWER_OF_GET_AVATARIMAGE_IMAGE:{	ProtocolInfoTag="0018";		break;}
		case AVATARIMAGE_CHANGED: {		ProtocolInfoTag="0019";		break;}
		default:
		{
			qCritical()<<"File\t"<<__FILE__<<endl
				<<"Line:\t"<<__LINE__<<endl
				<<"Function:\t"<<"CProtocol::send"<<endl
				<<"Message:\t"<<"unhandeld Message-TAG"<<"exit"<<endl;
				
			exit(-1);
		}	
	}
    dataWasTruncatedTo0xffffMinus4Bool=false;
	QString temp;
    if(Data.length()>(0xffff-4)){
        Data = Data.mid(0,(0xffff-4));
        qCritical()<<"Outgoing data was truncated to"<<(0xffff-4)<<"bytes";
        dataWasTruncatedTo0xffffMinus4Bool=true;
    }
	temp.setNum(Data.length()+4,16);//hex

	QString Paketlength=QString("%1").arg(temp,4,'0');

	Data.insert(0,ProtocolInfoTag);
	Data.insert(0,Paketlength);
	*(stream)<<Data;
}

Connector::~ Connector()
{
}
