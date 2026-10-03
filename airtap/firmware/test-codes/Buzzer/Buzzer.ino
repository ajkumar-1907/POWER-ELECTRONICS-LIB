/***********************************************************************
 * Engineered by : Ampnics
 * Project       : ProxiWave-D
 * Version       : v0.1.0
 * Date          : Dec 2025
 * Author        : Md Ammar Maniyar
 *
 * Test Name     : Buzzer Functional Test
 * Description   : Verifies buzzer driver circuit using GPIO and PWM.
 * MCU           : ATmega328P @ 3.3V, 8MHz
 *
 * GPIO Mapping  :
 *   BUZZER  -> PC3 (A3)
 *
 * Test Method   :
 *   - Generates a 2kHz tone for 300 ms
 *   - Silence for 700 ms
 *   - Repeats continuously
 *
 * Expected Result:
 *   - Audible beep once every second
 ***********************************************************************/

#define BUZZER_PIN A3

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  tone(BUZZER_PIN, 2000);   // 2 kHz tone
  delay(300);

  noTone(BUZZER_PIN);       // Stop buzzer
  delay(700);
}
