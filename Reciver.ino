#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

struct SensorData {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
};

SensorData data;

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
  Serial.println("Receiver Ready!");
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(data));

    int GZ = data.gz;
    int AY = data.ay;

    char direction;

    if (GZ >-130) {
      direction = 'L'; // More positive GZ means left
    } else if (GZ < -200) {
      direction = 'R'; // More negative GZ means right
    } else {
      direction = 'S'; // Otherwise, it's straight
    }

    Serial.print("AY="); Serial.print(AY);
    Serial.print(" GZ="); Serial.print(GZ);
    Serial.print("  Direction: ");
    Serial.println(direction);
  }
}