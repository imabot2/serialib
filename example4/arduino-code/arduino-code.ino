// Return the returns the character received on Serial multiply by two
// More details on:
// https://lucidar.me/en/serialib/cross-plateform-rs232-serial-library/

// Setup, initialize
void setup() {
  Serial.begin(115200);
}

// Loop forever
void loop() {
  // If serial data is pending, read, write twice the received byte
  if (Serial.available())
    Serial.write( 2*Serial.read() );
}