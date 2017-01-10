#include "rostercontroller.h"
#include "rostermodel.h"

RosterController::RosterController(RosterModel & rosterModel_):rosterModel(rosterModel_) {}

RosterModel & RosterController::getRosterModel() { return rosterModel; }

void RosterController::removeActorByI2PDestination(QString I2PDestination) {
    rosterModel.removeActorByI2PDestination(I2PDestination);
}

