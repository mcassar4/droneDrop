#include "Arduino.h"
#include "ESP32Servo.h"


Servo myservo; // Create servo object

int servoPin = 1; // Servo connected to GPIO pin 35
int photoresistorPin = 2; // Photoresistor connected to GPIO pin 34

int lightThreshold = 500; // Threshold for light level to open the servo


void setup() {
    Serial.begin(115200);
    myservo.attach(servoPin);
    pinMode(photoresistorPin, INPUT);
    
    // Move servo to closed position on boot
    Serial.println("Moving to closed state.");
    myservo.write(0);
    delay(60000); // Wait a minute to start the recording
}

void loop() {
    int lightLevel = analogRead(photoresistorPin);
    Serial.print("Light level: ");
    Serial.println(lightLevel);

    if (lightLevel > lightThreshold) {
        Serial.println("Light threshold exceeded. Moving to open state.");
        myservo.write(90); // Move servo to open state
    }

    delay(100); // Short delay for loop stability and to avoid flooding the serial output
}
