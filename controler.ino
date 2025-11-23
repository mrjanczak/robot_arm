#include <Servo.h>

char inputString[19];   // 18 znaków + terminator '\0'
int numbers[6];
byte indexPos = 0;
bool ready = false;

Servo servos[6];
const int servoPins[6] = {11,10,9,6,5,3};

void setup() {
  Serial.begin(9600);
  Serial.println("Robot Arm init...");

  for (int i = 0; i < 6; i++) {
    servos[i].attach(servoPins[i]);
  }
}

void loop() {

  // Odczyt z portu szeregowego
  while (Serial.available() > 0 && !ready) {
    char c = Serial.read();

    // ignorujemy znaki CR/LF
    if (c == '\n' || c == '\r') continue;

    inputString[indexPos++] = c;

    if (indexPos >= 18) {
      inputString[18] = '\0'; // zakończ string
      ready = true;
    }
  }

  if (ready) {
    for (int i = 0; i < 6; i++) {
      char buf[4];  
      strncpy(buf, &inputString[i * 3], 3);
      buf[3] = '\0';
      numbers[i] = atoi(buf);
    }
    for (int i = 0; i < 6; i++) {
      int angle = constrain(numbers[i], 0, 180);   // bezpieczeństwo
      servos[i].write(angle);
      Serial.print(angle);  
      Serial.print(' ');        
    }
    Serial.println();  
    indexPos = 0;
    ready = false;
  }
}

