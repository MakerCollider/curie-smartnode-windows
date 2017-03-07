
#ifndef _BLE_SERIAL_H_INCLUDED
#define _BLE_SERIAL_H_INCLUDED

struct ring_buffer;

#define BLE_BUFFER_SIZE 20


class Serial_ : public Stream
{
private:
  int peek_buffer;
public:
  Serial_() { peek_buffer = -1;};
  void setName(const char * _name);
  void begin();
  void end(void);

  virtual int available(void);
  virtual int peek(void);
  virtual int read(void);
  virtual void flush(void);
  virtual size_t write(uint8_t);
  virtual size_t write(void*, size_t);
  virtual size_t write(const uint8_t *, size_t);
  using Print::write; // pull in write(str) and write(buf, size) from Print
  operator bool();

  volatile uint8_t _rx_buffer_head;
  volatile uint8_t _rx_buffer_tail;
  unsigned char _rx_buffer[BLE_BUFFER_SIZE];
public:
  char name[20];
};

extern Serial_ BLESerial;
#endif
