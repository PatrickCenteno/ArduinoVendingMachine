#include <LiquidCrystal.h>

// Init LiquidCrystal, pass input pins as params
LiquidCrystal lcd(13, 12, 4, 3, 2, 1);

// Macros for Analog Inputs
#define TRIG_COIN A0
#define ECHO_COIN A1
#define LIGHTS A5

/** 
 * Constants 
 */
const int MOTOR_PIN_1 = 9;
const int MOTOR_PIN_2 = 10;
const int MOTOR_PIN_3 = 11;

// Buttons adjusted to match motors
const int BUTTON_PIN_1 = 7;
const int BUTTON_PIN_2 = 5;
const int BUTTON_PIN_3 = 6;

const int COIN_SENSOR_PIN = 8;

// Distance value that is determined to signify product dropping
const int SLOT_WIDTH = 2500;

// Set time that motor should run to dispense product if we dont use proximity sensor
const int MOTOR_TIME = 1500;

// Motor power (1- 255)
const int MOTOR_SPEED = 255;

// Delay after coin is registers 
const int REG_DELAY = 150;

// Delay for coin input
const int COIN_DELAY = 150;

// Microseconds for proximity sensor pulse
const int PROX_RUN_TIME = 2;
const int PROX_PAUSE_TIME = 10;

// Coin boolean
bool coinDetected = false;


void setup(){
  // Initialize pins
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
  pinMode(MOTOR_PIN_3, OUTPUT);

  pinMode(BUTTON_PIN_1, INPUT);
  pinMode(BUTTON_PIN_2, INPUT);
  pinMode(BUTTON_PIN_3, INPUT);

  pinMode(COIN_SENSOR_PIN, INPUT);
  
  pinMode(TRIG_COIN, OUTPUT);
  pinMode(ECHO_COIN, INPUT);

  pinMode(LIGHTS, OUTPUT);

//  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Insert Coin");
}

void loop(){
  
  // Wait for coin input
  if (detectDistanceChange()){
    coinDetected = true;
    // Can now listen for button presses
  }

  // Product 1
  if (digitalRead(BUTTON_PIN_1) == 1 ){
    runMotor(MOTOR_PIN_1);
  }

  // Product 2
  if (digitalRead(BUTTON_PIN_2) == 1 ){
    runMotor(MOTOR_PIN_2);
  }
  
  //Product 3
  if (digitalRead(BUTTON_PIN_3) == 1 ){
    runMotor(MOTOR_PIN_3);
  }
}

/**
 * Runs motor until a distance shorter than the base distance is detected
 * ignores all readings for the first second
 * @params: const int motorPin, const int proxPin
 * @returns: void
 */
bool detectDistanceChange(){
  
  int distance;
  while (true){
    if (millis() > 3000){
      break;
    }
  }
  // Standard pulse code
  digitalWrite(TRIG_COIN, LOW);
  delayMicroseconds(PROX_RUN_TIME);
  digitalWrite(TRIG_COIN, HIGH);
  delayMicroseconds(PROX_PAUSE_TIME);
  digitalWrite(TRIG_COIN, LOW);
  distance = (pulseIn(ECHO_COIN, HIGH)/2) / 29.1;
  if (distance < SLOT_WIDTH && distance > 0){
    lcd.clear();
    lcd.print("Please select");
    lcd.setCursor(0,1);
    lcd.print("Item");
    return true;
  }else
    return false;
}

/**
 * Checks if necessary coins were inserted and initializes
 * Uses millis() function and a while(true) loop to similuate delay()
 * motor sequences base on param passed
 * @params const int
 * calls void detectDistanceChange(const int)
 * @returns void 
 */
void runMotor(const int motorPin){
  if (coinDetected){
    int initTime = millis();
    analogWrite(motorPin, MOTOR_SPEED);
    lcd.clear();
    lcd.print("Now dispensing");
    while(true){
      // Start lights
      if ((millis() - initTime) > 750){
        digitalWrite(LIGHTS, HIGH);
      }

      if ((millis() - initTime) > 1250){
        analogWrite(motorPin, 0);
        lcd.clear();
        lcd.print("insert coin");
      }

      if ((millis() - initTime) > 5000){
        digitalWrite(LIGHTS, LOW);
        break;
        
      }
      
      coinDetected = false;
   }
  }else{
//    Serial.print("Please insert coin");
     lcd.clear();
     
     lcd.print("Please select");
     lcd.setCursor(0,1);
     lcd.print("Item");
  }
}
