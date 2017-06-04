#ifndef SWARMSMANAGER_H
#define SWARMSMANAGER_H

#include <QObject>

#include "AbstractGroupRosterEntry.h"

class SwarmsController : public QObject
{
    Q_OBJECT
public:
    explicit SwarmsController(QObject *parent = 0);

    //FIXME unreliable, can lose business data
    QStringList pickNewIncomingMessagesAndClear(shared_ptr<AbstractGroupRosterEntry> group) {
        if(!group)return QStringList();
        auto swarmRE = group->asSwarmType1();
        if(!swarmRE)return QStringList();
        auto swarm = swarmRE->getSwarm();
        return swarm->pickNewIncomingMessagesAndClear(); //FIXME unreliable, can lose business data
    }

signals:

public slots:
};

#endif // SWARMSMANAGER_H
