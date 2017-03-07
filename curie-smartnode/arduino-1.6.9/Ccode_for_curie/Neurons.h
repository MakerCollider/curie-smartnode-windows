#ifndef NEURONS_H
#define NEURONS_H

#include <arduino.h>
#include <CurieNeurons.h>
#include "Node.h"
#include "EventStore.h"
#include "QueueList.h"

class Neurons:public Node
{
private:
	CurieNeurons hNN;
	String name;
	uint8_t funType;
	uint8_t dataLength;
	QueueList<Node*> node;
	uint8_t neuron_status;
	uint8_t category;
	uint8_t category_status;
	int16_t pattens[100];
	int16_t collectData[300];
	uint8_t cn;
	uint8_t groupElements;
	uint8_t msgLenCounter;
	uint8_t channel;
	bool flagEnd;
	bool debug;
	int16_t msg[100];
public:
	Neurons();
	~Neurons();
	void 
	setUp(String name,uint8_t funType,uint8_t channel,QueueList<Node*> &node,bool debug),
	onMsg();

	
};
Neurons::Neurons(){
   hNN.Init();
   hNN.Forget();
   flagEnd = false;
   dataLength = 60;
}
Neurons::~Neurons(){}
void Neurons::setUp(String name,uint8_t funType,uint8_t channel,QueueList<Node*> &node,bool debug){
	this->name = name;
	this->funType = funType;
	this->node = node;
    this->neuron_status = -1;
    this->category_status = 0;
    this->channel = channel;
	this->debug = debug;
}
void Neurons::onMsg(){
	
    if (funType == 0){ //common
		if (debug){
		    Serial.print(name);
		    Serial.println(" start");
		}

		if (eventStore.getEvent().msg[0]==1){
			
			int length = eventStore.getEvent().msg[1];
			int catL = eventStore.getEvent().msg[2];
		    Serial.print(name);
		    Serial.println(" start learning");
			hNN.Learn((unsigned char*)&(eventStore.getEvent().msg[3]),length,catL);
	        Serial.println("Learn Finish!");
	        
		}
		else if(eventStore.getEvent().msg[0]==0){
			
			int length = eventStore.getEvent().msg[1];
			int dist,cat,nid;
			hNN.Classify((unsigned char*)&(eventStore.getEvent().msg[2]),length,&dist,&cat,&nid);

			cat &=0x7FFF;

	        if (cat!=0x7FFF){
	            Serial.print("cat:");
	            Serial.print(cat);
	            Serial.print(" ");
	            Serial.print("nid:");
	            Serial.print(nid);
	            Serial.print(" ");
	            Serial.print("dist:");
	            Serial.println(dist);
	            msg[0]=cat;msg[1]=nid;msg[2]=dist;
	            eventStore.addEvent(node,input,"payload",msg,3);
	        }
	        else {
	        	Serial.println("Pattern unknown");
	        }
	        
		}
		else{
			Serial.println("message format is wrong");
		}
    }
    if (funType == 1){ //advanced
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
	    if (eventStore.getEvent().dataType == "category"){
	        //pattern catId
	        category_status = eventStore.getEvent().msg[0];

	        if (debug){
	            Serial.print(" category_status:");
	            Serial.println(category_status);
	        }
              
            if (category_status==0){
            	/*
                Serial.print(" collect:");
	            for(int i = 0;i<cn;i++){
	                Serial.print(collectData[i]);
	                Serial.print(" ");
	            }
	            Serial.println();
	            */
	            
                
                if (cn < dataLength){
                	for(int i = 0;i<cn;i++){
	                    collectData[i] = 0;
	                }
	                cn = 0;
                	Serial.print(" Pattens data unmet:");
                    return; 
                }

                for(int i = 0;i<dataLength;i++){
	                pattens[i] = collectData[i];
	            }

                
            	//int groupLength = dataLength/groupElements;
	        	//int step = floor((cn/groupElements)/groupLength)-1;
                

                //----------------------------
                for (int i = 0; i < dataLength; i++){
	            	pattens[i] = map(pattens[i],-255,255,0,255);
	            }

	            msg[0] = neuron_status; //learn Classify
	            msg[1] = dataLength; //length

	            if (neuron_status == 1){ // learn   
	                msg[2] = category; //catId
	                for(int i = 0;i<dataLength;i++){
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

	                //----------------------
	                
	                int length = msg[1];
					int catL = msg[2];
					Serial.print(name);
					Serial.println(" start learning");
					hNN.Learn((unsigned char*)&(msg[3]),length,catL);
					Serial.println("Learn Finish!");
					
					
	                //------------------------
	            }
	            else if(neuron_status == 0){ //Classify
	                for(int i = 0;i<dataLength;i++){
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

	                //------------------------------------
	                
	                int length = msg[1];
					int dist,cat,nid;
					hNN.Classify((unsigned char*)&(msg[2]),length,&dist,&cat,&nid);

					cat &=0x7FFF;

					if (cat!=0x7FFF){
					    Serial.print("cat:");
					    Serial.print(cat);
					    Serial.print(" ");
					    Serial.print("nid:");
					    Serial.print(nid);
					    Serial.print(" ");
					    Serial.print("dist:");
					    Serial.println(dist);
					    msg[0]=cat;msg[1]=nid;msg[2]=dist;
					    eventStore.addEvent(node,input,"payload",msg,3);
					}
					else {
					    Serial.println("Pattern unknown");
					}
	                //------------------------------------
	            }


                //clear
	            for(int i = 0;i<dataLength;i++){
	                pattens[i] = 0;
	            }

	            for(int i = 0;i<cn;i++){
	                collectData[i] = 0;
	            }
	            for(int i = 0;i<100;i++){
	                msg[i] = 0;
	            }
	        	cn = 0;
            }
            else{
            	category = category_status;
            }

	    }
	    if (eventStore.getEvent().dataType == "payload"){ //patterns
	        if  (category_status > 0){
	            uint8_t msgLen = eventStore.getEvent().msgLen;
	            if (msgLenCounter < channel && flagEnd == false){
	                groupElements += msgLen;
	                msgLenCounter++;
	            }
	            else{
	            	msgLenCounter = 0;
	            	flagEnd = true;
	            }

	            for (int i = 0; i < msgLen; i++){
	            	collectData[cn] = eventStore.getEvent().msg[i];
	            	cn++;
	            }
	        }// end if category>0
	    }
    }


}


#endif

