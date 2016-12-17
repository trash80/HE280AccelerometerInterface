/*
 * 
 * The code below has been taken from Seemecnc's Repetier fork, specifically Commands.cpp
 * https://github.com/seemecnc/Firmware/blob/master/Repetier%20Firmware/Repetier/Commands.cpp
 * 
 */

#include <Arduino.h>

#define ACCELEROMETER_I2C_ADDR 0x19

#ifndef Z_PROBE_SENSITIVITY
#define Z_PROBE_SENSITIVITY  12 // 0-126 7 bit value
#endif

#ifdef DEBUG
 #define debug_print(x)  Serial.print (x)
 #define debug_println(x)  Serial.println (x)
#else
 #define debug_print(x)
 #define debug_println(x)
#endif

bool accelerometer_send(uint8_t val)
{
  Wire.beginTransmission(ACCELEROMETER_I2C_ADDR);
  Wire.write(val);
  if(Wire.endTransmission(false)) {
    debug_println("send i2c error.");
    return false;
  }
  return true;
}

bool accelerometer_write(uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(ACCELEROMETER_I2C_ADDR);
  Wire.write(reg);
  Wire.write(val);
  if(Wire.endTransmission()) {
    debug_println("write i2c error.");
    return false;
  }
  return true;
}

int accelerometer_read(uint8_t reg)
{
  uint8_t receiveByte;

  accelerometer_send(reg); //Send an 8bit register to be read
  
  Wire.requestFrom(ACCELEROMETER_I2C_ADDR,1); //Request one 8bit response
  
  if(Wire.available()) 
  {
    receiveByte = Wire.read();

    debug_print("Read REG: ");
    debug_print(reg);
    debug_print(" Value: ");
    debug_println(receiveByte);
    
    return receiveByte;
  }
  else
  {
    debug_println("i2c recv error.");
    return -1;
  }
}

bool accelerometer_rwr(uint8_t reg, uint8_t val)
{
  int d = accelerometer_read(reg);
  if(d < 0) return false;
  
  bool success = accelerometer_write(reg,val);
  if(!success) return false;
  
  d = accelerometer_read(reg);
  if(d < 0) return false;
  
  return true;
}

bool accelerometer_recv(uint8_t reg)
{
  uint8_t receiveByte;

  accelerometer_send(reg); //Send an 8bit register to be read
  
  Wire.requestFrom(ACCELEROMETER_I2C_ADDR,1); //Request one 8bit response
  
  if(Wire.available()) {
    receiveByte = Wire.read(); 

    debug_print("read reg ");
    debug_print(reg);
    debug_print("Value: ");
    debug_println(receiveByte);

    return true;
  } else {
    debug_println("i2c recv error.");

    return false;
  }
}

bool accelerometer_init()
{
  Wire.begin(); // join i2c bus

  boolean res = false;
  
  res = accelerometer_recv(0x0F); //WHO AM I = 0x6A
  res = accelerometer_recv(0x31); //INT1_SRC (31h)

  //CTRL_REG1 (20h)
  res = accelerometer_rwr(0x20,0b10011100); // ODR 5.376kHz in LPMode [7-4]. Low power enable [3]. Z enable [2].
  if(!res) return false;

  //CTRL_REG3 (22h)
  res = accelerometer_rwr(0x22,0b01000000); // CLICK interrupt on INT1 pin [7]. AOI (And Or Interrupt) on INT1 en [6]. AOI on INT2 en [5].
  if(!res) return false;

  //CTRL_REG6 (25h)
  res = accelerometer_rwr(0x25,0b000000); //Click interrupt on INT2 pin [7]. Interrupt 1 function enable on INT2 pin [6]. Interrupt 2 on INT2 pin enable [5]. 0=INT Active High [1]. 
  if(!res) return false;

  //CTRL_REG4 (23h)
  res = accelerometer_rwr(0x23,0b00110000); // Full-scale selection 16G [5-4]. High resolution mode [3].
  if(!res) return false;

  //CTRL_REG5 (24h)
  res = accelerometer_rwr(0x24,0b01001010); // FIFO enable [6]. Latch INT1 [3]. Latch INT2 until cleared by read [1].
  if(!res) return false;
  
  //INT1_CFG (30h)
  res = accelerometer_rwr(0x30,0b100000); // ZHI events enabled [5]. ZLO events enabled [4].
  if(!res) return false;
  
  //INT1_SRC (31h)
  accelerometer_recv(0x31);
  
  //INT1_THS (32h)  this is the i2c probe
  res = accelerometer_rwr(0x32,Z_PROBE_SENSITIVITY); // 7bits
  if(!res) return false;
  
  //INT1_DURATION (33h)
  res = accelerometer_rwr(0x33,0);
  if(!res) return false;
  
  //INT2_CFG (34h)
  res = accelerometer_rwr(0x34,0b000000); // ZHI events not enabled on INT2 [5].
  if(!res) return false;
  
  //INT2_SRC (35h)
  
  //INT2_THS (36h)
  res = accelerometer_rwr(0x36,50); // 7bits
  if(!res) return false;

  //INT2_DURATION (37h)
  res = accelerometer_rwr(0x37,0);
  if(!res) return false;
  
  //CLICK_CFG (38h)
  res = accelerometer_rwr(0x38,0b10000); //Single Click Z axis
  if(!res) return false;
  
  //CLICK_SRC (39h)
  accelerometer_recv(0x39);
  
  //CLICK_THS (3Ah)
  res = accelerometer_rwr(0x3A,50);
  if(!res) return false;
  
  return true;
}

void accelerometer_setThreshold(uint8_t thresh)
{
  //INT1_THS (32h)  this is the i2c probe
  accelerometer_recv(0x32);
  accelerometer_write(0x32, thresh); // 7bits
  accelerometer_recv(0x32);
}

void accelerometer_status()
{
    accelerometer_recv(0x31); //INT1_SRC (31h)
    accelerometer_recv(0x35); //INT2_SRC (35h)
    accelerometer_recv(0x39); //CLICK_SRC
    accelerometer_recv(0x2D);
}
