#ifndef SDWRITE_H
#define SDWRITE_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include <SPI.h>
#include <SD.h>


class SDWrite:public Node{
    private:
        String name;
        String filename;
        QueueList<Node*>node; //存储节点
        File myFile;
        bool debug;
    public:
        SDWrite();
        ~SDWrite();
        void 
            setUp(String name,String filename, QueueList<Node*> &node,bool debug),
            onMsg();
};
SDWrite::SDWrite(){}
SDWrite::~SDWrite(){}

void SDWrite::setUp(String name,String filename, QueueList<Node*> &node,bool debug){
    this->name = name;
    this->filename = filename;
    this->node = node;
    this->debug = debug;

    if (!SD.begin(4)) {
        Serial.println("initialization failed!");
        return;
    }
}

void SDWrite::onMsg(){
    
    if (debug){
        Serial.print(name);
    	Serial.println(" Start!");
    }

    int len = eventStore.getEvent().msgLen;
    if(filename =="")
    {
        filename = "test.csv";
    }
    myFile = SD.open(filename, FILE_WRITE);

    if(myFile)
    {
        for (int i = 0; i < len; i++)
        {
            if(i == 0){
                myFile.print(eventStore.getEvent().msg[i]);
                
            }
            else{
                myFile.print(",");
                myFile.print(eventStore.getEvent().msg[i]);
            }
            if(debug)
            {
            Serial.print(eventStore.getEvent().msg[i]);
            Serial.println();
            }
        }
        myFile.print("\n");
    }
    myFile.close(); 
}

#endif
