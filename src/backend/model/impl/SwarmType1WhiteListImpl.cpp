/*
 * SwarmType1WhiteList.cpp
 *
 *  Created on: Jan 10, 2017
 *      Author: user
 */

#include "SwarmType1WhiteList.h"
#include <QTextStream>
#include "AbstractUserLocalImage.h"
#include <QList>

#include "ParseErrorsUtil.h"
#include "UserManager.h"


SwarmType1WhiteList::SwarmType1WhiteList():i2pdestqstr2cuser() {
}

SwarmType1WhiteList::~SwarmType1WhiteList() {
}

void SwarmType1WhiteList::saveToQTextStreamForRosterConf(QTextStream& out) {
    //see SwarmType1RosterEntry::save()
    //    out<<"LocalNick:\t"<<swarm.getLocalSwarmNick()<<endl
    //       <<"LocalId:\t"<<swarm.getLocalId()<<endl;
    out <<"WhiteListLength:\t"<<i2pdestqstr2cuser.size()<<endl;
    for(QMap<QString,AbstractUserLocalImage*>::const_iterator it=i2pdestqstr2cuser.begin();it!=i2pdestqstr2cuser.end();++it) {
        AbstractUserLocalImage* user = *it;
        out << "WhiteListEntry_Nick:\t"<<user->getName()<<endl;
        out << "WhiteListEntry_I2PDest:\t"<<user->getI2PDestination()<<endl;
    }
}

SwarmType1WhiteList* SwarmType1WhiteList::loadFromRosterConf(
        QTextStream&in, int WhiteListLength, UserManager& userManager, UnsentChatMessagesStorage& mUnsentMessageStorage) {
    SwarmType1WhiteList * whitelist = new SwarmType1WhiteList();
    for(int i=0;i<WhiteListLength;++i) {
        QString line = in.readLine();
        QStringList temp=line.split("\t");
        QString NickName;
        QString I2PDest;
        if(temp.size()<2)reportParseError(QObject::tr("line must have two tokens"), roster_conf);
        if(temp[0]=="WhiteListEntry_Nick:"){
            NickName=temp[1];
            line = in.readLine();
            temp=line.split("\t");
            if(temp.size()<2)reportParseError(QObject::tr("line must have two tokens"), roster_conf);
        }

        if(temp[0]=="WhiteListEntry_I2PDest:"){
            I2PDest=temp[1];

            AbstractUserLocalImage* resultingNewUser;

            userManager.addNewUser(NickName,I2PDest,0,false, &resultingNewUser, true);
            if(resultingNewUser) {
                //load unsent ChatMessages
                QStringList message=mUnsentMessageStorage.getMessagesForDest(I2PDest);
                for(int i=0;i<message.count();i++){
                  if(message.at(i).isEmpty()==false){
                    resultingNewUser->slotSendChatMessage(message.at(i));
                  }
                }

                whitelist->addUser(*resultingNewUser);
            }else{
                QString errMsg = QObject::tr("Failed to add an user: nick='")+NickName+
                        QObject::tr("' while parsing roster.conf");
                QMessageBox::critical(0,QObject::tr("Error"),errMsg);
                throw std::runtime_error(errMsg.toStdString());
            }
        }
    }
    return whitelist;
}


void SwarmType1WhiteList::addUser(AbstractUserLocalImage&user) {
    i2pdestqstr2cuser[user.getI2PDestination()]=&user;
}

void SwarmType1WhiteList::removeUser(AbstractUserLocalImage&user) {
    i2pdestqstr2cuser.remove(user.getI2PDestination());
}

QList<AbstractUserLocalImage*>* SwarmType1WhiteList::getUsersSetClone() {
    return new QList<AbstractUserLocalImage*>(i2pdestqstr2cuser.values());
}
