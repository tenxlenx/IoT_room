// i2c slave program for the arduino

#include <Wire.h>
#include <dht.h>

#define SLAVE_ADDRESS 0x29 // slave address (arduino)

dht DHT;

#define DHT11_PIN 2

// sensor values
volatile int tank0;                        
volatile int tank1;                        
volatile int tank2;                        
volatile int tank3;                        
volatile int temperature;
volatile int humidity;

long duration;

const int trigPin1 = 3;
const int echoPin1 = 4;

const int trigPin2 = 5;
const int echoPin2 = 6;

const int trigPin3 = 7;
const int echoPin3 = 8;

const int trigPin4 = 9;
const int echoPin4 = 10;

// setup sensors and i2c connection
void setup() 
{

  //init serial communication at 9600 bits per sec
  Serial.begin(9600); 

  // setup i2c with slave address
  Wire.begin(SLAVE_ADDRESS);

  // if Master requests data -> call requestEvent() 
  Wire.onRequest(requestEvent);

  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin2,INPUT);
  pinMode(trigPin3,OUTPUT);
  pinMode(echoPin3,INPUT);
  pinMode(trigPin4,OUTPUT);
  pinMode(echoPin4,INPUT);

}

void loop() 
{

  temperature = DHT.temperature;
  humidity = DHT.humidity;
  
  tank0 = 0;
  tank1 = 0;
  tank2 = 0;
  tank3 = 0;
  
  // ultrasonic init low (reset)
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
  digitalWrite(trigPin3, LOW);
  digitalWrite(trigPin4, LOW);
  delayMicroseconds(2);
  
  tank0 =calculateDistance(trigPin1, echoPin1);
  tank1 =calculateDistance(trigPin2, echoPin2);
  tank2 =calculateDistance(trigPin3, echoPin3);
  tank3 =calculateDistance(trigPin4, echoPin4);
 
  Serial.print("cm = ");
  Serial.println(tank0);
  
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  delay(2000);
 
}

// calculates the distance using sonar sensors
float calculateDistance(const int &trigpin, const int &echopin) {
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  long response_time = pulseIn(echopin, HIGH);
  float dist = response_time * 0.034/2;
  return dist;
}

// if i2c master requests data, this is what we send
void requestEvent() 
{
  
  uint8_t sensor_array[6]; 
  
  sensor_array[0] = tank0;
  sensor_array[1] = tank1;
  sensor_array[2] = tank2;
  sensor_array[3] = tank3;
  sensor_array[4] = temperature;
  sensor_array[5] = humidity;

  // send readings (i2c) to Master
  int result = Wire.write(sensor_array,6);
  
}


