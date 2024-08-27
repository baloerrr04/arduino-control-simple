// Pin Ultrasonic
#define trigPin 7
#define echoPin 6

// pin Led
#define led 11

// Flag untuk menandakan status fungsi LED dan Ultrasonic
bool isFunctionActive = true;

void setup() {
  Serial.begin(19200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  long duration, distance;

  if (isFunctionActive) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration / 2) / 29.1;
  }

  if (Serial.available() > 0) {
    char data = Serial.read();
    if (!data) Serial.end();

    if (data == '1' && !isFunctionActive) {
      // Menghidupkan fungsi LED dan Ultrasonic
      isFunctionActive = true;
    }

    if (data == '2') {
      // Mematikan fungsi LED dan Ultrasonic
      isFunctionActive = false;
      digitalWrite(led, LOW);
    }
  }

  if (isFunctionActive && distance < 16) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }

  Serial.print("Jarak: ");
    Serial.print(distance);
    Serial.println(" cm");
}
