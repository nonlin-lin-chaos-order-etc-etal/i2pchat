#include "RosterController.h"

#include "../backend/Roster.h"

RosterController::RosterController(Roster & rosterModel_):rosterModel(rosterModel_) {}

Roster & RosterController::getRosterModel() { return rosterModel; }

void RosterController::removeActorByI2PDestination(QString I2PDestination) {
    rosterModel.removeActorByI2PDestination(I2PDestination);
}

QStringList RosterController::getNewMessages(shared_ptr<AbstractGroupRosterEntry> group) {
    return appCtx->getSwarmsManager()->pickNewIncomingMessagesAndClear(group);//FIXME unreliable, can lose business data
}
