#include "actorrosterentry.h"
#include "UserManager.h"
#include "UnsentChatMessageStorage.h"

ActorRosterEntry::ActorRosterEntry(CUser& user_): user(&user_) {

}

ActorRosterEntry::~ActorRosterEntry(){}

QString ActorRosterEntry::getClassTag() {
    return ActorRosterEntry_CLASS_TAG;
}

QString ActorRosterEntry::getMapIdQString() {
    return QString("user_")+user->getI2PDestination();
}


void ActorRosterEntry::save(QTextStream& out, CUnsentChatMessageStorage& offlineMessagesStorage){
    QString InvisibleText = user->getIsInvisible() ? "true" : "false";

    out<<"Nick:\t"<<user->getName()<<endl
       <<"I2PDest:\t"<<user->getI2PDestination()<<endl
       <<"Invisible:\t"<<InvisibleText<<endl;

    //save unsent ChatMessages for this users
    const QString Dest=user->getI2PDestination();
    const QStringList Messages=user->getUnsentedMessages();
    offlineMessagesStorage.saveChatMessagesForDest(Dest,Messages);
}


AbstractRosterEntry* ActorRosterEntry::loadRosterEntry(QTextStream& in, CUserManager & userManager, CUnsentChatMessageStorage& mUnsentMessageStorage) {
    QString line = in.readLine();
    QStringList temp=line.split("\t");
    QString NickName;
    QString I2PDest;
    if(temp.size()<2)return nullptr;
    if(temp[0]=="Nick:"){
        NickName=temp[1];
        line = in.readLine();
        temp=line.split("\t");
        if(temp.size()<2)return nullptr;
    }

    if(temp[0]=="I2PDest:"){
        I2PDest=temp[1];

        CUser* resultingNewUser;

        userManager.addNewUser(NickName,I2PDest,0,false, &resultingNewUser);
        if(!resultingNewUser)return nullptr;

        //load unsent ChatMessages
        QStringList message=mUnsentMessageStorage.getMessagesForDest(I2PDest);
        for(int i=0;i<message.count();i++){
          if(message.at(i).isEmpty()==false){
            resultingNewUser->slotSendChatMessage(message.at(i));
          }
        }
        line = in.readLine();
        temp=line.split("\t");
        if(temp.size()>=2){
            if(temp[0]=="Invisible:"){
                if(temp[1]=="true"){
                    resultingNewUser->setInvisible(true);
                }
            }
        }

        return new ActorRosterEntry(*resultingNewUser);
    }
    return nullptr;
}

AbstractRosterEntry* ActorRosterEntry::createRosterEntryForOldStyleUser(CUser& user, CUserManager & /*userManager*/, CUnsentChatMessageStorage& mUnsentMessageStorage) {
    //load unsent ChatMessages
    QStringList message=mUnsentMessageStorage.getMessagesForDest(user.getI2PDestination());
    for(int i=0;i<message.count();i++){
      if(message.at(i).isEmpty()==false){
        user.slotSendChatMessage(message.at(i));
      }
    }

    return new ActorRosterEntry(user);
}
