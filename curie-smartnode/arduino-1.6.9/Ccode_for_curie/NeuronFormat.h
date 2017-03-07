#ifndef NEURONFORMAT_H
#define NEURONFORMAT_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"

class NeuronFormat:public Node
{
    private:
        String name;
        uint8_t APin;
        int16_t pattens[100];
        uint8_t count;
        QueueList<Node*> node;
        uint8_t neuron_status;
        uint8_t category;
        uint8_t dimension;
        uint8_t dataLength;
        uint8_t status;
        bool debug;

    public:
        NeuronFormat();
        ~NeuronFormat(); 
        void setUp(String name,uint8_t dimension,uint8_t dataLength,QueueList<Node*> &node,bool debug);
        void onMsg();

};

NeuronFormat::NeuronFormat(){}
NeuronFormat::~NeuronFormat(){}

void NeuronFormat::setUp(String name,uint8_t dimension,uint8_t dataLength,QueueList<Node *> &node,bool debug){
    this->name = name;
    this->node = node;
    this->neuron_status = -1;
    this->category = 0;
    this->dimension = dimension;
    this->dataLength = dataLength;
    this->count = 0;
    this->status = 0;
    this->debug = debug;
}

void NeuronFormat::onMsg(){
    int16_t msg[3]={0};
    if (eventStore.getEvent().dataType == "neuronStatus"){
        //learn or Classify
        if (eventStore.getEvent().msg[0] == 1){
           neuron_status = 1;
        }
        else if(eventStore.getEvent().msg[0] == 0){
           neuron_status = 0;
        }

        if (debug){
            Serial.print(" neuronStatus:");
            Serial.println(neuron_status);
        }
    }
    else if (eventStore.getEvent().dataType == "category"){
        //pattern catId
        category = eventStore.getEvent().msg[0];
        if  (category == 0){
             status = 0;
             count = 0;
        }

        if (debug){
            Serial.print(" category:");
            Serial.println(category);
        }
    }
    else if (eventStore.getEvent().dataType == "payload"){ //patterns
        if  (category > 0){
            if (count<dataLength){
                pattens[count] = map(eventStore.getEvent().msg[0],-255,255,0,255);
                pattens[count+1] = map(eventStore.getEvent().msg[1],-255,255,0,255);
                pattens[count+2] = map(eventStore.getEvent().msg[2],-255,255,0,255);
                count = count+3;
            }
            else if (count==dataLength){
                if (status == 0){
                    status = 1;

                    msg[0] = neuron_status; //learn Classify
                    msg[1] = dataLength; //length


                    if (neuron_status == 1){ // learn
                        
                        msg[2] = category; //catId
                        for(int i = 0;i<count;i++){
                            msg[i+3] = pattens[i];
                        }

                        if (debug){
                            Serial.print(" learn payload:");
                            for(int i = 0;i<dataLength+3;i++){
                                Serial.print(msg[i]);
                                Serial.print(" ");
                            }
                            Serial.println(); 
                        }

                        eventStore.addEvent(node,input,"payload",msg,3);
                    }
                    else if(neuron_status == 0){ //Classify
                        for(int i = 0;i<count;i++){
                            msg[i+2] = pattens[i];
                        }

                        if (debug){
                            Serial.print(" Classify payload:");
                            for(int i = 0;i<dataLength+2;i++){
                                Serial.print(msg[i]);
                                Serial.print(" ");
                            }
                            Serial.println(); 
                        }
                        eventStore.addEvent(node,input,"payload",msg,3);
                    }

                    //init
                    // msg[0] = 0;
                    // msg[1] = 0;
                    // msg[2] = 0; 

                    for(int i = 0;i<count;i++){
                        msg[i+3] = 0;
                    }
                        
                    for(int i = 0;i<dataLength;i++){
                        pattens[i] = 0;
                    }
                }
            }
        }//    
        
    }
}
#endif




