#ifndef DEBUG_H
#define DEBUG_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
class Debug:public Node
{
	private:
		 String name;
	  	QueueList<Node *> node;
	public:
		Debug();
		~Debug();
		void
	       setUp(String name,QueueList<Node *> &node),
	       onMsg();
	
};
Debug::Debug(){}
Debug::~Debug(){}
void Debug::setUp(String name,QueueList<Node *> &node){
	this->name = name;
    this->node = node;
}
void Debug::onMsg(){}
#endif
