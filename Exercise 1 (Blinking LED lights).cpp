void setup()
{
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop()
{
  for (int i = 1; i <= 10; i++)
  {
    // Convert number to binary and display it on the LEDs
    digitalWrite(8, (i & 0b1000) >> 3); // Get 4th bit
    digitalWrite(7, (i & 0b0100) >> 2); // Get 3rd bit
    digitalWrite(6, (i & 0b0010) >> 1); // Get 2nd bit
    digitalWrite(5, (i & 0b0001));      // Get 1st bit

    delay(1000);
  }
}
