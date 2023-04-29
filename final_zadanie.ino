#include <dht.h>
#define dht11 8
#define TRIGGER_PIN  12
#define ECHO_PIN     11
const int MAX_DISTANCE = 400;
const int TIMER_ENABLED = false;
const int buzzerPin = 9;
char serialData;
int inputPin = 10;

dht DHT; 
int readDHT, temp, hum;
float speedOfSound, distance, duration;
unsigned long previousMillis = 0;
const long interval = 2000;
bool beepSound;
bool recordValues;

void setup() {
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(inputPin, OUTPUT);
  Serial.begin(9600);
  beepSound = false;
  recordValues = false;
}

//read humidity values
void readDHT11(int dht11_pin, int& readDHT, float& temp, float& hum) {
  int readData = DHT.read11(dht11_pin);
  temp = DHT.temperature;        
  hum = DHT.humidity;          
  readDHT = readData;
}

void measureDistance(int triggerPin, int echoPin, float temp, float hum, float& distance) {
  float speedOfSound = 331.4 + (0.6 * temp) + (0.0124 * hum); 
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(1);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(1);
  digitalWrite(triggerPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) * (speedOfSound/10000); 
  if (distance < 5) {
    recordValues = true;
  } 
}

void printData(float temp, float hum, float distance) {
  Serial.print("{\"t\":");
  Serial.print(temp, 1);
  Serial.print(", \"h\":");
  Serial.print(hum, 1);
  Serial.print(", \"d\":");
  Serial.print(distance, 1);
  Serial.println("}");
}


void beep(int distance, int buzzerPin) {
  if (distance < 5) {
    tone(buzzerPin, 4000);
  } else if (distance < 30) {
    int delayTime = map(distance * 2, 5, 30, 50, 300);
    for (int i = 0; i < 10; i++) {
      tone(buzzerPin, 4000, 50);
      delay(delayTime);
    }
  } else {
    noTone(buzzerPin);
  }
}



void loop() {

  if (Serial.available()) {
    String command = Serial.readString();
    if (command.indexOf("beepStart") >= 0) {
      beepSound = true;
    } else if (command.indexOf("beepEnd") >= 0) {
      beepSound = false;
    } else if (command.indexOf("recordStart") >= 0) {
      recordValues = true;
    } else if (command.indexOf("recordEnd") >= 0) {
      recordValues = false;
    }
  }

  int readDHT;
  float temp, hum;
  readDHT11(dht11, readDHT, temp, hum);

  float distance;
  measureDistance(TRIGGER_PIN, ECHO_PIN, temp, hum, distance);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if(recordValues == 1){
      printData(temp, hum, distance);
    }
  }

  if(beepSound == true){
    beep(distance, buzzerPin);
  }
}