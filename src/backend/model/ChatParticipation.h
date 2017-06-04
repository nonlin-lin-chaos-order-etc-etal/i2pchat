/*
 * ChatParticipation.h
 *
 *  Created on: Jan 14, 2017
 *      Author: user
 */

#ifndef SRC_BACKEND_CHATPARTICIPATION_H_
#define SRC_BACKEND_CHATPARTICIPATION_H_

#include <QColor>
#include <QFont>

#include <memory>

class ChatParticipation : public std::enable_shared_from_this<ChatParticipation> {
public:
    //forbid some operators
    ChatParticipation(const ChatParticipation&)=delete;
    ChatParticipation& operator=(const ChatParticipation&)=delete;
    
    class MessageToolbarParameters : public std::enable_shared_from_this<MessageToolbarParameters> {
	public:
	
    	//forbid some operators
	    MessageToolbarParameters(const MessageToolbarParameters&)=delete;
    	MessageToolbarParameters& operator=(const MessageToolbarParameters&)=delete;
    
		MessageToolbarParameters(){};
		virtual ~MessageToolbarParameters(){};
	    virtual QFont& getTextFont()=0;
	    virtual QColor& getTextColor()=0;
	    virtual void setTextFont(QFont)=0;
	    virtual void setTextColor(QColor)=0;
	};

	ChatParticipation(){};
	virtual ~ChatParticipation(){};
    virtual void setDefaultMessageToolbarParameters(shared_ptr<MessageToolbarParameters>)=0;
    virtual shared_ptr<MessageToolbarParameters> getDefaultMessageToolbarParameters()=0;
};

#endif /* SRC_BACKEND_CHATPARTICIPATION_H_ */
