#include "swarmtype1rosterentry.h"
#include "SwarmType1.h"
#include "QtNumeric"
#include "UnsentChatMessageStorage.h"
#include "Core.h"

SwarmType1RosterEntry::SwarmType1RosterEntry(SwarmType1&swarm_,CCore&chatCore):AbstractGroupRosterEntry(chatCore),swarm(swarm_) {}

SwarmType1RosterEntry::~SwarmType1RosterEntry() {}


SwarmType1RosterEntry* SwarmType1RosterEntry::createRosterEntryForSwarmType1(SwarmType1&swarm,CCore&chatCore) {
    return new SwarmType1RosterEntry(swarm,chatCore);
}


void SwarmType1RosterEntry::save(QTextStream& out, CUnsentChatMessageStorage& /*offlineMessagesStorage*/) {
    out<<"LocalNick:\t"<<swarm.getLocalSwarmNick()<<endl
       <<"LocalId:\t"<<swarm.getLocalId()<<endl;
    swarm.getWhiteList().saveToQTextStreamForRosterConf(out);

    //TODO offlineMessagesStorage.saveUnsentChatMessagesForSwarmType1(swarm);
}

QString SwarmType1RosterEntry::getMapIdQString() {
    return QString("SwarmType1_")+QString::number(swarm.getLocalId());
}

AbstractRosterEntry* SwarmType1RosterEntry::loadRosterEntry(
        QTextStream& in, CUnsentChatMessageStorage& mUnsentMessageStorage, CUserManager& userManager, CCore& chatCore) {
    QString line = in.readLine();
    QStringList temp=line.split("\t");
    QString LocalNick;
    QString LocalId;
    int WhiteListLength;
    if(temp.size()<2)return nullptr;
    if(temp[0]=="LocalNick:"){
        LocalNick=temp[1];
        line = in.readLine();
        temp=line.split("\t");
        if(temp.size()<2)return nullptr;
    }
    if(temp[0]=="LocalId:"){
        LocalId=temp[1];
        line = in.readLine();
        temp=line.split("\t");
        if(temp.size()<2)return nullptr;
    }
    if(temp[0]=="WhiteListLength:"){
        WhiteListLength=temp[1].toInt();
    }
    SwarmType1WhiteList* WhiteList= SwarmType1WhiteList::loadFromRosterConf(in, WhiteListLength, userManager, mUnsentMessageStorage);
    SwarmType1* swarm = SwarmType1::loadSwarm(LocalNick, LocalId, WhiteList, chatCore);
    //TODO load unsent ChatMessages
    /*
    QStringList message=mUnsentMessageStorage.getMessagesForSwarmType1(swarm);
    for(int i=0;i<message.count();i++){
      if(message.at(i).isEmpty()==false){
        swarm->slotSendChatMessage(message.at(i));
      }
    }
    */
    return new SwarmType1RosterEntry(*swarm,chatCore);
}

SwarmType1& SwarmType1RosterEntry::getSwarm() {
    return swarm;
}
