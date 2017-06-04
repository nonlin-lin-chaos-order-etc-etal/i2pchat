#ifndef ONETOONEROSTERENTRY_H
#define ONETOONEROSTERENTRY_H

#include "AbstractRosterEntry.h"
#include "AbstractUserLocalImage.h"
#include "RemoteUserLocalImage.h"
#include "LocalUserLocalImage.h"
#include "ParseError.h"
#include "ParseErrorsUtil.h"

#include <memory>

using namespace std;

class QString;

class OneToOneRosterEntry : public AbstractRosterEntry {

public:
    OneToOneRosterEntry(){};
    virtual ~OneToOneRosterEntry(){};

    virtual shared_ptr<LocalUserLocalImage> getLocalUser()=0;
    virtual shared_ptr<RemoteUserLocalImage> getRemoteUser()=0;

    //typecasts
    virtual shared_ptr<AbstractGroupRosterEntry> asGroup(){return nullptr;}
    virtual shared_ptr<OneToOneRosterEntry> asOneToOne(){return std::static_pointer_cast<OneToOneRosterEntry>(shared_from_this());}
    virtual shared_ptr<SwarmType1RosterEntry> asSwarmType1(){return nullptr;}

    virtual bool hasUnreadChatMessages()=0;
};

#endif // ONETOONEROSTERENTRY_H
