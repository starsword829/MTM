/*
  Analog input, analog output, serial output

 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.

 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground

 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
//const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
float outputValue = 0.0;        // value output to the PWM (analog out)
  int count = 0;
  int sum = 0;
  int threshold = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  threshold = analogRead(analogInPin);
}

void loop() {
  // read the analog in value:
  // map it to the range of the analog out:
    sensorValue = analogRead(analogInPin);
    sensorValue = (float)sensorValue;
    threshold = (float)threshold;
    
    outputValue = (sensorValue-threshold)/150.0;
  // change the analog out value:
  //analogWrite(analogOutPin, outputValue);

  // print the results to the serial monitor:

  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t");
  Serial.println(outputValue);
  count++;
  sum+=sensorValue;
  if(count>100){
    Serial.println(sum/count);
    count=0;
    sum=0;
  }
  
  //Serial.print("\t output = ");
  //Serial.println(outputValue);

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(30);
}
