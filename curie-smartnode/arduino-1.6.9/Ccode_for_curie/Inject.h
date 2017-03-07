#ifndef INJECT_H
#define INJECT_H 
#include "Node.h"
#include <QueueList.h>
#include <arduino.h>

class Inject:public Node
{
private:
	String name;
	QueueList<Node *> node;
	bool debug;
public:
	Inject();
	~Inject();
	void
	  setUp(String name,QueueList<Node*> &node,bool debug),
	  onMsg();
};
Inject::Inject(){
}
Inject::~Inject(){}
void Inject::setUp(String name,QueueList<Node*> &node,bool debug){
	this->name = name;
  	this->node = node;
  	this->debug = debug;
}
void Inject::onMsg(){
	if (debug){
	    Serial.print(name);
	    Serial.println(" start");
	}
	eventStore.addEvent(node,input,"payload",eventStore.getEvent().msg,1);
}

#endif
