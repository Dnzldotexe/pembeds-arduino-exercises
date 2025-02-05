// Re-generated with CoPilot, because the original code wasn't saved

const int buzzerPin = 8;

// Define musical notes frequencies
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

// Create arrays for the melody
int melody[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4,
  NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4
};

int noteDurations[] = {
  250, 250, 250, 250,
  250, 250, 250, 250,
  250, 250, 250, 250,
  250, 250, 250, 250
};

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  playMelody();
  
  delay(1000);
}

void playMelody() {
  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
    tone(buzzerPin, melody[i]);
    Serial.print("Playing note: ");
    Serial.println(melody[i]);
    
    delay(noteDurations[i]);
    
    noTone(buzzerPin);
    delay(50);
  }
}