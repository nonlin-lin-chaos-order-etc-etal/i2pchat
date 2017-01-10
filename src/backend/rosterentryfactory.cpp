#include "rosterentryfactory.h"
#include "abstractrosterentry.h"
#include "actorrosterentry.h"
#include "UserManager.h"
#include "swarmtype1rosterentry.h"

#include <QString>
#include <QTextStream>

RosterEntryFactory::RosterEntryFactory()
{}

AbstractRosterEntry* RosterEntryFactory::loadRosterEntry(QString& classTag, QTextStream& input,
                                                         CUserManager & cum, CUnsentChatMessageStorage& mUnsentMessageStorage,
                                                         CCore & chatCore) {
    if(classTag == ActorRosterEntry_CLASS_TAG) {
        return ActorRosterEntry::loadRosterEntry(input, cum, mUnsentMessageStorage);
    }
    if(classTag == SwarmType1RosterEntry_CLASS_TAG) {
        return SwarmType1RosterEntry::loadRosterEntry(input, mUnsentMessageStorage, cum, chatCore);
    }

    return nullptr;
}

AbstractRosterEntry* RosterEntryFactory::createRosterEntryForOldStyleUser(CUser&user, CUserManager & cum, CUnsentChatMessageStorage& mUnsentMessageStorage) {
    return ActorRosterEntry::createRosterEntryForOldStyleUser(user, cum, mUnsentMessageStorage);
}

SwarmType1RosterEntry* RosterEntryFactory::createRosterEntryForSwarmType1(SwarmType1&swarm,CCore&chatCore) {
    return SwarmType1RosterEntry::createRosterEntryForSwarmType1(swarm,chatCore);
}
