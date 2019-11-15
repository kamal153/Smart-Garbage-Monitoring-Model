#include<Servo.h>
#include "LiquidCrystal.h"

int sensorValue = 9;      //mositure

int pir = 2;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)
int height=20;
LiquidCrystal lcd(3,4,5,6,7,8);
const int trigPin1 = 10;
const int echoPin1 = 11;
const int trigPin2 = 12;
const int echoPin2 = 13;
int servoPin1 = 14;
int servoPin2 = 15;
int sensorPin = A0;    // select the input pin for the potentiometer

Servo servo,servo2;

long duration1,distance1,duration2,distance2;
int flag=0;

void setup() {
  pinMode(pir, INPUT);    // initialize sensor as an input
  Serial.begin(9600);        // initialize serial
  servo.attach(servoPin1);
  servo.write(0);
  servo2.attach(servoPin2);
  servo2.write(0);
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input

  lcd.begin(16,2);

  // set cursor position to start of first line on the LCD
  lcd.setCursor(0,0);
  //text to print
  lcd.print("% Filled :-");
}

void loop(){
  delay(1000);
  if (digitalRead(pir)==HIGH) {           // check if the sensor is HIGH
    for(int angle = 0; angle < 90; angle++)  
    {                                  
      servo.write(angle);               
      delay(10);
    }
    flag=1;
  } 
  else {
      delay(100);
      if(flag==1)
      {
        for(int angle = 90; angle > 0; angle--)    
        {                                
          servo.write(angle);           
          delay(15);       
        }
        sensorValue = analogRead(sensorPin);  // read the value from the sensor:
        sensorValue=( 100 - ( (sensorValue/1023.00) * 100 ) );            
        if(sensorValue < 2)
        {
            /*for(int angle = 0; angle < 90; angle++)  
            {                                  
              servo2.write(angle);               
              delay(15);
            }  */        //Anticlockwise 
        }
        else
        {
                      //Clockwise
        }
        flag=0;

        //code for ultrasonic sensor
        digitalWrite(trigPin1, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin1, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin1, LOW);
        duration1 = pulseIn(echoPin1, HIGH);
        distance1= duration1*0.034/2;
      
        
        digitalWrite(trigPin2, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin2, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin2, LOW);
        duration2 = pulseIn(echoPin2, HIGH);
        distance2= duration2*0.034/2;
      
        Serial.print("Fill : ");
        distance1=(distance1/20)*100;
        Serial.print(distance1);
        Serial.print("% ");
        distance2=(distance2/20)*100;
        Serial.print(distance2);
        Serial.print("%");
        delay(1000);

        lcd.setCursor(1,0);
        lcd.print("100");
      }
  }
}
