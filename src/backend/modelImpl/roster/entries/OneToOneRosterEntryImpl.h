#ifndef ACTORROSTERENTRY_IMPL_H
#define ACTORROSTERENTRY_IMPL_H

#include "AbstractRosterEntry.h"
#include "AbstractUserLocalImage.h"
#include "ParseError.h"
#include "ParseErrorsUtil.h"

class QString;
class UserManager;
class UnsentChatMessagesStorage;

class OneToOneRosterEntry : public AbstractRosterEntry
{

public:
    OneToOneRosterEntry(AbstractUserLocalImage& cuser);
    virtual ~OneToOneRosterEntry();

#define ActorRosterEntry_CLASS_TAG "OneToOneRosterEntry"

    static AbstractRosterEntry* loadRosterEntry(QTextStream& in, UserManager & userManager, UnsentChatMessagesStorage& mUnsentMessageStorage, parsed_item_file parsedItem);
    static AbstractRosterEntry* createRosterEntryForOldStyleUser(AbstractUserLocalImage& user, UserManager & userManager, UnsentChatMessagesStorage& mUnsentMessageStorage);

    virtual QString getClassTag();
    virtual QString getMapIdQString();
    virtual void save(QTextStream& out, UnsentChatMessagesStorage& offlineMessagesStorage);
    AbstractUserLocalImage& getUser();

    //typecasts
    virtual AbstractGroupRosterEntry * asGroup(){return nullptr;}//FIXME bad design: o2o can be group
    virtual OneToOneRosterEntry * asOneToOne(){return this;}
    virtual SwarmType1RosterEntry * asSwarmType1(){return nullptr;}//FIXME bad design: swarm type 1 can be o2o

    virtual bool hasUnreadChatMessages(){return getUser().getHaveNewUnreadChatmessages();}
private:
    AbstractUserLocalImage* user;
};

#endif // ACTORROSTERENTRY_IMPL_H
