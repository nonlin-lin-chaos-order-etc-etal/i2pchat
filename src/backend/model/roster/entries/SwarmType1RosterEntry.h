#ifndef SWARMTYPE1ROSTERENTRY_H
#define SWARMTYPE1ROSTERENTRY_H

#include "AbstractGroupRosterEntry.h"

using namespace std;

class SwarmType1RosterEntry;
class SwarmType1LocalImage;

class SwarmType1RosterEntry : public AbstractGroupRosterEntry {
public:
    SwarmType1RosterEntry(){};
    virtual ~SwarmType1RosterEntry(){};

    virtual shared_ptr<SwarmType1LocalImage> getSwarm()=0;

    //typecasts
    virtual shared_ptr<SwarmType1RosterEntry> asSwarmType1(){return std::static_pointer_cast<SwarmType1RosterEntry>(shared_from_this());}

    virtual bool hasUnreadChatMessages()=0;
};

#endif // SWARMTYPE1ROSTERENTRY_H
