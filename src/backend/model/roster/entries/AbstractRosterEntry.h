#ifndef ABSTRACTROSTERENTRY_H
#define ABSTRACTROSTERENTRY_H

#include <QTextStream>
#include <QString>

#include <memory>

using namespace std;

class AbstractGroupRosterEntry;
class OneToOneRosterEntry;
class SwarmType1RosterEntry;

class AbstractRosterEntry : public std::enable_shared_from_this<AbstractRosterEntry> {
public:
    AbstractRosterEntry(){};
    virtual ~AbstractRosterEntry(){};

    //for serialization
    virtual QString getClassTag()=0;

    //typecasts
    virtual shared_ptr<AbstractGroupRosterEntry> asGroup()=0;
    virtual shared_ptr<OneToOneRosterEntry> asOneToOne()=0;
    virtual shared_ptr<SwarmType1RosterEntry> asSwarmType1()=0;

    virtual QString getMapIdQString()=0;
    
    virtual bool hasUnreadChatMessages()=0;
};

#endif // ABSTRACTROSTERENTRY_H
