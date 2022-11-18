/*
  TCA9554.h - Library for work with TCA9554 or TCA9554a GPIO expander
  Created by Prohorov Vladimir, Octomber 14, 2019.
  Released into the public domain.
*/
#ifndef TCA9554_H
#define TCA9554_H



#include "Arduino.h"
#include <Wire.h>


#define INPUT_PORT_REG 0x0
#define OUTPUT_PORT_REG 0x1
#define POLARITY_INVERSION_REG 0x2
#define CONFIG_REG 0x3

class TCA9554
{
  public:
    TCA9554(int address);
    int PinSetMode(byte pin, bool mode);
    int DigitalWrite(byte pin, bool state);   // it return 0 if write successful, else retrun -1
    int DigitalRead(byte pin);                // it return pin state if read successful, else return -1
    int GetAddress();                         // it return init address
  private:
    int _address;
};

TCA9554::TCA9554(int address)
{
  _address = address;
}

int TCA9554::PinSetMode(byte pin, bool mode)
{
  byte output_reg = 0;

  Wire.beginTransmission(_address);
  Wire.write(CONFIG_REG);
    
  Wire.requestFrom(_address, 1);
  while (Wire.available()) 
  {                                   
    char output_reg = Wire.read();    
  }
  
  byte error = Wire.endTransmission();
  if (error > 0)
    return -1;

  if (mode == INPUT)
  {
    output_reg |= (1 << pin);
  }
  else if (mode == OUTPUT)
  {
    output_reg &= ~(1 << pin);
  }
  
  Wire.beginTransmission(_address);
  Wire.write(CONFIG_REG);
  Wire.write(output_reg);
  error = Wire.endTransmission();

  if (error > 0)
    return -1;

  return 0;
}

int TCA9554::DigitalWrite(byte pin, bool state)
{
  byte output_reg = 0;
  // This code get current register data
  Wire.beginTransmission(_address);
  Wire.write(OUTPUT_PORT_REG);
    
  Wire.requestFrom(_address, 1);
  while (Wire.available()) 
  {                                   
    char output_reg = Wire.read();    
  }
  
  byte error = Wire.endTransmission();
  if (error > 0)
    return -1;

  // This code get update register 
  if (state == HIGH)
  {
    output_reg |= (1 << pin);
  }
  else if (state == LOW)
  {
    output_reg &= ~(1 << pin);
  }
  
  Wire.beginTransmission(_address);
  Wire.write(OUTPUT_PORT_REG);
  Wire.write(output_reg);
  error = Wire.endTransmission();

  if (error > 0)
    return -1;

  return 0;
}

int TCA9554::DigitalRead(byte pin)
{
  byte output_reg = 0;
  // This code get current register data
  Wire.beginTransmission(_address);
  Wire.write(INPUT_PORT_REG);
    
  Wire.requestFrom(_address, 1);
  while (Wire.available()) 
  {                                   
    char output_reg = Wire.read();    
  }
  
  byte error = Wire.endTransmission();
  if (error > 0)
    return -1;
  
  if ((output_reg & (1 << pin)) > 0)
    return 1;
  else
    return 0;  
}

int TCA9554::GetAddress()
{
  return _address;
}
#endif
