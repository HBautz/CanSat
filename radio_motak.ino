#include <Cansat_RFM96.h>
#define USE_SD 0

Cansat_RFM96 rfm96 (433350, USE_SD);
unsigned long time_counter=0;

void setup() {
  Serial.begin(9600);
  while(!Serial);

  Serial.println("Starting Setup");

  if (!rfm96.init()){
    Serial.println("Init of radio failed, stopping");
    while(1);
  }

  Serial.println("Found rfm radio, and it is working as expected");

  rfm96.setTxPower(5);

  Serial.println("End of Setup");
  Serial.println();

}

void loop() {
  uint8_t read_value;

  while (rfm96.available()){
    time_counter = millis();

    read_value = rfm96.read();

    Serial.write(read_value);
  }

  if (millis()-time_counter > 5000) {
    time_counter = millis();
    Serial.println("We hav not received anything in 5 seconds");
 
  }

}
