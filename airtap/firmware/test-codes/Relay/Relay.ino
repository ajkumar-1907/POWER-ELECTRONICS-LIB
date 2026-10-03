/***********************************************************************
 * Engineered by : Ampnics
 * Project       : ProxiWave-D
 * Version       : v0.1.0
 * Date          : Dec 2025
 * Author        : Md Ammar Maniyar
 *
 * Test Name     : Relay Coil Driver Test
 * Description   : Validates relay MOSFET driver and flyback diode.
 * MCU           : ATmega328P @ 3.3V, 8MHz
 *
 * GPIO Mapping  :
 *   RELAY -> PC2 (A2)
 *
 * Test Method   :
 *   - Relay ON for 2 seconds
 *   - Relay OFF for 2 seconds
 *   - Repeats continuously
 *
 * Expected Result:
 *   - Audible relay click ON and OFF every 2 seconds
 ***********************************************************************/

#define RELAY_PIN A2

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  digitalWrite(RELAY_PIN, HIGH);  // Relay ON
  delay(2000);

  digitalWrite(RELAY_PIN, LOW);   // Relay OFF
  delay(2000);
}
