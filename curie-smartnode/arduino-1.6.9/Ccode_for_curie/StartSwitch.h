#ifndef STARTSWITCH_H
#define STARTSWITCH_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"

class StartSwitch:public Node
{
    private:
      	String name;
      	QueueList<Node*> node;
        bool debug;
        int16_t output;
    public:
      	StartSwitch();
      	~StartSwitch();
      	void 
      		setUp(String name,int16_t output,QueueList<Node*> &node,bool debug);
};

StartSwitch::StartSwitch(){}
StartSwitch::~StartSwitch(){}
void StartSwitch::setUp(String name,int16_t output,QueueList<Node*> &node,bool debug){
    int16_t msg[1]={0}; 
  	this->name = name;
  	this->node = node;
    this->debug = debug;
    
    if (debug){
        Serial.print(name);
        Serial.print(" start");

        Serial.print(" output:");
        Serial.println(output);
       
    }
    msg[0] = output;
    eventStore.addEvent(node,input,"payload",msg,1);
    
}
#endif
