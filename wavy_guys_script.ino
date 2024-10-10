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

const int sensitivity  = 1000;//1000;//1000;
const int thresh  = 5000;
const int csStep  = 10000;

int redPin = 51;
int orangePin = 49;
int yellowPin = 47;
int greenPin = 45;
int bluePin = 43;

int DEBUG = 3;

unsigned long autocal = 100; //0xFFFFFFFF; //1000; //0xFFFFFFFF

// vars for button
const int buttonPin = 40;  // the number of the pushbutton pin
int buttonState;            // the current reading from the input pin
int lastButtonState = HIGH;  // the previous reading from the input pin

int devicePowerState = LOW;        // the current state of the output pin
const int buttonLEDPin = 41;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


void setup() {

  // Button Pin
  pinMode(buttonPin, INPUT);
  pinMode(buttonLEDPin, OUTPUT);
  
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

  check_power_button();

  if (devicePowerState==LOW) {
    digitalWrite(redPin, LOW);
    digitalWrite(orangePin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    delay(50);
    return;
  } else {
  
    // Serial.println(smooth(redtotal, redreadings, redIndex, csred, redPin));
    smooth(0, redPin);
    check_power_button();
    smooth(1, orangePin);
    check_power_button();
    smooth(2, yellowPin);
    check_power_button();
    smooth(3, greenPin);
    check_power_button();
    smooth(4, bluePin);
    check_power_button();
    // Serial.println(csblue.capacitiveSensor(sensitivity));

    // handle index
    readIndex = readIndex + 1;
    if (readIndex >= numReadings) {
      readIndex = 0;
    }

    // delay(10);
  }

}

int check_power_button() {
    // read the state of the switch into a local variable:
    int reading = digitalRead(buttonPin);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState) {
      // reset the debouncing timer
      lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:

      // if the button state has changed:
      if (reading != buttonState) {
        buttonState = reading;

        // only toggle the LED if the new button state is HIGH
        if (buttonState == LOW) {
          devicePowerState = !devicePowerState;
        }
      }
    }

    // Serial.println(devicePowerState);

    // save the reading. Next time through the loop, it'll be the lastButtonState:
    lastButtonState = reading;


    if (DEBUG==3) {
      Serial.print(reading);
      Serial.print(" ");
      Serial.println(devicePowerState);
    }

    digitalWrite(buttonLEDPin, devicePowerState);
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
