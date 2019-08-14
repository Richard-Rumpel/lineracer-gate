#include <Ultrasonic.h>

/*
 * ANSCHLUSSBELEGUNG:
 * GRAUES KABEL OHNE RING:
 *  br -> VCC
 *  wh -> GND
 * GRAUES KABEL MIT RING:
 *  br -> TOGGLE
 *  wh -> SENS
 */

//#define DEBUG 1

#define U1LEDPIN A0
#define U2LEDPIN A1
#define U1TOGLEPIN 5
#define U1SENSPIN 6
#define U2TOGLEPIN 7
#define U2SENSPIN 8

#define MAXDIST 0.8 //angabge in metern abstand zum sensor in metern



//timeout von schallgeschwindigkeit
const unsigned long timeout = (unsigned long)((double)MAXDIST / 343.0 * 1000000); // dist / airspeed * ms = timeout [ms]

Ultrasonic ultrasonic1(U1TOGLEPIN, U1SENSPIN);
Ultrasonic ultrasonic2(U2TOGLEPIN, U2SENSPIN);

//rolling avg
float ravg1 = 0;
float ravg2 = 0;

void setup() {

  pinMode(U1LEDPIN, OUTPUT);
  digitalWrite(U1LEDPIN, HIGH);
  pinMode(U2LEDPIN, OUTPUT);
  digitalWrite(U2LEDPIN, HIGH);
  
  Serial.begin(115200);

  #ifdef DEBUG
  Serial.print(timeout);
  #endif
    
  ultrasonic1.setTimeout(timeout);
  ultrasonic2.setTimeout(timeout);


  delay(2000);

  //quick calibration ...
  const int quickCalibrationCount = 10;
  unsigned long u1=0,u2=0;
  for(int co=0;co < quickCalibrationCount;co++) {
    delay(5);
    u1 += ultrasonic1.read();
    delay(5);
    u2 += ultrasonic2.read();    
  }
  u1 /= quickCalibrationCount;
  u2 /= quickCalibrationCount;

  ravg1 = u1;
  ravg2 = u2;
}



void loop() {
  unsigned long u1=0,u2=0; //todo move to int
  static unsigned long coolDown1 = 0,coolDown2 = 0,lastRun = 0;
  const unsigned long coolDownTimeMS = 500;


  if((millis() - lastRun) < 100) return;  
  lastRun = millis();
  
  //-- read
  delay(5);
  u1 = ultrasonic1.read();
  delay(5);
  u2 = ultrasonic2.read();
  //-- end read
  
  const unsigned int ravgCount = 100;
  const unsigned int threshold = 10;
  float delta1 = (ravg1 - u1);
  float delta2 = (ravg2 - u2);

  //TODO: move code to function,data to struct.
  //channel 1
  if(delta1 > threshold) {
    digitalWrite(U1LEDPIN, HIGH);
    if((millis() - coolDown1) > coolDownTimeMS) {
      Serial.print("TRIGER1\n"); //send signal to brian
    }
    coolDown1 = millis();
  } else {
    ravg1 = (((ravg1 * ravgCount) + u1) / (ravgCount + 1));
    digitalWrite(U1LEDPIN, LOW);
  }

  //channel2
  if(delta2 > threshold) {
    digitalWrite(U2LEDPIN, HIGH);
    if((millis() - coolDown2) > coolDownTimeMS) {
      Serial.print("TRIGER2\n"); //send signal to brian
    }
    coolDown2 = millis();
  } else {
    ravg2 = (((ravg2 * ravgCount) + u2) / (ravgCount + 1));
    digitalWrite(U2LEDPIN, LOW);
  }

#ifdef DEBUG
  Serial.print(u1);Serial.print(",");
  Serial.print(u2);Serial.print(",");
  Serial.print(ravg1);Serial.print(",");
  Serial.print(ravg2);Serial.print(",");
  Serial.print(delta1);Serial.print(",");
  Serial.print(delta2);//Serial.print(",");
  Serial.print('\n');
#endif
  //delay(100); //wenn delay angepasst wird auch ravgCount neu berechnen !!!
}
