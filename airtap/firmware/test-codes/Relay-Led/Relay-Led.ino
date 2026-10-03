/***********************************************************************
 * Engineered by : Ampnics
 * Project       : ProxiWave-D
 * Version       : v0.1.0
 * Date          : Dec 2025
 * Author        : Md Ammar Maniyar
 *
 * Test Name     : Relay Status LED Test
 * Description   : Verifies relay status LED polarity and resistor value.
 * MCU           : ATmega328P @ 3.3V, 8MHz
 *
 * GPIO Mapping  :
 *   RELAY_LED -> PC1 (A1)
 *
 * Test Method   :
 *   - LED ON for 500 ms
 *   - LED OFF for 500 ms
 *
 * Expected Result:
 *   - LED blinks at ~1 Hz
 ***********************************************************************/

#define RELAY_LED_PIN A1

void setup() {
  pinMode(RELAY_LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(RELAY_LED_PIN, HIGH);
  delay(500);

  digitalWrite(RELAY_LED_PIN, LOW);
  delay(500);
}
