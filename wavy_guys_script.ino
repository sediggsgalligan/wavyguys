//Libraries
#include <CapacitiveSensor.h>//https://github.com/PaulStoffregen/CapacitiveSensor

//Parameters
// bool autocal  = 1;
const int numReadings  = 3;


CapacitiveSensor csred  = CapacitiveSensor(30, 31);
CapacitiveSensor csorange  = CapacitiveSensor(28, 29);
CapacitiveSensor csyellow  = CapacitiveSensor(26, 27);
CapacitiveSensor csgreen  = CapacitiveSensor(24, 25);
CapacitiveSensor csblue  = CapacitiveSensor(22, 23);

CapacitiveSensor sensors[] = {csred, csorange, csyellow, csgreen, csblue};
const int numGuys = 5;

long readings [numGuys][numReadings];

int readIndex  = 0;

long totals [numGuys];

const int sensitivity  = 1000;//1000;
const int thresh  = 5000;
const int csStep  = 10000;

int redPin = 51;
int orangePin = 49;
int yellowPin = 47;
int greenPin = 45;
int bluePin = 43;

int DEBUG = 1;

unsigned long autocal = 0xFFFFFFFF; //1000; //0xFFFFFFFF


void setup() {
  
  //red guy
  pinMode(redPin, OUTPUT);

  //orange guy
  pinMode(orangePin, OUTPUT);

  //yellow guy
  pinMode(yellowPin, OUTPUT);

  //green guy
  pinMode(greenPin, OUTPUT);

  // blue guy
  pinMode(bluePin, OUTPUT);

  //Init Serial USB
  Serial.begin(9600);
  Serial.println(F("Initialize System"));
  //Init cs
  // if (autocal == 0) {
  //   {
      // csred.set_CS_AutocaL_Millis(0xFFFFFFFF);
      // csorange.set_CS_AutocaL_Millis(0xFFFFFFFF);
      // csyellow.set_CS_AutocaL_Millis(0xFFFFFFFF);
      // csgreen.set_CS_AutocaL_Millis(0xFFFFFFFF);
      sensors[0].set_CS_AutocaL_Millis(autocal);
      sensors[1].set_CS_AutocaL_Millis(autocal);
      sensors[2].set_CS_AutocaL_Millis(autocal);
      sensors[3].set_CS_AutocaL_Millis(autocal);
      sensors[4].set_CS_AutocaL_Millis(autocal);
  //   }
  // }
}

void loop() {
  
  // Serial.println(smooth(redtotal, redreadings, redIndex, csred, redPin));
  smooth(0, redPin);
  
  smooth(1, orangePin);
  smooth(2, yellowPin);
  smooth(3, greenPin);
  smooth(4, bluePin);
  // Serial.println(csblue.capacitiveSensor(sensitivity));

  // handle index
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  // delay(10);

}

long smooth(int colorIndex, int output) { /* function smooth */

  ///Serial.println(output);
  ////Perform average on sensor readings
  long average;  

  // subtract the last reading:
  totals[colorIndex] = totals[colorIndex] - readings[colorIndex][readIndex];
  // read the sensor:
  readings[colorIndex][readIndex] = sensors[colorIndex].capacitiveSensor(sensitivity);

  if (DEBUG==1) {
    
    Serial.print(readings[colorIndex][readIndex]);

    if (colorIndex == numGuys-1) {
        Serial.println("");
    } else {
      Serial.print("  ");
    }
  }

  // add value to total:
  totals[colorIndex] = totals[colorIndex] + readings[colorIndex][readIndex];

  // // calculate the average:
  average = totals[colorIndex] / numReadings;
  

  if (average>thresh) {
    digitalWrite(output, HIGH);  // turn the LED on (HIGH is the voltage level)
  }

  else {
    digitalWrite(output, LOW);  // turn the LED on (HIGH is the voltage level)
  }

  if (DEBUG==2) {
    
    Serial.print(average);

    if (colorIndex == numGuys-1) {
        Serial.println("");
    } else {
      Serial.print("  ");
    }
  }

  return average;

}