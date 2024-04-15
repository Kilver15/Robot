#include <Ultrasonic.h>
#include "HX711.h"
#include <Adafruit_TCS34725.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Boton
const int botonpin = 22;

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
float rangoDistancia = -10; // Rango en cm para parar si detecta algo

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

// Variables para instrucciones
String instruccion = "";
int caracterActual = 0;
String raspColor = "";
bool ciclo = true;

void setup() {
  Serial.begin(9600);
  // Inicio de boton
  pinMode(botonpin, INPUT_PULLUP);

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
  if (Serial.available() > 0) {
    if (instruccion.length() == 0) {
      instruccion = Serial.readStringUntil('\n');
      // Serial.println("Instruccion recibida");
      caracterActual = 0;
      ciclo = true;
    } 
    while(ciclo) {
      // Bascula
      float peso = leerPeso();
      // Boton
      int boton = leerBoton();
      // Giro
      int giro = deteccionGiroscopio();
      // Ultrasonicos
      float di = leerUltrasonico(ultrasonicoIzq);
      float dd = leerUltrasonico(ultrasonicoDer);
      float dc = leerUltrasonico(ultrasonicoCen);
      // Colores 1 y 2
      if(Serial.available() > 0) {
        raspColor = Serial.readStringUntil('\n');
      } else {
        raspColor = "Otro";
      }
      String ardColor = leerColor();
      // Impresiones
      Serial.print("BO : "); Serial.print(boton); Serial.println(" ");
      Serial.print("GI : "); Serial.print(giro); Serial.println(" ");
      Serial.print("PE : "); Serial.print(peso); Serial.println(" : gr");
      Serial.print("UL1 : "); Serial.print(di); Serial.println(" : cm");
      Serial.print("UL2 : "); Serial.print(dd); Serial.println(" : cm");
      Serial.print("UL3 : "); Serial.print(dc); Serial.println(" : cm");
      Serial.print("COA : "); Serial.print(ardColor); Serial.println(" ");
      Serial.print("COR : "); Serial.print(raspColor); Serial.println(" ");
      if (di <= rangoDistancia || dd <= rangoDistancia || dc <= rangoDistancia) {
        stop();
      } else {
        compararColores(raspColor, ardColor);
        if (caracterActual == instruccion.length()) {
          instruccion = "";
        }
      }
    }
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

int leerBoton() {
  if(digitalRead(botonpin) == HIGH) {
    return 1;
  } else {
    return 0;
  }
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
   Serial.print("R: ");Serial.print(int(red));
   Serial.print("   G: ");Serial.print(int(green));
   Serial.print("   B: ");Serial.print(int(blue));
  //  Serial.print("   Color: ");Serial.println(color);
  return color;
}

void compararColores(String raspColor, String ardColor) {
  if (raspColor == "Verde" || ardColor == "Verde") {
    char caracter = instruccion.charAt(caracterActual);
    elegirCamino(caracter);
    caracterActual ++;
    Serial.println(instruccion.length());
  } else if (raspColor == "Negro" && ardColor == "Otro") {
    izquierda();
  } else if (raspColor == "Otro" && ardColor == "Negro") {
    derecha();
  } else if (raspColor == "Negro" && ardColor == "Negro") {
    stop();
  } else if (raspColor == "Otro" && ardColor == "Otro") {
    adelante();
  } else if (raspColor == "Rojo" || ardColor == "Rojo") {
    stop();
    ciclo = false;
    caracterActual = 0;
    instruccion = "";
  }
}

void elegirCamino(char currentChar) {
  if(currentChar == 'I' || currentChar == 'i'){
    izquierda();
    delay(2000);
    adelante();
    // Serial.println("Izq");
  }
  else if(currentChar == 'D' || currentChar == 'd'){
    derecha();
    delay(2000);
    adelante();
    // Serial.println("Der");
  }
  else if(currentChar == 'C' || currentChar == 'c'){
    adelante();
    // Serial.println("Seg");
  }
  delay(1000);
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