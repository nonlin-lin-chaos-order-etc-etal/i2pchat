#ifndef ABSTRACTGROUPROSTERENTRY_IMPL_H
#define ABSTRACTGROUPROSTERENTRY_IMPL_H
#include <QFont>
#include <QColor>
#include "AbstractRosterEntry.h"

class OneToOneRosterEntry;
class SwarmType1RosterEntry;
class AppContext;

class AbstractGroupRosterEntry : public AbstractRosterEntry {
public:
    AbstractGroupRosterEntry(AppContext& chatCore);
    virtual ~AbstractGroupRosterEntry();

    //typecasts
    virtual AbstractGroupRosterEntry * asGroup(){return this;}
    virtual OneToOneRosterEntry * asOneToOne()=0;
    virtual SwarmType1RosterEntry * asSwarmType1()=0;

    virtual QStringList pickNewIncomingMessagesAndClear()=0; //FIXME unreliable, can lose business data

    virtual QStringList getUnsentMessages()=0;
    virtual void setEditedUnsentMessages(QStringList outbox)=0;

    //send to all users from whitelist
    virtual void sendChatMessage(QString & message)=0;

private:
    QFont* textFont;
    QColor* textColor;
};

#endif // ABSTRACTGROUPROSTERENTRY_IMPL_H
