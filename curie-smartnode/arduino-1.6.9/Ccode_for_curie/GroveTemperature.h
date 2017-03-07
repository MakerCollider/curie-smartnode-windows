// Demo code for Grove - Temperature Sensor V1.1/1.2
// Loovee @ 2015-8-26

#ifndef GROVETEMPERATURE_H
#define GROVETEMPERATURE_H
extern "C" {
	#include <stdint.h>
}
#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"

class GroveTemperature:public Node
{
private:
	String name;
	uint16_t interval;
	uint8_t APin;
	QueueList<Node *> node;
  bool is_off;
  uint8_t intervalID;
  bool debug;
public:
    GroveTemperature();
    ~GroveTemperature();
    void 
    	setUp(String name,uint8_t APin,uint16_t interval,QueueList<Node *> &node,bool debug),
    	onMsg();
    void callBack();
};
GroveTemperature::GroveTemperature(){  
  is_off=true;
}
GroveTemperature::~GroveTemperature(){  }
void GroveTemperature::setUp(String name,uint8_t APin,uint16_t interval,QueueList<Node *> &node,bool debug){
  	this->name=name;
  	this->interval=interval;
  	this->APin=APin;
    this->node=node;
    this->debug = debug;
}
void GroveTemperature::onMsg(){
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
            is_off=true;
        }
    }
}
void GroveTemperature::callBack(){
    int16_t msg[1]={0};
    int B = 4275;                 // B value of the thermistor
    int sensorValue = analogRead(APin);
    float R = 1023.0/((float)sensorValue)-1.0;
    R = 100000.0*R;
    float temperature=1.0/(log(R/100000.0)/B+1/298.15)-273.15;//convert to temperature via datasheet ;

    msg[0] = temperature;
    eventStore.addEvent(node,input,"payload",msg,1);
    

    if (debug){
        Serial.print("Current temperature is ");
        Serial.print(temperature);
        Serial.println(" degrees Celsius");
    }
}
#endif
