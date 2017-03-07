#ifndef DFTEMPERATURE_H
#define DFTEMPERATURE_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"

class DFTemperature:public Node
{
    private:
      	String name;
      	uint16_t APin;
      	uint16_t interval;
      	QueueList<Node*> node;
      	bool is_off;
        bool debug;
        uint8_t intervalID;
    public:
      	DFTemperature();
      	~DFTemperature();
      	void 
      		setUp(String name,uint16_t APin,uint16_t interval,QueueList<Node*> &node,bool debug),
      		onMsg();
         void callBack();
};

DFTemperature::DFTemperature(){
	  is_off = true;
}
DFTemperature::~DFTemperature()
{
  timer.clearInterval(intervalID);
  }
void DFTemperature::setUp(String name,uint16_t APin,uint16_t interval,QueueList<Node*> &node,bool debug){
  	this->name = name;
  	this->APin = APin;
  	this->interval = interval;
  	this->node = node;
    this->debug = debug;
}
void DFTemperature::onMsg(){
if(debug)
{
    Serial.print(name);
    Serial.println(" start");
}
    if (eventStore.getEvent().msg[0] == 1){
      	if (is_off){
            //parameter[0] = APin;
        		intervalID = timer.setInterval(this->interval,this);
        		is_off = false;
      	} 	
    }
    if (eventStore.getEvent().msg[0] == 0){
      	if (!is_off){
        		timer.clearInterval(intervalID);
        		is_off = true;
      	}
    }
}
void DFTemperature::callBack(){
    int16_t msg[1]={0};
    double sensorValue = analogRead(APin);
	  double temperature = sensorValue * (3/10.24);
    
    msg[0] = temperature;
    eventStore.addEvent(node,input,"payload",msg,1);
    

    if (debug){
        Serial.print("Current temperature is ");
        Serial.print(temperature);
        Serial.println(" degrees Celsius");
    }
}
#endif
