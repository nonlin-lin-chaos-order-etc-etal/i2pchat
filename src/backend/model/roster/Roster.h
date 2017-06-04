#ifndef ROSTER_H
#define ROSTER_H

#include <memory>
#include "AbstractRosterEntry.h"

using namespace std;

class Roster : public std::enable_shared_from_this<Roster> {
public:
    Roster(){};
    virtual ~Roster(){};

    void invalidateContents();

private:
    //forbid some operators
    Roster(const Roster&)=delete;
    Roster& operator=(const Roster&)=delete;
public:
    virtual shared_ptr<AbstractRosterEntry> operator [](QString mapId)=0;
    //dealloc by `deallocClonedRosterEntriesSet()`
    virtual shared_ptr<QList<shared_ptr<AbstractRosterEntry>>> cloneRosterEntriesSet()=0;
    virtual void deallocClonedRosterEntriesSet(shared_ptr<QList<shared_ptr<AbstractRosterEntry>>>)=0;
    virtual void removeActorByI2PDestination(QString I2PDestination);
};

#endif // ROSTER_H
