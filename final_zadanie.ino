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