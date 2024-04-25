#include <Ultrasonic.h>
#include "HX711.h"
#include <Adafruit_TCS34725.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Giroscopio
Adafruit_MPU6050 mpu;

// Color
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS,TCS34725_GAIN_4X);
// Bascula HTX711
#define DT 46
#define SCK 47
float peso = 0;
HX711 celda;

// Ultrasonicos
Ultrasonic ultrasonicoIzq(52,53);
Ultrasonic ultrasonicoDer(50,51);
Ultrasonic ultrasonicoCen(48,49);
float rangoDistancia = 25; // Rango en cm para parar si detecta algo

// Motores
int velocidad = 100;
// Motor de Atras
int ENC = 7;
int IN5 = 6;
int IN6 = 5;
int IN7 = 4;
int IN8 = 3;
int END = 2;
// Motor de Adelante
int ENA = 13;
int IN1 = 12;
int IN2 = 11;
int IN3 = 10;
int IN4 = 9;
int ENB = 8;

void setup() {
  Serial.begin(9600);
  // Inicio de boton

  // Inicio de bascula
  celda.begin(DT, SCK);	
  celda.set_scale(115.f);	
  celda.tare();

  // Inicio de Color
  tcs.begin();

  // Inicio de Giro y configuracion del evento
  mpu.begin();
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  // Inicio de motores
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
      // Bascula
      float peso = leerPeso();
      // Giro
      int giro = deteccionGiroscopio();
      // Ultrasonicos
      float di = leerUltrasonico(ultrasonicoIzq);
      float dd = leerUltrasonico(ultrasonicoDer);
      float dc = leerUltrasonico(ultrasonicoCen);
      String Color = leerColor();
      // Impresiones
      Serial.print("GI : "); Serial.print(giro); Serial.print(" : "); Serial.println(": Giroscopio");
      Serial.print("PE : "); Serial.print(peso); Serial.print(" : gr"); Serial.println(": Bascula");
      Serial.print("UL1 : "); Serial.print(di); Serial.print(" : cm"); Serial.println(": Ultrasonico");
      Serial.print("UL2 : "); Serial.print(dd); Serial.print(" : cm"); Serial.println(": Ultrasonico");
      Serial.print("UL3 : "); Serial.print(dc); Serial.print(" : cm"); Serial.println(": Ultrasonico");
      Serial.print("CO : "); Serial.print(Color); Serial.print(" : "); Serial.println(": Color");
      if (di >= rangoDistancia || dd >= rangoDistancia || dc >= rangoDistancia) {
        stop();
      } 

      char Instruccion = Serial.read();
      if (Instruccion == "W" || Instruccion == "w") {
        adelante();
      } else if (Instruccion == "A" || Instruccion == "a") {
        izquierda();
      } else if (Instruccion == "D" || Instruccion == "d") {
        derecha();
      } else if (Instruccion == "S" || Instruccion == "s") {
        reversa();
      } 
    }
int deteccionGiroscopio() {
  if(mpu.getMotionInterruptStatus()) {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        return 1;
      } else {
        return 0;
      }
}
float leerUltrasonico(Ultrasonic ultrasonico) {
  float distancia = ultrasonico.read();
  return distancia;
}

float leerPeso() {
  float peso = 0.0;
  peso = celda.get_units(10);
  celda.power_down();
  celda.power_up();	
  celda.tare();
  return peso;
}

String leerColor() {
  float red, green, blue;
  tcs.getRGB(&red, &green, &blue);
  int R = int(red);
  int G = int(green);
  int B = int(blue);
  String color = "";
  if ((R == G == B) || ((R >= 70 && R <= 100) && (G >= 80 && G <= 110) && (B >= 70 && B <= 80))){
    color = "Negro";
  } else if ((R >= 50 && R <= 85) && (G >= 130 && G <= 150) && (B >= 60 && B <= 95)) {
    color = "Verde";
  }
  else if ((R >= 140 && R <= 200) && (G >= 50 && G <= 100) && (B >= 50 && B <= 95)) {
    color = "Rojo";
  } else {
    color = "Otro";
  }
  return color;
}

void adelante() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, velocidad);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, velocidad);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  analogWrite(ENC, velocidad);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);
  analogWrite(END, velocidad);
}

void izquierda() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);

  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, HIGH); 
  digitalWrite(IN8, LOW);
  analogWrite(ENC, velocidad);
  analogWrite(END, velocidad);
}

void derecha() {
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH);  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);

  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);
  analogWrite(ENC, velocidad);
  analogWrite(END, velocidad);

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

void reversa() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);

  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);
  analogWrite(ENC, velocidad);
  analogWrite(END, velocidad);
}