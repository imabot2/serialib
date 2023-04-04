// Return the returns the character received on Serial after capitalizing it
// More details on:
// https://lucidar.me/en/serialib/cross-plateform-rs232-serial-library/

// Setup, initialize
void setup() {
  Serial.begin(115200);
}

// Loop forever
void loop() {
  // If serial data is pending, read, capitalize and write the character
  if (Serial.available())
    Serial.write( toupper (Serial.read()) );
}