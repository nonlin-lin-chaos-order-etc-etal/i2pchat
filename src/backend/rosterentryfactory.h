#ifndef ROSTERENTRYFACTORY_H
#define ROSTERENTRYFACTORY_H

#include "abstractrosterentry.h"
#include "swarmtype1rosterentry.h"
#include "SwarmType1.h"
#include "Core.h"

class CUser;
class CUserManager;
class CUnsentChatMessageStorage;

class RosterEntryFactory
{
public:
    static AbstractRosterEntry* loadRosterEntry(QString& classTag, QTextStream& input,
                                                CUserManager & cum, CUnsentChatMessageStorage& mUnsentMessageStorage,
                                                CCore & chatCore);
    static AbstractRosterEntry* createRosterEntryForOldStyleUser(CUser&user, CUserManager & cum, CUnsentChatMessageStorage& mUnsentMessageStorage);
    static SwarmType1RosterEntry* createRosterEntryForSwarmType1(SwarmType1&swarm,CCore&chatCore);

private:
    RosterEntryFactory();
};

#endif // ROSTERENTRYFACTORY_H
