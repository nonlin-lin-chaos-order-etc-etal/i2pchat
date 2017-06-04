#ifndef ABSTRACTROSTERENTRY_IMPL_H
#define ABSTRACTROSTERENTRY_IMPL_H

#include <QTextStream>
#include <QString>

class UnsentChatMessagesStorage;
class AbstractGroupRosterEntry;
class OneToOneRosterEntry;
class SwarmType1RosterEntry;

class AbstractRosterEntry {
public:
    AbstractRosterEntry();
    virtual ~AbstractRosterEntry();

    //for serialization
    virtual QString getClassTag()=0;

    //typecasts
    virtual AbstractGroupRosterEntry * asGroup()=0;
    virtual OneToOneRosterEntry * asOneToOne()=0;
    virtual SwarmType1RosterEntry * asSwarmType1()=0;

    virtual QString getMapIdQString()=0;
    virtual void save(QTextStream& out, UnsentChatMessagesStorage& offlineMessagesStorage)=0;

    virtual bool hasUnreadChatMessages()=0;
};

#endif // ABSTRACTROSTERENTRY_IMPL_H
