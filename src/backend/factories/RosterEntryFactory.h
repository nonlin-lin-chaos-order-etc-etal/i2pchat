#ifndef ROSTERENTRYFACTORY_H
#define ROSTERENTRYFACTORY_H

#include "AbstractRosterEntry.h"
#include "AppContext.h"
#include "ParseErrorsUtil.h"
#include "SwarmType1LocalImage.h"
#include "SwarmType1RosterEntry.h"

class AbstractUserLocalImage;
class UserManager;
class UnsentChatMessagesStorage;

class RosterEntryFactory
{
public:
    static AbstractRosterEntry* loadRosterEntry(QString& classTag, QTextStream& input,
                                                UserManager & cum, UnsentChatMessagesStorage& mUnsentMessageStorage,
                                                AppContext & chatCore, parsed_item_file parsedItem);
    static AbstractRosterEntry* createRosterEntryForOldStyleUser(AbstractUserLocalImage&user, UserManager & cum, UnsentChatMessagesStorage& mUnsentMessageStorage);
    static SwarmType1RosterEntry* createRosterEntryForSwarmType1(SwarmType1LocalImage&swarm,AppContext&chatCore);

private:
    RosterEntryFactory();
};

#endif // ROSTERENTRYFACTORY_H
