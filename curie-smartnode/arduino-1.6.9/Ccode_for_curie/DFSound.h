#ifndef DFSOUND_H
#define DFSOUND_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"

class DFSound:public Node
{
    private:
      	String name;
      	int8_t APin;
      	uint16_t interval;
      	QueueList<Node*> node;
      	bool is_off;
        bool debug;
        uint8_t intervalID;
        int oldSensorValue;
    public:
    	DFSound();
    	~DFSound();
    	void 
    		setUp(String name,int8_t APin,uint16_t interval,QueueList<Node*> &node,bool debug),
    		onMsg();
      void callBack();
};
DFSound::DFSound(){
	  is_off=true;
}
DFSound::~DFSound(){
    timer.clearInterval(intervalID);
}
void DFSound::setUp(String name,int8_t APin,uint16_t interval,QueueList<Node*> &node,bool debug){
  	this->name = name;
  	this->APin = APin;
  	this->interval = interval;
  	this->node = node;
    this->debug = debug;
}
void DFSound::onMsg(){
    if (debug){
        Serial.print(name);
        Serial.println(" start");
    }

    if (eventStore.getEvent().msg[0]==1){
      	if (is_off){
        		intervalID=timer.setInterval(this->interval,this);
        		is_off=false;
      	} 	
    }
    if (eventStore.getEvent().msg[0]==0){
      	if (!is_off){
        		timer.clearInterval(intervalID);
        		is_off = true;
      	}
    }
}
void DFSound::callBack(){
    int16_t msg[1]={0};
	  int sensorValue = analogRead(APin);
    if (sensorValue != oldSensorValue){
  	    msg[0] = sensorValue;
  	    eventStore.addEvent(node,input,"payload",msg,1);
        oldSensorValue = sensorValue;
  	

        if (debug){
            Serial.print("Sound Analog Value:");
            Serial.println(sensorValue);
        }
   }
}
#endif
