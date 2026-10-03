/***********************************************************************
 * Engineered by : Ampnics
 * Project       : ProxiWave-D
 * Version       : v0.1.0
 * Date          : Dec 2025
 * Author        : Md Ammar Maniyar
 *
 * Test Name     : VL53L0CX Distance Measurement Test
 * Description   : Validates I2C communication and ranging functionality.
 * MCU           : ATmega328P @ 3.3V, 8MHz
 *
 * Interface     :
 *   SDA -> PC4 (A4)
 *   SCL -> PC5 (A5)
 *
 * Library       : VL53L0X by Pololu
 *
 * Test Method   :
 *   - Initializes sensor
 *   - Reads distance in continuous mode
 *   - Prints distance over UART
 *
 * Expected Result:
 *   - Serial output shows distance in millimeters
 *   - Distance decreases when object moves closer
 ***********************************************************************/

#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!sensor.init()) {
    Serial.println("VL53L0CX init failed");
    while (1);
  }

  sensor.setTimeout(500);
  sensor.startContinuous();

  Serial.println("VL53L0CX Ready");
}

void loop() {
  uint16_t distance = sensor.readRangeContinuousMillimeters();

  if (sensor.timeoutOccurred()) {
    Serial.println("Sensor timeout");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");
  }

  delay(200);
}
