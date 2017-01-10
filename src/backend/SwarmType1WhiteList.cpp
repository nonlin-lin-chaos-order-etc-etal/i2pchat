/*
 * SwarmType1WhiteList.cpp
 *
 *  Created on: Jan 10, 2017
 *      Author: user
 */

#include "SwarmType1WhiteList.h"
#include <QTextStream>
#include "User.h"
#include <QList>
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
    for(QMap<QString,CUser*>::const_iterator it=i2pdestqstr2cuser.begin();it!=i2pdestqstr2cuser.end();++it) {
        CUser* user = *it;
        out << "WhiteListEntry_Nick:\t"<<user->getName()<<endl;
        out << "WhiteListEntry_I2PDest:\t"<<user->getI2PDestination()<<endl;
    }
}

SwarmType1WhiteList* SwarmType1WhiteList::loadFromRosterConf(
        QTextStream&in, int WhiteListLength, CUserManager& userManager, CUnsentChatMessageStorage& mUnsentMessageStorage) {
    SwarmType1WhiteList * whitelist = new SwarmType1WhiteList();
    for(int i=0;i<WhiteListLength;++i) {
        QString line = in.readLine();
        QStringList temp=line.split("\t");
        QString NickName;
        QString I2PDest;
        if(temp.size()<2)return whitelist;
        if(temp[0]=="WhiteListEntry_Nick:"){
            NickName=temp[1];
            line = in.readLine();
            temp=line.split("\t");
            if(temp.size()<2)return whitelist;
        }

        if(temp[0]=="WhiteListEntry_I2PDest:"){
            I2PDest=temp[1];

            CUser* resultingNewUser;

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
            }
        }
    }
    return whitelist;
}


void SwarmType1WhiteList::addUser(CUser&user) {
    i2pdestqstr2cuser[user.getI2PDestination()]=&user;
}

void SwarmType1WhiteList::removeUser(CUser&user) {
    i2pdestqstr2cuser.remove(user.getI2PDestination());
}

QList<CUser*>* SwarmType1WhiteList::getUsersSetClone() {
    return new QList<CUser*>(i2pdestqstr2cuser.values());
}
