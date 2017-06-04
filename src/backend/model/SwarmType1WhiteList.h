/*
 * SwarmType1WhiteList.h
 *
 *  Created on: Jan 10, 2017
 *      Author: user
 */

#ifndef SRC_BACKEND_SWARMTYPE1WHITELIST_H_
#define SRC_BACKEND_SWARMTYPE1WHITELIST_H_

#include <QList>
#include <QSharedPointer>

#include <memory>

#include "SwarmParticipantLocalImage.h"

using namespace std;

class SwarmType1WhiteList  : public std::enable_shared_from_this<SwarmType1WhiteList> {
public:
	SwarmType1WhiteList(){};
	virtual ~SwarmType1WhiteList(){};

    virtual void addParticipant(shared_ptr<SwarmParticipantLocalImage> participant)=0;
    virtual void removeParticipant(shared_ptr<SwarmParticipantLocalImage> participant)=0;

    virtual std::shared_ptr<QList<std::shared_ptr<SwarmParticipantLocalImage>>> getParticipantsListClone()=0;
    virtual void deallocParticipantsListClone(std::shared_ptr<QList<std::shared_ptr<SwarmParticipantLocalImage>>>)=0;
};

#endif /* SRC_BACKEND_SWARMTYPE1WHITELIST_H_ */
