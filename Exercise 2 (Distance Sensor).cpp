// Re-generated with CoPilot, because the original code wasn't saved

const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance
  // Speed of sound = 343m/s = 0.0343 cm/microsecond
  // Distance = (duration × speed of sound) / 2
  distance = duration * 0.0343 / 2;
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  delay(500);
}