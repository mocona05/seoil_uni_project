#include <Arduino.h>

#define DAC2  26
#define LED_BUILTIN 2


int val = 0;
void setup() {
  Serial.begin(115200);
    }

void loop() {
  // put your main code here, to run repeatedly:
  val = hallRead();
  // print the results to the serial monitor:
  //Serial.print("sensor = ");
  Serial.println(val);//to graph 
}
