#ifndef SWARMTYPE1ROSTERENTRY_IMPL_H
#define SWARMTYPE1ROSTERENTRY_IMPL_H

#include "AbstractGroupRosterEntry.h"

class SwarmType1RosterEntry;
class SwarmType1LocalImage;
class UserManager;
class AppContext;

class SwarmType1RosterEntry : public AbstractGroupRosterEntry
{
public:
    SwarmType1RosterEntry(SwarmType1LocalImage&swarm,AppContext&chatCore);
    virtual ~SwarmType1RosterEntry();

    static SwarmType1RosterEntry* createRosterEntryForSwarmType1(SwarmType1LocalImage&swarm,AppContext&chatCore);

#define SwarmType1RosterEntry_CLASS_TAG "SwarmType1RosterEntry"

    virtual QString getClassTag() { return SwarmType1RosterEntry_CLASS_TAG; }
    virtual QString getMapIdQString();
    virtual void save(QTextStream& out, UnsentChatMessagesStorage& offlineMessagesStorage);

    SwarmType1LocalImage& getSwarm();

    static AbstractRosterEntry* loadRosterEntry(
            QTextStream& in, UnsentChatMessagesStorage& mUnsentMessageStorage, UserManager& userManager, AppContext& chatCore);

    //typecasts
    virtual OneToOneRosterEntry * asOneToOne(){return nullptr;}//FIXME bad design: swarm type 1 can be o2o
    virtual SwarmType1RosterEntry * asSwarmType1(){return this;}

    virtual bool hasUnreadChatMessages(){return false;}//TODO
    virtual QStringList pickNewIncomingMessagesAndClear(){return QStringList();} //FIXME unreliable, can lose business data

    virtual QStringList getUnsentMessages(){return QStringList();}//FIXME
    virtual void setEditedUnsentMessages(QStringList /*outbox*/){}//FIXME

    //send to all users from whitelist
    virtual void sendChatMessage(QString & /*message*/);

private:
    SwarmType1LocalImage& swarm;
};

#endif // SWARMTYPE1ROSTERENTRY_IMPL_H
