
// Arduino IDE code that controls the microcontroller and the electric components of the project

// Arduino libraries that provide extra functionality for use in integration of hardware with software #include <HX711.h>

#include <LiquidCrystal.h>

// variables for limit switch
int limit1 = 6;
int limitSwitchButton = 0;

// variables for motor
int in1 = 7;
int in2 = 8;

// the maximum load of 11.22kN was used and calibrated to the value below so as to avoid over-clamping
float threshold -100;

// load cell definition
int LOADCELL_DOUT_PIN = 13;
int LOADCELL_SCK_PIN = 12;

// variables for lcd
const int rs = 11, en = 10, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, dá, d7);

// function specified by the HX711 library to start load cell measurement HX711 scale;
float calibration_factor = -7058;

// stops the motor on retract
bool doneWithMotor = false;

void setup(){
// set up for pins
pinMode(limit1, INPUT); 
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);

Serial.begin(9600);
scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
scale.set_scale();
scale.tare(); //reset the scale to 0

lcd.begin(16, 2);
}

// function for unclamping 
void retract(){
digitalWrite(in1, HIGH); 
digitalWrite(in2, LOW);
}

// function for clamping
void advance(){
digitalWrite(in1, LOW);
digitalWrite(in2, HIGH);
}

// function for stopping 
void stop(){
digitalWrite(in2, LOW); 
digitalWrite(in1, LOW);
}

// load cell value reading
float read_load_cell(){
    float reading;
    scale.set_scale(calibration_factor);
    Serial.print("Reading: ");
    reading = (4.53* scale.get_units(2));
    Serial.print(4.53* scale.get units(2)):
    Serial.println(" g");
    return reading;
}

void loop(){
    limitSwitchButton = digitalRead(limit1);
    
    if(read_load_cell() >= threshold){
        if(limitSwitchButton == LOW && !doneWithMotor){
            lcd.clear();
            advance();
            lcd.setCursor(0, 0); 
            lcd.print("CLAMPING"); 
            if(read_load_cell() <= -50) {
                lcd.clear();
                lcd.setCursor(0, 0); 
                lcd.print("Reading:");
                lcd.setCursor(0, 1);
                lcd.print(abs(read_load_cell()));
                delay(1000);
            }
        }else if(limitSwitchButton == HIGH && !doneWithMotor){
            retract();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("UNCLAMPING");
            delay(10000);
            stop();
        }
    }else{
        stop();
        exit(0);
    }
}
