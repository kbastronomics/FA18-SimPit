# TCA9554 Ardruino library
This library help work with TCA9554 GPIO expander.
## Getting Started
For start download .zip archive or simple .h file and include in your project.
### Example
```
    #include <Wire.h>

    TCA9554 gpioExpander (56);
    
    void setup()
    {
      Wire.begin();
      Serial.begin(9600);
      while (!Serial);
      delay(1000);
      gpioExpander.PinSetMode(1,OUTPUT);
    }
    void loop()
    {
      gpioExpander.DigitalWrite(1,LOW);
      delay(1000);
      gpioExpander.DigitalWrite(1,HIGH);
      delay(1000);      
    }
```
