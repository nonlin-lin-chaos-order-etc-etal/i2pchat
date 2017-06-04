#include "RosterEntryFactory.h"

#include "UserManager.h"
#include <QString>
#include <QTextStream>

#include "AbstractRosterEntry.h"
#include "OneToOneRosterEntry.h"
#include "ParseErrorsUtil.h"
#include "SwarmType1RosterEntry.h"

RosterEntryFactory::RosterEntryFactory()
{}

AbstractRosterEntry* RosterEntryFactory::loadRosterEntry(QString& classTag, QTextStream& input,
                                                         UserManager & cum, UnsentChatMessagesStorage& mUnsentMessageStorage,
                                                         AppContext & chatCore, parsed_item_file parsedItem) {
    if(classTag == ActorRosterEntry_CLASS_TAG) {
        return OneToOneRosterEntry::loadRosterEntry(input, cum, mUnsentMessageStorage, parsedItem);
    }
    if(classTag == SwarmType1RosterEntry_CLASS_TAG) {
        return SwarmType1RosterEntry::loadRosterEntry(input, mUnsentMessageStorage, cum, chatCore);
    }

    return nullptr;
}

AbstractRosterEntry* RosterEntryFactory::createRosterEntryForOldStyleUser(AbstractUserLocalImage&user, UserManager & cum, UnsentChatMessagesStorage& mUnsentMessageStorage) {
    return OneToOneRosterEntry::createRosterEntryForOldStyleUser(user, cum, mUnsentMessageStorage);
}

SwarmType1RosterEntry* RosterEntryFactory::createRosterEntryForSwarmType1(SwarmType1LocalImage&swarm,AppContext&chatCore) {
    return SwarmType1RosterEntry::createRosterEntryForSwarmType1(swarm,chatCore);
}
