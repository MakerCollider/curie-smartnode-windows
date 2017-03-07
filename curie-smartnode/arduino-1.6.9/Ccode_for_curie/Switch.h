#ifndef SWITCH_H
#define SWITCH_H
#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
class Switch:public Node
{
private:
	String name;
  	uint8_t outputs;
	String operate[2];//这里应该提前知道到底有几个输出;后期使用QueueList解决
	int16_t value[2];
	QueueList<QueueList<Node *> > node;
	bool debug;
public:
	Switch();
	~Switch();
	void
		setUp(String name,uint8_t outputs,String operate[2],int16_t value[2],QueueList<QueueList<Node *>> &node,bool debug),
		onMsg();
	
};
Switch::Switch(){}
Switch::~Switch(){}
void Switch::setUp(String name,uint8_t outputs,String operate[2],int16_t value[2],QueueList<QueueList<Node *>> &node,bool debug){
	this->name=name;
	this->outputs=outputs;
	this->node=node;
	this->debug=debug;
	for(uint8_t i=0;i<outputs;i++){
	  	this->operate[i] = operate[i];
	  	this->value[i] = value[i];
	}
}

void Switch::onMsg(){
    if (debug){
	    Serial.print(name);
	    Serial.print(" start");
    }

	for(uint8_t i=0;i<outputs;i++){
		if(operate[i]=="lt"){
			if (eventStore.getEvent().msg[0]<value[i]){
				if (debug){
				    Serial.print(" value:");
				    Serial.println(eventStore.getEvent().msg[0]);
			    }
				eventStore.addEvent(node.at(i),input,"payload",eventStore.getEvent().msg,1);
			}
		}
		if(operate[i]=="gt"){
			if (eventStore.getEvent().msg[0]>value[i]){
				if (debug){
				    Serial.print(" value:");
				    Serial.println(eventStore.getEvent().msg[0]);
			    }
				eventStore.addEvent(node.at(i),input,"payload",eventStore.getEvent().msg,1);
			}
		}
		if(operate[i]=="eq"){
			if (eventStore.getEvent().msg[0]==value[i]){
				if (debug){
				    Serial.print(" value:");
				    Serial.println(eventStore.getEvent().msg[0]);
			    }
				eventStore.addEvent(node.at(i),input,"payload",eventStore.getEvent().msg,1);
			}
		}
	}	

}
#endif
