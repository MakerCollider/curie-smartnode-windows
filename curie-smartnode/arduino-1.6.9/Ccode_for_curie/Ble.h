#ifndef BLE_H
#define BLE_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "CurieBLE.h"
#include "Timer.h"
  
BLEPeripheral blePeripheral;  // 给BLE外围设备命名（建立对象）
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service可使用BLE服务创建由BLE设备显示的服务
// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEUnsignedCharCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
        
class Ble:public Node
{
    private:
        String name;
        QueueList<Node*>node; 
        uint8_t intervalID;
        uint16_t interval;
        uint8_t getValue;
        bool debug;
    public:
      Ble();
      ~Ble();
      void setUp(String name,QueueList<Node*> &node, bool debug);
      void callBack();
};
Ble::Ble(){ }
Ble::~Ble(){timer.clearInterval(intervalID);
}
void Ble::setUp(String name,QueueList<Node*> &node, bool debug)
{
    this->name = name;
    this->node = node;
    this->interval = 500;
    this->debug = debug;
    intervalID = timer.setInterval(this->interval,this);
  // set advertised local name and service UUID:
    const char* c_s = name.c_str();
    blePeripheral.setLocalName(c_s);
    blePeripheral.setAdvertisedServiceUuid(ledService.uuid());

    // add service and characteristic:
    blePeripheral.addAttribute(ledService);
    blePeripheral.addAttribute(switchCharacteristic);
    // set the initial value for the characeristic:
    switchCharacteristic.setValue(0);
    if (debug){
       Serial.println("setup s11");
       Serial.println("callback s11");
      // begin advertising BLE service:
       Serial.println("BLE LED Peripheral");
    }
        blePeripheral.begin();
}

void Ble::callBack()
{  
    int16_t msg[1]={0};
    BLECentral central = blePeripheral.central();
    // if a central is connected to peripheral:
    if (central) {
	    if (debug){
		    Serial.print("Connected to central: ");
		    // print the central's MAC address:
		    Serial.println(central.address());
	    }

        // if the remote device wrote to the characteristic,
        // use the value to control the LED:
        if (switchCharacteristic.written()) {
            getValue = switchCharacteristic.value();
            msg[0] = getValue;
            eventStore.addEvent(node,input,"payload",msg,1);  
            
        }

        // when the central disconnects, print it out:
        if (debug){ 
		    Serial.print("Disconnected from central: ");
		    Serial.println(central.address());
		}
    }

}
#endif

