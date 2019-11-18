#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include<Servo.h>

int pir=D0;
int servoPin1=D1;
int servoPin2=D2;
int trig1=D3;
int echo1=D4;
int trig2=D5;
int echo2=D6;
int sensorPin=A0;
int flag=0;

const char* ssid     = "kkk"; //WiFiSSID
const char* password = "kkk15032000";  //PASSWORD
const char* host = "api.thingspeak.com";
const char* privateKey = "WZR2LMP6I5OXQF87"; //ENTER_YOUR_PRIVATE_KEY

          char auth[] = "bV6v5cwR-kINmqKkbgP8uYQTXIgQoWBs";  //code sent via email

Servo servo,servo1;
long duration1,distance1,duration2,distance2;
int sensorValue = 0;      //mositure

void setup() {
  Serial.begin(9600);

        Blynk.begin(auth, "kkk", "kkk15032000");  //wifi name and password
  
  pinMode(pir,INPUT);    // initialize sensor as an input
  pinMode(trig1,OUTPUT);
  pinMode(echo1,INPUT);
  pinMode(trig2,OUTPUT);
  pinMode(echo2,INPUT);
  servo.attach(servoPin1);
  servo.write(0);
  servo1.attach(servoPin2);
  servo1.write(45);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
          Blynk.run();
  int pirReading =digitalRead(pir);
  delay(2000);
  if (pirReading==HIGH) {  
        Serial.println("motion detected..");// check if the sensor is HIGH
    for(int angle = 90; angle > 0; angle--)    
        {                                
          servo.write(angle);           
          delay(15);       
        }
    flag=1;
  }
  else {
      delay(100);
      if(flag==1)
      {
        for(int angle = 0; angle < 90; angle++)    
        {                                
          servo.write(angle);           
          delay(15);       
        }
        sensorValue = analogRead(sensorPin);  // read the value from the sensor:
        sensorValue=( 100 - ( (sensorValue/1023.00) * 100 ) );
        Serial.print("Mositure : ");
        Serial.print(sensorValue);
        Serial.print("      ");            
        if(sensorValue < 10)
        {
            for(int angle = 45; angle <= 90; angle++)  
            {                                  
              servo1.write(angle);               
              delay(15);
            }          //Anticlockwise 
            
            for(int angle = 90; angle >=45; angle--)  
            {                                  
              servo1.write(angle);               
              delay(15);
            }          //Anticlockwise 
        }
        else
        {
          for(int angle = 45; angle >=0; angle--)  
            {                                  
              servo1.write(angle);               
              delay(15);
            }
             for(int angle = 0; angle <= 45; angle++)  
            {                                  
              servo1.write(angle);               
              delay(15);
            }      //Anticlockwise 
            
        }
        flag=0;

        //code for ultrasonic sensor
        digitalWrite(trig1, LOW);
        delayMicroseconds(2);
        digitalWrite(trig1, HIGH);
        delayMicroseconds(10);
        digitalWrite(trig1, LOW);
        duration1 = pulseIn(echo1, HIGH);
        distance1= duration1*0.034/2;
      
        
        digitalWrite(trig2, LOW);
        delayMicroseconds(2);
        digitalWrite(trig2, HIGH);
        delayMicroseconds(10);
        digitalWrite(trig2, LOW);
        duration2 = pulseIn(echo2, HIGH);
        distance2= duration2*0.034/2;
      
        Serial.print("Fill : ");
        Serial.print(distance1);
        Serial.print("cm ");
        Serial.print(distance2);
        Serial.println("cm");
        delay(1000);
     
        WiFiClient client;
        const int httpPort = 80;
        if (!client.connect(host, httpPort)) {
          Serial.println("connection failed");
          return;
        }
        Serial.println("connection done");
       
        String url = "https://api.thingspeak.com/update?api_key=WZR2LMP6I5OXQF87&field1=";
        url += distance1;
        url += "&field2=";
        url += distance2;
        url += "&field3=";
        url += sensorValue;
      
        Serial.print("Requesting URL: ");
        Serial.println(url);
      
        
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n");
      
        
        while (client.available()) {
          String line = client.readStringUntil('\r');
          Serial.print(line);
        }
      
        Serial.println();
        Serial.println("closing connection"); 

        String url1="Distance : ";
        url1 += distance1;
        url1 += " & ";
        url1 +=distance2;
        Blynk.notify(url1);
        
       }
  }
  
}
