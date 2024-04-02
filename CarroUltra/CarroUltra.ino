#include <Adafruit_TCS34725.h>
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS,TCS34725_GAIN_4X);

// Motor Atras
int ENA = 6;
int IN1 = 7;
int IN2 = 2;
int IN3 = 5;
int IN4 = 4;
int ENB = 3;

// Motor Adelante
int ENC = 10;
int IN5 = 8;
int IN6 = 9;
int IN7 = 12;
int IN8 = 13;
int END = 11;

void setup() {
  //Color
  Serial.begin(115200);
  tcs.begin();

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(ENC, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  pinMode(END, OUTPUT);

}

void loop() {
  String arduinoColor = readColor();
  //Adaptar esto para que reciba el color que envia la rasp
  String piColor = "BLanco";
  if (Serial.available() > 0) {
    String piColor = Serial.readStringUntil('\n');
    piColor.trim();
    Serial.println(piColor);
  }

  String movimiento = makeMovementDecision(arduinoColor, piColor);
  Serial.println("Movimiento: " + movimiento);
  delay(100);
}


void adelante() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 100);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 100);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  analogWrite(ENC, 100);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);
  analogWrite(END, 100);
}

void derecha() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);

  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, HIGH); 
  digitalWrite(IN8, LOW);
  analogWrite(ENC, 100);
  analogWrite(END, 100);
}

void izquierda() {
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH);  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);

  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);
  analogWrite(ENC, 100);
  analogWrite(END, 100);

}

void stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, LOW);
  analogWrite(ENC, 0);
  analogWrite(END, 0);
}

String readColor() {
  float red, green, blue;
  tcs.getRGB(&red, &green, &blue);
  int R = int(red);
  int G = int(green);
  int B = int(blue);
  String color = "";
  if ((R <= 110 && G <= 110 && B <= 110)) {
    color = "Negro";
  } else if ((R <= 120 && G >= 100 && B >= 70) || ((G - R) >= 20 && (G - B) >= 15)) {
    color = "Verde";
  } else if ((R >= 120 && G <= 105 && B >= 70) || ((R - G) >= 50 && (R - B) >= 50)) {
    color = "Rojo";
  } else {
    color = "Otro"; // "Indefinido" o categoría específica según necesidades
  }

   Serial.print("R: ");Serial.print(int(red));
   Serial.print("   G: ");Serial.print(int(green));
   Serial.print("   B: ");Serial.print(int(blue));
  Serial.print("   Color: ");Serial.print(color);
  Serial.println();
  return color;
}

String makeMovementDecision(String arduinoColor, String piColor) {
  if (arduinoColor == "Negro" && piColor == "Negro") {
    stop();
    return "Parar";
  } else if (arduinoColor != "Negro" && piColor == "Negro") {
    derecha();
    return "Derecha";
  } else if (arduinoColor == "Negro" && piColor != "Negro") {
    izquierda();
    return "Izquierda";
  } else {
    adelante();
    return "Adelante";
  }
}
