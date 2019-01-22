#ifndef SWARMSMANAGER_H
#define SWARMSMANAGER_H

#include <QObject>

#include "AbstractGroupRosterEntry.h"
#include "SwarmType1RosterEntry.h"
#include "SwarmType1LocalImage.h"

class SwarmsController : public QObject
{
    Q_OBJECT
public:
    explicit SwarmsController(QObject *parent = 0);

    //FIXME unreliable, can lose business data
    QStringList pickNewIncomingMessagesAndClear(shared_ptr<AbstractGroupRosterEntry> group) {
        if(!group)return QStringList();
        auto swarmRosterEntry = group->asSwarmType1();
        if(!swarmRosterEntry)return QStringList();
        auto swarm = swarmRosterEntry->getSwarm();
        return swarm->pickNewIncomingMessagesAndClear(); //FIXME unreliable, can lose business data
    }

signals:

public slots:
};

#endif // SWARMSMANAGER_H
