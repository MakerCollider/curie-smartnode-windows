#ifndef PWMLED_H
#define PWMLED_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
class PWMLed:public Node
{
    private:
    	String name;
      uint8_t preValue = 0 ;
      uint8_t currentValue ;
    	uint8_t PWMPin;
      	QueueList<Node *> node;
        bool debug;
    public:
    	PWMLed();
    	~PWMLed();
    	void setUp(String name,uint8_t PWMPin,QueueList<Node *> &node,bool debug);
      void fadeOn(unsigned int currentValue);
      void fadeOff(unsigned int currentValue);
      void onMsg();
	
};
PWMLed::PWMLed(){}
PWMLed::~PWMLed(){}
void PWMLed::setUp(String name,uint8_t PWMPin,QueueList<Node *> &node,bool debug){
  	this->name=name;
  	this->PWMPin=PWMPin;
    this->node=node;
    this->debug=debug;
  	pinMode(PWMPin,OUTPUT);
}
void PWMLed::onMsg(){
    if (debug){
        Serial.print(name);
        Serial.println(" start");
    }
    currentValue = eventStore.getEvent().msg[0];
    if (currentValue>preValue){
        fadeOn(currentValue);
    }
    else if(currentValue<=preValue){
        fadeOff(currentValue);
    }
    preValue = currentValue;
}
void PWMLed::fadeOn(unsigned int currentValue){
        for (byte value = preValue ; value <currentValue ; value++){ 
            analogWrite(PWMPin, value);
        } 
}

void PWMLed::fadeOff(unsigned int currentValue){
        for (byte value = preValue; value > currentValue ; value--){ 
            analogWrite(PWMPin, value); 
        }
}
#endif
