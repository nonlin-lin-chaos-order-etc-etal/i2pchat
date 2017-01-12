#ifndef ROSTERMODEL_H
#define ROSTERMODEL_H

#include <QTextStream>
#include <QObject>
#include <QSet>

#include "actorrosterentry.h"

class CUserManager;
class CCore;
class AbstractRosterEntry;
class CUnsentChatMessageStorage;
class SwarmType1;

class RosterModel : public QObject
{
    Q_OBJECT
public:
    RosterModel(CCore& chatCore_, CUserManager& userManager_, CUnsentChatMessageStorage& offlineMessagesStorage_,
                         QObject *parent = nullptr);

    void invalidateContents();

signals:

public slots:
private:
    void 		save() const;
    void 		load();

    friend class CUserManager;

    //forbid some operators
    RosterModel(const RosterModel&)=delete;
    RosterModel& operator=(const RosterModel&)=delete;

    CCore& chatCore;
    CUserManager& userManager;
    CUnsentChatMessageStorage& offlineMessagesStorage;

    QMap<QString,AbstractRosterEntry*> entryIdQStrToRosterEntry;

    void loadNewActor(QString& nickName,QString& i2pDest);

    void loadRosterEntry(QStringList& lookaheadTokens, QTextStream& input, parsed_item_file);
public:
    void createNewSwarmType1(SwarmType1&swarm);
    AbstractRosterEntry* operator [](QString mapId) { return entryIdQStrToRosterEntry[mapId]; }
private:
    void insertRosterEntry(AbstractRosterEntry* entry);
public:
    //dealloc by `delete result`
    QList<AbstractRosterEntry*>* cloneRosterEntriesSet();
    void removeActorByI2PDestination(QString I2PDestination) {
        for(QMap<QString,AbstractRosterEntry*>::iterator it = entryIdQStrToRosterEntry.begin(); it!=entryIdQStrToRosterEntry.end(); ++it) {
            AbstractRosterEntry* e = *it;
            ActorRosterEntry * actor = e->asActor();
            if(!actor)continue;
            if(actor->getUser().getI2PDestination()==I2PDestination) {
                entryIdQStrToRosterEntry.remove(actor->getMapIdQString());
                return;
            }
        }
    }
};

#endif // ROSTERMODEL_H
