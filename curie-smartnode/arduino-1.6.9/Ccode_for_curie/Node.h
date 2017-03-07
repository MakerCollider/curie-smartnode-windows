#ifndef NODE_H
#define NODE_H
#define DEBUG
class Node{
 public: 
    virtual void onMsg();  
    virtual void callBack(){};
};
void Node::onMsg(){
  Serial.println("you do not define \"onMsg\" of your Node");
}
#endif
