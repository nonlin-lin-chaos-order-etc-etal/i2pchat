#include <QTextStream>

#include "rostermodel.h"
#include "UserManager.h"
#include "Core.h"
#include "actorrosterentry.h"
#include "rosterentryfactory.h"

RosterModel::RosterModel(
        CCore& chatCore_, CUserManager& userManager_, CUnsentChatMessageStorage& offlineMessagesStorage_, QObject *parent) :
    QObject(parent), chatCore(chatCore_), userManager(userManager_),
    offlineMessagesStorage(offlineMessagesStorage_), entryIdQStrToRosterEntry()
{
    load();
}


void RosterModel::save()const {
    QFile file(chatCore.getConfigPath()+"/roster.conf");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    for(QMap<QString,AbstractRosterEntry*>::const_iterator it=entryIdQStrToRosterEntry.begin();it != entryIdQStrToRosterEntry.end(); ++it) {
        out << (*it)->getClassTag() << endl;
        (*it)->save(out, offlineMessagesStorage);
    }

    out.flush();
    file.close();
}

void RosterModel::load() {
    {
        //new style db
        QFile file(chatCore.getConfigPath()+"/roster.conf");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QByteArray ba=file.readAll();
            file.close();
            QTextStream in(ba);

            QString line;
            QStringList tokens;

            while (!in.atEnd()) {
                line = in.readLine();
                tokens = line.split("\t");
                if(tokens.isEmpty()) {
                    //skip empty line
                    continue;
                }
                loadRosterEntry(tokens, in);
            }

            //finishing migration
            //QFile fileOld(chatCore.getConfigPath()+"/users.config");
            //fileOld.remove();
            return;
        }
    }
    {
        //old style
        QFile file(chatCore.getConfigPath()+"/users.config");

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QByteArray bUserList=file.readAll();
        file.close();
        QTextStream in(bUserList);
        in.skipWhiteSpace();

        QString nickName;
        QString i2pDest;

        QString line;
        QStringList temp;

        while (!in.atEnd()) {
            line = in.readLine(550);
            temp=line.split("\t");

            if(temp.size()<2)continue;

            if(temp[0]=="Nick:"){
                nickName=temp[1];
            } else if(temp[0]=="I2PDest:") {
                i2pDest=temp[1];
                loadNewActor(nickName,i2pDest);

            } else if(temp[0]=="Invisible:") {
                if(temp[1]=="true"){
                    CUser* user = userManager.getUserByI2P_Destination(i2pDest);
                    if(user)user->setInvisible(true);
                }
            } else {
                //ignore garbage
            }
        }
    }
}


void RosterModel::loadNewActor(QString& nickName,QString& i2pDest) {
    CUser* resultingNewUser = nullptr;
    userManager.addNewUser(nickName,i2pDest,0,false, &resultingNewUser);
    if(!resultingNewUser)return;

    AbstractRosterEntry* entry = RosterEntryFactory::createRosterEntryForOldStyleUser(*resultingNewUser, userManager, offlineMessagesStorage);
    insertRosterEntry(entry);
}

void RosterModel::loadRosterEntry(QStringList& lookaheadTokens, QTextStream& input) {
    if(lookaheadTokens.isEmpty())return;
    QString& rosterEntryClassTag=lookaheadTokens[0];
    AbstractRosterEntry* entry = RosterEntryFactory::loadRosterEntry(rosterEntryClassTag,input,userManager, offlineMessagesStorage, chatCore);
    insertRosterEntry(entry);
}

void RosterModel::invalidateContents() {
    entryIdQStrToRosterEntry.clear();
}

QList<AbstractRosterEntry*>* RosterModel::cloneRosterEntriesSet() {
    return new QList<AbstractRosterEntry*>(entryIdQStrToRosterEntry.values());
}


void RosterModel::createNewSwarmType1(SwarmType1&swarm) {
    SwarmType1RosterEntry* entry = RosterEntryFactory::createRosterEntryForSwarmType1(swarm,chatCore);
    insertRosterEntry(entry);
}

void RosterModel::insertRosterEntry(AbstractRosterEntry* entry) {
    if(!entry)return;
    entryIdQStrToRosterEntry[entry->getMapIdQString()]=entry;
    save();
}
