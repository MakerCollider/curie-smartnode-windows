#ifndef DFINFRAREDAVOID_H
#define DFINFRAREDAVOID_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"
class DFInfraredAvoid:public Node
{
    private:
      	String name;
      	uint8_t DPin;
      	uint16_t interval;
      	QueueList<Node*> node;
      	bool is_off;
        bool debug;
        uint8_t intervalID;
        int oldOutputValue;
    public:
      	DFInfraredAvoid();
      	~DFInfraredAvoid();
      	void 
      		setUp(String name,uint8_t DPin,uint16_t interval,QueueList<Node*> &node,bool debug),
      		onMsg();
        void callBack();
};
DFInfraredAvoid::DFInfraredAvoid(){
	  is_off=true;
}
DFInfraredAvoid::~DFInfraredAvoid(){}
void DFInfraredAvoid::setUp(String name,uint8_t DPin,uint16_t interval,QueueList<Node*> &node,bool debug){
  	this->name=name;
  	this->DPin=DPin;
  	this->interval=interval;
  	this->node=node;
    this->debug = debug;
}
void DFInfraredAvoid::onMsg(){
    if (eventStore.getEvent().msg[0]==1){
      	if (is_off){            
        		intervalID=timer.setInterval(this->interval,this);
        		is_off=false;
      	} 	
    }
    if (eventStore.getEvent().msg[0]==0){
      	if (!is_off){
        		timer.clearInterval(intervalID);
        		is_off=true;
      	}
    }
}
void DFInfraredAvoid::callBack(){
    int16_t msg[1]={0};
	  int sensorValue = digitalRead(DPin);
    int output = 1;
    if (sensorValue == 1){// no body
        output = 0;
    }
    if (sensorValue == 0){ //find body
        output = 1;
    }

    if (output != oldOutputValue){
        msg[0] = output;
        eventStore.addEvent(node,input,"payload",msg,1);
        oldOutputValue = output;

        if (debug){
            Serial.print("Output Value:");
            Serial.println(output);
        }
    }
}
#endif
