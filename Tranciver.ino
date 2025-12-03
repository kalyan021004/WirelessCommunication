
/*********************************
 * TRANSMITTER – SEND ACCEL + GYRO (MPU6050)
 *********************************/

#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <MPU6050.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);
MPU6050 mpu;

const byte address[6] = "00001";

// Struct for sending all data
struct SensorData {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
};

SensorData data;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize MPU6050
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 NOT CONNECTED!");
    while (1);
  }
  Serial.println("MPU6050 READY!");

  // Initialize nRF24L01
  if (!radio.begin()) {
    Serial.println("NRF24 NOT RESPONDING!");
    while (1);
  }

  radio.openWritingPipe(address);
  radio.stopListening();
  radio.setPALevel(RF24_PA_LOW);
  Serial.println("TRANSMITTER READY!");
}

void loop() {
  // Read Accelerometer
  mpu.getAcceleration(&data.ax, &data.ay, &data.az);

  // Read Gyroscope
  mpu.getRotation(&data.gx, &data.gy, &data.gz);

  // Send the struct
  radio.write(&data, sizeof(data));

  // Debug print
  Serial.print("AX="); Serial.print(data.ax);
  Serial.print(" AY="); Serial.print(data.ay);
  Serial.print(" AZ="); Serial.print(data.az);
  Serial.print("  GX="); Serial.print(data.gx);
  Serial.print(" GY="); Serial.print(data.gy);
  Serial.print(" GZ="); Serial.println(data.gz);

  delay(100);  // 10 times per second
}