#include <DHT11.h>
#include <SoftwareSerial.h>

DHT11 dht11(12);

#define RX 5
#define TX 4
SoftwareSerial bluetooth(RX, TX);

void setup() {
  pinMode(13, OUTPUT);
  bluetooth.begin(9600);
  Serial.begin(9600);
}




void loop() {

int humidity = 0;
int temperature = 0;
bool fan_state = 0;
bool auto_mode = 0;
int delay_count = 0;
int result;
char msg;

  while(1) {
    msg = 0;


    if (bluetooth.available()) {
      msg = bluetooth.read();
      Serial.print("msg: ");
      Serial.println(msg);
    }

    // A 1-second delay is required to ensure stable sensor readings.
    if (delay_count == 0) {
      result = dht11.readTemperatureHumidity(temperature, humidity);
    }


    if (result == 0) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C\tHumidity: ");
        Serial.print(humidity);
        Serial.print(" %\t");
    } else {
        // Print error message based on the error code.
        Serial.println(DHT11::getErrorString(result));
    }



    if (msg == '1') {
      fan_state = !fan_state;
      auto_mode = 0;
    }
    else if (msg == '3') {
      auto_mode = !auto_mode;
    } 


    if (auto_mode) {
      if (humidity >= 50) {
        fan_state = 1;
      }
      else fan_state = 0;
    }


    if (fan_state) {
      digitalWrite(13, HIGH);
    }
    else digitalWrite(13, LOW);


    Serial.print("fan_state: ");
    Serial.print(fan_state);
    Serial.print("\tauto_mode: ");
    Serial.println(auto_mode);

    bluetooth.print(temperature);
    bluetooth.print(" "); 
    bluetooth.print(humidity);
    bluetooth.print(" "); 
    bluetooth.print((int)fan_state);
    bluetooth.print(" "); 
    bluetooth.println((int)auto_mode);

    // Signals are transmitted and received quickly.
    delay(100);

    delay_count++;
    if (delay_count >=10) {
      delay_count = 0;
    }

  }
}
