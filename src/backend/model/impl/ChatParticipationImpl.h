/*
 * ChatParticipation.h
 *
 *  Created on: Jan 14, 2017
 *      Author: user
 */

#ifndef SRC_BACKEND_CHATPARTICIPATION_IMPL_H_
#define SRC_BACKEND_CHATPARTICIPATION_IMPL_H_

#include <QColor>
#include <QFont>

class ChatParticipation {
	class MessageToolbarParameters {
		MessageToolbarParameters(){}
		virtual ~MessageToolbarParameters(){}
	    virtual QFont& getTextFont()=0;
	    virtual QColor& getTextColor()=0;
	    virtual void setTextFont(QFont)=0;
	    virtual void setTextColor(QColor)=0;
	};

	ChatParticipation(){}
	virtual ~ChatParticipation(){}
	virtual void setDefaultMessageToolbarParameters(MessageToolbarParameters)=0;
	virtual MessageToolbarParameters getDefaultMessageToolbarParameters()=0;
};

#endif /* SRC_BACKEND_CHATPARTICIPATION_IMPL_H_ */
