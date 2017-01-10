#ifndef SWARMTYPE1ROSTERENTRY_H
#define SWARMTYPE1ROSTERENTRY_H

#include "abstractgrouprosterentry.h"

class SwarmType1RosterEntry;
class SwarmType1;
class CUserManager;
class CCore;

class SwarmType1RosterEntry : public AbstractGroupRosterEntry
{
public:
    SwarmType1RosterEntry(SwarmType1&swarm,CCore&chatCore);
    virtual ~SwarmType1RosterEntry();

    static SwarmType1RosterEntry* createRosterEntryForSwarmType1(SwarmType1&swarm,CCore&chatCore);

#define SwarmType1RosterEntry_CLASS_TAG "SwarmType1RosterEntry"

    virtual QString getClassTag() { return SwarmType1RosterEntry_CLASS_TAG; }
    virtual QString getMapIdQString();
    virtual void save(QTextStream& out, CUnsentChatMessageStorage& offlineMessagesStorage);

    SwarmType1& getSwarm();

    static AbstractRosterEntry* loadRosterEntry(
            QTextStream& in, CUnsentChatMessageStorage& mUnsentMessageStorage, CUserManager& userManager, CCore& chatCore);

    //typecasts
    virtual ActorRosterEntry * asActor(){return nullptr;}//FIXME bad design: swarm type 1 can be actor
    virtual SwarmType1RosterEntry * asSwarmType1(){return this;}

    virtual bool hasUnreadChatMessages(){return false;}//TODO
    virtual QStringList pickNewIncomingMessagesAndClear(){return QStringList();} //FIXME unreliable, can lose business data

    virtual QStringList getUnsentMessages(){return QStringList();}//FIXME
    virtual void setEditedUnsentMessages(QStringList /*outbox*/){}//FIXME

    //send to all users from whitelist
    virtual void sendChatMessage(QString & /*message*/){}//FIXME

private:
    SwarmType1& swarm;
};

#endif // SWARMTYPE1ROSTERENTRY_H
