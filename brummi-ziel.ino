/*
 * Miltiple Ultrasonic Sensors
 * Prints the distance read by many ultrasonic sensors in
 * centimeters and inches. They are supported to four pins
 * ultrasound sensors (liek HC-SC04) and three pins
 * (like PING))) and Seeed Studio sensors).
 *
 * The circuit:
 * * In this circuit there is an ultrasonic module HC-SC04,
 *   PING))) and a Seeed Studio (4 pins, 3 pins, 3 pins,
 *   respectively), attached to digital pins as follows:
 * ---------------------     ---------------------     -------------------
 * | HC-SC04 | Arduino |     | PING))) | Arduino |     | Seeed | Arduino |
 * ---------------------     ---------------------     -------------------
 * |   Vcc   |   5V    |     |   Vcc   |   5V    |     |  Vcc  |   5V    |
 * |   Trig  |   12    | AND |   SIG   |   10    | AND |  SIG  |    8    |
 * |   Echo  |   13    |     |   Gnd   |   GND   |     |  Gnd  |   GND   |
 * |   Gnd   |   GND   |     ---------------------     -------------------
 * ---------------------
 * Note: You do not obligatorily need to use the pins defined above
 * 
 * By default, the distance returned by the read()
 * method is in centimeters. To get the distance in inches,
 * pass INC as a parameter.
 * Example: ultrasonic.read(INC)
 *
 * created 3 Mar 2017
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 * modified 11 Jun 2018
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 *
 * This example code is released into the MIT License.
 */

#include <Ultrasonic.h>
#define U1LEDPIN A0
#define U2LEDPIN A1


#define MAXDIST 0.8 //angabge in metern 
const unsigned long timeout = (unsigned long)((double)MAXDIST / 343.0 * 1000000); // dist / airspeed * ms = timeout [ms]

Ultrasonic ultrasonic1(5, 6);
Ultrasonic ultrasonic2(7,8);

//rolling avg 
float ravg1 = 0;
float ravg2 = 0;

void setup() {

  pinMode(U1LEDPIN, OUTPUT);
  digitalWrite(U1LEDPIN, HIGH);
  pinMode(U2LEDPIN, OUTPUT);
  digitalWrite(U2LEDPIN, HIGH);
  
  Serial.begin(115200);
  
  Serial.print(timeout);
  delay(2000);
  ultrasonic1.setTimeout(timeout);
  ultrasonic2.setTimeout(timeout);
}



void loop() {
  unsigned long u1=0,u2=0;
  
  //for(int co=0;co < 3;co++) {
    delay(5);
    u1 = ultrasonic1.read();
    delay(5);
    u2 = ultrasonic2.read();    
  //}
  //u1 /= 3;
  //u2 /= 3;
  

  
  const unsigned int ravgCount = 100;
  ravg1 = (((ravg1 * ravgCount) + u1) / (ravgCount + 1));
  ravg2 = (((ravg2 * ravgCount) + u2) / (ravgCount + 1));

  float delta1 = abs(ravg1 - u1);
  float delta2 = abs(ravg2 - u2);

  if(delta1 > 10) {
    digitalWrite(U1LEDPIN, HIGH);
  } else {
    digitalWrite(U1LEDPIN, LOW);
  }

  if(delta2 > 10) {
    digitalWrite(U2LEDPIN, HIGH);
  } else {
    digitalWrite(U2LEDPIN, LOW);
  }





  Serial.print("MAX: ");
  Serial.print(u1);Serial.print(",");
  Serial.print(u2);Serial.print(",");
  Serial.print(ravg1);Serial.print(",");
  Serial.print(ravg2);Serial.print(",");
  Serial.print(delta1);Serial.print(",");
  Serial.print(delta2);//Serial.print(",");
  Serial.print('\n');
  delay(100);
}
