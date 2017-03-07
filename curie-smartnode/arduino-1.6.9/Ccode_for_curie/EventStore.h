#ifndef EVENTSTORE_H
#define EVENTSTORE_H
#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
typedef enum EType
{
	input,
	close
}EType;

typedef struct event
{
	Node *node;	
	EType eventType;
	String dataType;
	int16_t *msg;
	uint8_t msgLen;

}event;

//int16_t msg[100]={0};

class EventStore{
private:
	QueueList<event> events;
	event nowEvent;
public:
	EventStore();
	~EventStore();
	void
		addEvent(QueueList<Node *> &node,EType eventType,String dataType,int16_t *msg,uint8_t msgLen),
		exec();

  event getEvent();  
  uint8_t getEventCount();
};
EventStore::EventStore(){
	events.setPrinter(Serial);
}
EventStore::~EventStore(){

}
void EventStore::addEvent(QueueList<Node *> &node,EType eventType,String dataType,int16_t *msg,uint8_t msgLen){
	event newEvent;
  for(int i=0;i<node.count();i++){
    	newEvent.node=node.at(i);
    	newEvent.eventType=eventType;
    	newEvent.dataType=dataType;
    	newEvent.msgLen = msgLen;
    	newEvent.msg = (int16_t*)calloc(msgLen,sizeof(int16_t));
    	for(int i=0;i<msgLen;i++)  
            newEvent.msg[i]=msg[i];	
    	events.push(newEvent);
	}
}
void EventStore::exec(){
	if(events.count()>0){
		nowEvent=events.pop();
		nowEvent.node->onMsg();
		free(nowEvent.msg);
	}
}
event EventStore::getEvent(){
   return nowEvent;  
}

uint8_t EventStore::getEventCount(){
	return events.count();
}

EventStore eventStore;

#endif
