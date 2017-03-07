#ifndef IRDISTANCE_H
#define IRDISTANCE_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"

class IRDistance:public Node
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
      	IRDistance();
      	~IRDistance();
      	void 
      		setUp(String name,uint16_t APin,uint16_t interval,QueueList<Node*> &node,bool debug),
      		onMsg();
         void callBack();
};

IRDistance::IRDistance(){
	  is_off = true;
}
IRDistance::~IRDistance()
{
  timer.clearInterval(intervalID);
  }
void IRDistance::setUp(String name,uint16_t APin,uint16_t interval,QueueList<Node*> &node,bool debug){
  	this->name = name;
  	this->APin = APin;
  	this->interval = interval;
  	this->node = node;
    this->debug = debug;
}
void IRDistance::onMsg(){
    if(debug)
    {
        Serial.print(name);
        Serial.println(" start");
    }
    
    if (eventStore.getEvent().msg[0] == 1){
      	if (is_off){
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
void IRDistance::callBack(){
    int16_t msg[1]={0};
    double sensorValue = analogRead(APin);
    float distant = sensorValue*5/1024;
    distant = 16.297*pow(distant,-0.993)+0.5;
    msg[0] = distant;
    eventStore.addEvent(node,input,"payload",msg,1);
    

    if (debug){
        Serial.print("Current distant is ");
        Serial.println(distant);
        Serial.println(sensorValue);
    }
}
#endif
