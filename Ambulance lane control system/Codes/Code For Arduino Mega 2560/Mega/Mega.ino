#include <SoftwareSerial.h>

// Pin definitions for traffic lights
const int redPins[] = {23, 29, 35, 41};
const int yellowPins[] = {25, 31, 37, 43};
const int greenPins[] = {27, 33, 39, 45};

// Pin definitions for ultrasonic sensors
const int trigPins[] = {2, 4, 6, 8};
const int echoPins[] = {3, 5, 7, 9};

SoftwareSerial mySerial(10, 11);  // RX, TX

// Timing variables
unsigned long greenTime = 3000;  // Base green time (5 seconds)
unsigned long yellowTime = 1000;  // 1 second
unsigned long redTime = 3000;  // 5 seconds

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);  // Using built-in LED for testing

  // Initialize traffic light pins
  for (int i = 0; i < 4; i++) {
    pinMode(redPins[i], OUTPUT);
    pinMode(yellowPins[i], OUTPUT);
    pinMode(greenPins[i], OUTPUT);
    digitalWrite(redPins[i], LOW);  // Ensure all lights are off initially
    digitalWrite(yellowPins[i], LOW);
    digitalWrite(greenPins[i], LOW);

    // Initialize ultrasonic sensor pins
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
  Serial.println("Mega setup complete. Waiting for data...");
}

void loop() {
  if (mySerial.available()) {
    int ambulanceDetected = mySerial.read();
    Serial.print("Received signal: ");
    Serial.println(ambulanceDetected);
    if (ambulanceDetected == 1) {
      Serial.println("Clearing ambulance lane.");
      clearAmbulanceLane();
    }
  } else {
    normalTrafficControl();
  }
}

void normalTrafficControl() {

  for (int i = 0; i < 4; i++) {  
       int ambulanceDetected = mySerial.read();
   
    // Assuming a 4-way junction
    //unsigned long adjustedGreenTime = adjustGreenTime(i);
    Serial.print("Lane ");
    Serial.print(i);
    Serial.print(" Green time: ");
  //  Serial.println(adjustedGreenTime);
   if (ambulanceDetected == 1) {
      Serial.println("Clearing ambulance lane.");
      clearAmbulanceLane();
      break;
    }
    setTrafficLight(i, HIGH, LOW, LOW);  // Green for lane i
    delay(greenTime);
     if (ambulanceDetected == 1) {
      Serial.println("Clearing ambulance lane.");
      clearAmbulanceLane();
     break;
    }
    setTrafficLight(i, LOW, HIGH, LOW);  // Yellow for lane i
    delay(yellowTime);
       if (ambulanceDetected == 1) {
      Serial.println("Clearing ambulance lane.");
      clearAmbulanceLane();
 break;
    }
    setTrafficLight(i, LOW, LOW, HIGH);  // Red for lane i
    delay(redTime);
       if (ambulanceDetected == 1) {
      Serial.println("Clearing ambulance lane.");
      clearAmbulanceLane();
      break;
    }
  }
}

void clearAmbulanceLane() {
  // Turn all lights red
  for (int i = 0; i < 4; i++) {
    Serial.print("ambulance found detection started");
    setTrafficLight(i, LOW, LOW, HIGH);
    
    Serial.print("Lane ");
    Serial.print(i);
    Serial.println(" set to Red.");
    delay(100);
  }
  // Turn the green light on for lane 0 (assuming lane 0 is for ambulance)
  setTrafficLight(0, HIGH, LOW, LOW);
  Serial.println("Lane 0 set to Green for ambulance.");
  delay(10000);  // 10 seconds for ambulance to pass
  // Turn off the green light for lane 0
  setTrafficLight(0, LOW, LOW, HIGH);
  Serial.println("Lane 0 set back to Red.");
}

void setTrafficLight(int lane, int greenState, int yellowState, int redState) {
  digitalWrite(greenPins[lane], greenState);
  digitalWrite(yellowPins[lane], yellowState);
  digitalWrite(redPins[lane], redState);
  Serial.print("Lane ");
  Serial.print(lane);
  Serial.print(" - Green: ");
  Serial.print(greenState);
  Serial.print(", Yellow: ");
  Serial.print(yellowState);
  Serial.print(", Red: ");
  Serial.println(redState);
}

unsigned long adjustGreenTime(int lane) {
  // Calculate distance using ultrasonic sensor
  digitalWrite(trigPins[lane], LOW);
  delayMicroseconds(2);
  digitalWrite(trigPins[lane], HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPins[lane], LOW);

  unsigned long duration = pulseIn(echoPins[lane], HIGH);
  unsigned long distance = duration * 0.034 / 2;

  Serial.print("Lane ");
  Serial.print(lane);
  Serial.print(" Distance: ");
  Serial.println(distance);

  // Adjust green time based on distance (traffic density)
  if (distance < 30) {  // High density
    return greenTime + 3000;  // Add 3 seconds
  } else if (distance < 60) {  // Medium density
    return greenTime + 1000;  // Add 1 second
  } else {  // Low density
    return greenTime;  // Base time
  }
}