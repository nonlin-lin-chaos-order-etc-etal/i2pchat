#include "UserManager.h"
#include <stdexcept>
#include "OneToOneRosterEntry.h"
#include "UnsentChatMessagesStorage.h"

OneToOneRosterEntry::OneToOneRosterEntry(AbstractUserLocalImage& user_): user(&user_) {

}

OneToOneRosterEntry::~OneToOneRosterEntry(){}

QString OneToOneRosterEntry::getClassTag() {
    return ActorRosterEntry_CLASS_TAG;
}

QString OneToOneRosterEntry::getMapIdQString() {
    return QString("user_")+user->getI2PDestination();
}

AbstractUserLocalImage& OneToOneRosterEntry::getUser() { return *user; }


void OneToOneRosterEntry::save(QTextStream& out, UnsentChatMessagesStorage& offlineMessagesStorage){
    QString InvisibleText = user->getIsInvisible() ? "true" : "false";

    out<<"Nick:\t"<<user->getName()<<endl
       <<"I2PDest:\t"<<user->getI2PDestination()<<endl
       <<"Invisible:\t"<<InvisibleText<<endl;

    //save unsent ChatMessages for this users
    const QString Dest=user->getI2PDestination();
    const QStringList Messages=user->getUnsentedMessages();
    offlineMessagesStorage.saveChatMessagesForDest(Dest,Messages);
}


AbstractRosterEntry* OneToOneRosterEntry::loadRosterEntry(QTextStream& in, UserManager & userManager, UnsentChatMessagesStorage& mUnsentMessageStorage, parsed_item_file parsedItem) {
    QString line = in.readLine();
    QStringList temp=line.split("\t");
    QString NickName;
    QString I2PDest;
    if(temp.size()<2)reportParseError(QObject::tr("line must have two tokens"), parsedItem);
    if(temp[0]=="Nick:"){
        NickName=temp[1];
        line = in.readLine();
        temp=line.split("\t");
        if(temp.size()<2)reportParseError(QObject::tr("line must have two tokens"), parsedItem);
    }

    if(temp[0]=="I2PDest:"){
        I2PDest=temp[1];

        AbstractUserLocalImage* resultingNewUser;

        userManager.addNewUser(NickName,I2PDest,0,false, &resultingNewUser);
        if(!resultingNewUser){
            QString errMsg = QObject::tr("Failed to add an user: nick='")+NickName+QObject::tr("' while parsing a file type: ")+QString::number((int)parsedItem);
            QMessageBox::critical(0,QObject::tr("Error"),errMsg);
            throw std::runtime_error(errMsg.toStdString());
        }


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
        }else reportParseError(QObject::tr("line must have two tokens"), parsedItem);

        return new OneToOneRosterEntry(*resultingNewUser);
    }
    return nullptr;
}

AbstractRosterEntry* OneToOneRosterEntry::createRosterEntryForOldStyleUser(AbstractUserLocalImage& user, UserManager & /*userManager*/, UnsentChatMessagesStorage& mUnsentMessageStorage) {
    //load unsent ChatMessages
    QStringList message=mUnsentMessageStorage.getMessagesForDest(user.getI2PDestination());
    for(int i=0;i<message.count();i++){
      if(message.at(i).isEmpty()==false){
        user.slotSendChatMessage(message.at(i));
      }
    }

    return new OneToOneRosterEntry(user);
}
