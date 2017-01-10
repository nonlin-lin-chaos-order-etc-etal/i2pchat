#ifndef ABSTRACTGROUPROSTERENTRY_H
#define ABSTRACTGROUPROSTERENTRY_H
#include "abstractrosterentry.h"

#include <QFont>
#include <QColor>

class ActorRosterEntry;
class SwarmType1RosterEntry;
class CCore;

class AbstractGroupRosterEntry : public AbstractRosterEntry {
public:
    AbstractGroupRosterEntry(CCore& chatCore);
    virtual ~AbstractGroupRosterEntry();

    //typecasts
    virtual AbstractGroupRosterEntry * asGroup(){return this;}
    virtual ActorRosterEntry * asActor()=0;
    virtual SwarmType1RosterEntry * asSwarmType1()=0;

    virtual QStringList pickNewIncomingMessagesAndClear()=0; //FIXME unreliable, can lose business data

    virtual QStringList getUnsentMessages()=0;
    virtual void setEditedUnsentMessages(QStringList outbox)=0;

    //send to all users from whitelist
    virtual void sendChatMessage(QString & message)=0;

    virtual QFont& getTextFont();
    virtual QColor& getTextColor();
    virtual void setTextFont(QFont);
    virtual void setTextColor(QColor);
private:
    QFont* textFont;
    QColor* textColor;
};

#endif // ABSTRACTGROUPROSTERENTRY_H
