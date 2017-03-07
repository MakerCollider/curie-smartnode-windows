#ifndef DELAY_H
#define DELAY_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
class Delay:public Node
{
	private:
		String name;
	  	QueueList<Node *> node;
	  	uint16_t interval; 
	  	uint8_t count;
	  	bool is_off;
        uint8_t intervalID;
        bool debug;
        uint16_t inputValue; 

	public:
		Delay();
		~Delay();
		void
	       setUp(String name,uint16_t interval,QueueList<Node *> &node,bool debug),
	       onMsg();
	       void callBack();
	
};
Delay::Delay(){
	is_off=true;
	count = 0;
}
Delay::~Delay(){
	timer.clearInterval(intervalID);
}
void Delay::setUp(String name,uint16_t interval,QueueList<Node *> &node,bool debug){
	this->name = name;
    this->node = node;
    this->debug = debug;
    this->interval = interval;
}
void Delay::onMsg(){
	if (debug){
        Serial.print(name);
        Serial.println(" start");
    }

    if (is_off){
        inputValue = eventStore.getEvent().msg[0];
        if (debug){
            Serial.print(" inputValue:");
            Serial.println(inputValue);
        }

        intervalID=timer.setInterval(1000,this);
        is_off=false;
        count = 0;
    }   
}

void Delay::callBack(){
    int16_t msg[1]={0};
	   count++;

    if (debug){
    	Serial.print("delay:");
        Serial.print(count);
        Serial.println("s");
    }

    if (count>=interval){
        if (!is_off){
        	msg[0] = inputValue;
            eventStore.addEvent(node,input,"payload",msg,1);
            timer.clearInterval(intervalID);
            is_off=true;
        }
    }
}
#endif
