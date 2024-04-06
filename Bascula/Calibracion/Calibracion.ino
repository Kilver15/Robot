#include "HX711.h"		// incluye libreria HX711

#define DT 2			// DT de HX711 a pin digital 2
#define SCK 3			// SCK de HX711 a pin digital 3

HX711 celda;			// crea objeto con nombre celda

void setup() {
  Serial.begin(9600);		// inicializa monitor serie a 9600 baudios
  
  celda.begin(DT, SCK);		// inicializa objeto con los pines a utilizar

  Serial.println("Para obtener factor de escala:");	// texto estatico descriptivo

  celda.set_scale();		// establece en factor de escala por defecto
  celda.tare();			// realiza la tara o puesta a cero		       
  Serial.println("Colocar un peso conocido (10 seg.)");	// texto estatico descriptivo
  delay(10000);			// demora de 10 segundos para colocar el peso conocido
  Serial.println(celda.get_units(10)); 	// obtiene valor (promedio de 10 lecturas) para calcular factor de escala    
  Serial.println("Hecho. Dividir el valor mostrado por el peso colocado");	// texto descriptivo
}

void loop() {
  	// nada por aqui
}