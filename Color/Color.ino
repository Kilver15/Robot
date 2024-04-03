
void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    String color = Serial.readStringUntil('\n');

    if (color == "Negro") {
      // Actions for black color
      Serial.println("Avanza");
    } else if (color == "Verde") {
      // Actions for green color
      Serial.println("Gira");
    } else if (color == "Rojo") {
      // Actions for red color
      Serial.println("Para");
    } else {
      // Actions for other colors
    }
  }
}
