
#include <CurieBLE.h>
#include "BLESerial.h"

extern volatile uint32_t gu32RxSize;

uint8_t bleRxTail=0,bleRxHead=0;
uint8_t bleRxBuf[81];

BLEPeripheral blePeripheral;
BLECharacteristic serialCharact("DFB1",BLERead | BLEWrite | BLEWriteWithoutResponse | BLENotify,20);
  BLEService blunoService("DFB0");
BLEService deviceinfoService("180A"); 
BLECharacteristic modelNumberString("2A24",BLERead,"DF Bluno");
BLECharacteristic manufacterString("2A29",BLERead,"DFRobot");



//extern volatile unsigned char usbRxBuf[256];
//extern volatile int usbRxHead,usbRxTail;

#define WEAK __attribute__ ((weak))

void switchCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  char buf[20]={0};
  int length = characteristic.valueLength();
  memcpy(buf,characteristic.value(),length);
  if(length){
  //  Serial.print("len=");Serial.println(len);
  }
  if(bleRxHead){
    int i;
    for(i=0;i<bleRxTail-bleRxHead;i++){
      bleRxBuf[i] = bleRxBuf[bleRxHead+i];
    }
    bleRxTail -= bleRxHead;
    bleRxHead = 0;
  }else if(bleRxTail == 80){
      return;
  }
  if(bleRxTail+length>80){
    bleRxTail = 80;
  }else{
    memcpy((void *)&bleRxBuf[bleRxTail],buf,length);
    bleRxTail += length;
  }

}

void Serial_::setName( const char * _name)
{
  Serial.println(_name);
  if((strlen(_name))>20){
    memcpy(name,_name,19);
    name[19] = 0;
  }
  else{
    strcpy(name,_name);
  }
}

void Serial_::begin()
{
	peek_buffer = -1;
	blePeripheral.setLocalName(name);
	blePeripheral.setAdvertisedServiceUuid(blunoService.uuid());
	blePeripheral.addAttribute(deviceinfoService);   // Add the BLE Battery service
	blePeripheral.addAttribute( modelNumberString );
	blePeripheral.addAttribute( manufacterString );

	blePeripheral.addAttribute(blunoService);   // Add the BLE Battery service
	blePeripheral.addAttribute(serialCharact); // add the battery level characteristic

	serialCharact.setEventHandler(BLEWritten, switchCharacteristicWritten);

	blePeripheral.begin();
}

void Serial_::end(void)
{
}

int Serial_::available(void)
{
  return (bleRxTail>bleRxHead);
}

int Serial_::peek(void)
{
  if(bleRxTail-bleRxHead){
    return bleRxBuf[0];
  }else{
    return -1;
  }
}

int Serial_::read(void)
{
  if(bleRxTail-bleRxHead){
    return bleRxBuf[bleRxHead++];
  }else{
    return -1;
  }
}

void Serial_::flush(void)
{
  //USB_Flush(0);
}

size_t Serial_::write(uint8_t c)
{
    if(*this)
    serialCharact.setValue(&c,1);
  return 1;
}

size_t Serial_::write(void *buffer, size_t size)
{
  volatile size_t left = size;

  if (*this)  {
    while(left){
      size_t count = left>20?20:left;
      serialCharact.setValue(((const uint8_t*)buffer)+size-left,count);
      left -= count;
    }
    return size;
  }
  return size;
}

size_t Serial_::write(const uint8_t *buffer, size_t size)
{
  return write((void *)buffer,size);
}

Serial_::operator bool() {
  bool result = false;
  BLECentral central = blePeripheral.central();
  if(central.connected())
    result = true;
  return result;
}
Serial_ BLESerial;

//#endif

