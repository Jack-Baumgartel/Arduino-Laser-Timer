
//If laser is on sensor, run timer block and then stop.
//Print time during run on the lcd!!
//If no light, reset loop until there is!
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

int ledPin = 13;  //use the built in LED for feedback
int sensorPin = A0; //input pin for sensor
int sensorValue = 0;  //variable to store sensor value
unsigned long starttime = 0; //set blank variables for timer
unsigned long finishtime = 0;
unsigned long currenttime = 0;
float laptime = 0;
boolean start = false; //use to prevent infinite looping
boolean go = false; //when to start the timer loop code
int count = 0;  //count for timer section

void(* resetFunc) (void) = 0;  // declare reset fuction at address 01`

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize lED pin as an output
  pinMode(ledPin, OUTPUT);
  // read the value from the light sensor and store it in sensorValue
  sensorValue = analogRead(sensorPin);
  digitalWrite(ledPin, LOW);
  go = false;  //do not enable the timer to go yet

  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
  lcd.setCursor(2, 1);

  if (sensorValue < 500) { //if insufficient light, reset and try again
    lcd.print("Too dim!:");
    lcd.print(sensorValue);
    delay(500);
    resetFunc(); 

  } 
    else {  //if light is sufficient, initiate timer
    lcd.print("All good:");
    lcd.print(analogRead(sensorPin));
    delay(100);
    lcd.clear();
    lcd.print("Timer initiated!");
    delay(500);
    go = true;
    start = true;
    digitalWrite(ledPin, HIGH);
  }
}


void loop() {
  if (start == true) { // only run the code if start is still true
    if (go > 0.5) {
      if ((analogRead(sensorPin) > 300) && (count == 0)) {  
        // dont do anything until a break is detected
        delay(1);
        return;
      } 
      
        else if ((analogRead(sensorPin) > 300) && (count == 1)) {  
          // still dont do anything until a second break is detected
          lcd.clear();
          currenttime = millis()-starttime;
          lcd.print(currenttime);
          delay(1);
          return;
        } 
      
        else if ((analogRead(sensorPin) < 300) && (count == 0)){
          //if theres a break and count is 0, record a start time and turn off the LED
          starttime = millis();  // record the start time
          digitalWrite(ledPin, LOW);  //turn off the led
          count = 1;
          delay(750); //wait to prevent double entries
          return;
        }

        else if ((analogRead(sensorPin) < 300) && (count == 1)) {
          finishtime = millis();   //record the finish time
          digitalWrite(ledPin, HIGH);  //turn back on the led
          lcd.clear();
          lcd.print("finished!");
          count = 2;
          go = false;
          delay(500);
        }
    } 

    laptime = float(finishtime-starttime);
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Laptime");
    lcd.setCursor(5,1);
    lcd.print(laptime/1000,3);
    lcd.print("s");
    delay(10000);
    resetFunc();
     
  
 }  
  
  else {
    delay(200);
    resetFunc();
  }
}
  
