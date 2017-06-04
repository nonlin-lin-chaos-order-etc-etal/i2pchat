#ifndef ABSTRACTGROUPROSTERENTRY_H
#define ABSTRACTGROUPROSTERENTRY_H

#include "AbstractRosterEntry.h"

#include <memory>

class OneToOneRosterEntry;
class SwarmType1RosterEntry;

class AbstractGroupRosterEntry : public AbstractRosterEntry {
public:
    AbstractGroupRosterEntry(){}
    virtual ~AbstractGroupRosterEntry(){}

    //typecasts
    virtual std::shared_ptr<AbstractGroupRosterEntry> asGroup(){return std::static_pointer_cast<AbstractGroupRosterEntry>(shared_from_this());}
    virtual std::shared_ptr<OneToOneRosterEntry> asOneToOne(){return nullptr;}
    virtual std::shared_ptr<SwarmType1RosterEntry> asSwarmType1()=0;
};

#endif // ABSTRACTGROUPROSTERENTRY_H
