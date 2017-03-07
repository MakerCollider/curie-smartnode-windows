#ifndef CHANGE_H
#define CHANGE_H
#include "Node.h"
#include "EventStore.h"
#include <arduino.h>
#include <QueueList.h>
class Change:public Node
{
private:
	String name;
	String mode;
	String changeto;
  	QueueList<Node *> node;
  	bool debug;
public:
	Change();
	~Change();
	void
		setUp(String name,String mode,String changeto,QueueList<Node *> &node,bool debug),
		onMsg();	
};
Change::Change(){}
Change::~Change(){}
void Change::setUp(String name,String mode,String changeto,QueueList<Node *> &node,bool debug){
	this->name=name;
	this->mode=mode;
	this->changeto=changeto;
  	this->node=node;
  	this->debug=debug;
}
void Change::onMsg(){
	int16_t msg[1]={0};
	if (debug){
	    Serial.print(name);
	    Serial.print(" start");
	}
	
	if (mode=="setstr"){
		msg[0]=changeto.toInt();
		if (debug){
		    Serial.print(" value:");
		    Serial.println(msg[0]);
	    }
        eventStore.addEvent(node,input,"payload",msg,1);
       
	}
	if (mode=="setmsg"){
		if (debug){
			Serial.print(" value:");
		    Serial.println(eventStore.getEvent().msg[0]);
	    }
		eventStore.addEvent(node,input,changeto,eventStore.getEvent().msg,1);
	}
}
#endif
