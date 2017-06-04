#include "SwarmType1RosterEntry.h"

#include "AppContext.h"
#include "QtNumeric"
#include "SwarmType1LocalImage.h"
#include "UnsentChatMessagesStorage.h"

SwarmType1RosterEntry::SwarmType1RosterEntry(SwarmType1LocalImage&swarm_,AppContext&chatCore):AbstractGroupRosterEntry(chatCore),swarm(swarm_) {}

SwarmType1RosterEntry::~SwarmType1RosterEntry() {}


SwarmType1RosterEntry* SwarmType1RosterEntry::createRosterEntryForSwarmType1(SwarmType1LocalImage&swarm,AppContext&chatCore) {
    return new SwarmType1RosterEntry(swarm,chatCore);
}


void SwarmType1RosterEntry::save(QTextStream& out, UnsentChatMessagesStorage& /*offlineMessagesStorage*/) {
    out<<"LocalNick:\t"<<swarm.getLocalSwarmNick()<<endl
       <<"LocalId:\t"<<swarm.getLocalId()<<endl;
    swarm.getWhiteList().saveToQTextStreamForRosterConf(out);

    //TODO offlineMessagesStorage.saveUnsentChatMessagesForSwarmType1(swarm);
}

QString SwarmType1RosterEntry::getMapIdQString() {
    return QString("SwarmType1_")+QString::number(swarm.getLocalId());
}

AbstractRosterEntry* SwarmType1RosterEntry::loadRosterEntry(
        QTextStream& in, UnsentChatMessagesStorage& mUnsentMessageStorage, UserManager& userManager, AppContext& chatCore) {
    QString line = in.readLine();
    QStringList temp=line.split("\t");
    QString LocalNick;
    QString LocalId;
    int WhiteListLength;
    if(temp.size()<2)reportParseError(QObject::tr("line must have two tokens"),roster_conf);
    if(temp[0]=="LocalNick:"){
        LocalNick=temp[1];
        line = in.readLine();
        temp=line.split("\t");
        if(temp.size()<2)reportParseError(QObject::tr("line must have two tokens"),roster_conf);
    }
    if(temp[0]=="LocalId:"){
        LocalId=temp[1];
        line = in.readLine();
        temp=line.split("\t");
        if(temp.size()<2)reportParseError(QObject::tr("line must have two tokens"),roster_conf);
    }
    if(temp[0]=="WhiteListLength:"){
        WhiteListLength=temp[1].toInt();
    }
    SwarmType1WhiteList* WhiteList= SwarmType1WhiteList::loadFromRosterConf(in, WhiteListLength, userManager, mUnsentMessageStorage);
    SwarmType1LocalImage* swarm = SwarmType1LocalImage::loadSwarm(LocalNick, LocalId, WhiteList, chatCore);
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

SwarmType1LocalImage& SwarmType1RosterEntry::getSwarm() {
    return swarm;
}

//send to all users from whitelist
void SwarmType1RosterEntry::sendChatMessage(QString & message){
    SwarmType1WhiteList& whiteList = getSwarm().getWhiteList();
    QList<AbstractUserLocalImage*>* whiteListUsers = whiteList.getUsersSetClone();
    for(QList<AbstractUserLocalImage*>::const_iterator it=whiteListUsers->begin();it!=whiteListUsers->end();++it) {
        AbstractUserLocalImage* whiteListUser=*it;
        whiteListUser->transferMessageToSwarmWhiteListUser(getSwarm(),message);
    }
    deallocUsersSetClone(whiteListUsers);
}
