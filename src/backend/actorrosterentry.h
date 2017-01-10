#ifndef ACTORROSTERENTRY_H
#define ACTORROSTERENTRY_H

#include "User.h"
#include "abstractrosterentry.h"

class QString;
class CUserManager;
class CUnsentChatMessageStorage;

class ActorRosterEntry : public AbstractRosterEntry
{

public:
    ActorRosterEntry(CUser& cuser);
    virtual ~ActorRosterEntry();

#define ActorRosterEntry_CLASS_TAG "ActorRosterEntry"

    static AbstractRosterEntry* loadRosterEntry(QTextStream& in, CUserManager & userManager, CUnsentChatMessageStorage& mUnsentMessageStorage);
    static AbstractRosterEntry* createRosterEntryForOldStyleUser(CUser& user, CUserManager & userManager, CUnsentChatMessageStorage& mUnsentMessageStorage);

    virtual QString getClassTag();
    virtual QString getMapIdQString();
    virtual void save(QTextStream& out, CUnsentChatMessageStorage& offlineMessagesStorage);
    CUser& getUser();

    //typecasts
    virtual AbstractGroupRosterEntry * asGroup(){return nullptr;}//FIXME bad design: actor can be group
    virtual ActorRosterEntry * asActor(){return this;}
    virtual SwarmType1RosterEntry * asSwarmType1(){return nullptr;}//FIXME bad design: swarm type 1 can be actor

    virtual bool hasUnreadChatMessages(){return getUser().getHaveNewUnreadChatmessages();}
private:
    CUser* user;
};

#endif // ACTORROSTERENTRY_H
