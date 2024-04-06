#include "HX711.h"		// incluye libreria HX711

#define DT 2			// DT de HX711 a pin digital 2
#define SCK 3			// SCK de HX711 a pin digital 3
float peso = 0;

HX711 celda;			// crea objeto con nombre celda

void setup() {
  Serial.begin(115200);		// inicializa monitor serie a 9600 baudios
  Serial.println("Balanza con celda de carga");	// texto descriptivo

  celda.begin(DT, SCK);		// inicializa objeto con los pines a utilizar

  celda.set_scale(113.f);	// establece el factor de escala obtenido del primer programa
  celda.tare();			// realiza la tara o puesta a cero
}

void loop() {
  Serial.print("Valor (gramos): ");		// texto descriptivo
  Serial.println(celda.get_units(10), 1);	// muestra el valor obtenido promedio de 10 lecturas

  celda.power_down();				// apaga el modulo HX711
  delay(3000);					// demora de 3 segundos
  celda.power_up();				// despierta al modulo HX711
}