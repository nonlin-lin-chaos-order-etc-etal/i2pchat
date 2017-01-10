#ifndef ABSTRACTROSTERENTRY_H
#define ABSTRACTROSTERENTRY_H

#include <QTextStream>
#include <QString>

class CUnsentChatMessageStorage;
class AbstractGroupRosterEntry;
class ActorRosterEntry;
class SwarmType1RosterEntry;

class AbstractRosterEntry {
public:
    AbstractRosterEntry();
    virtual ~AbstractRosterEntry();

    //for serialization
    virtual QString getClassTag()=0;

    //typecasts
    virtual AbstractGroupRosterEntry * asGroup()=0;
    virtual ActorRosterEntry * asActor()=0;
    virtual SwarmType1RosterEntry * asSwarmType1()=0;

    virtual QString getMapIdQString()=0;
    virtual void save(QTextStream& out, CUnsentChatMessageStorage& offlineMessagesStorage)=0;

    virtual bool hasUnreadChatMessages()=0;
};

#endif // ABSTRACTROSTERENTRY_H
