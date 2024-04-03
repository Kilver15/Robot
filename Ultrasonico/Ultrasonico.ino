int rangoDeDistancia = 20; // Rango en cm para parar si detecta algo
int distancia = 25;

void setup() {
 Serial.begin(115200);
}

void loop() {
 while (Serial.available()) {
    distancia = Serial.parseInt(); // LEER DISTANCIA DEL ULTRASONICO
    if (distancia > rangoDeDistancia) {
    // Robot avanza
    Serial.println("Avanza");
 } else {
    // Robot para
    Serial.println("Para");
 }
 Serial.println(distancia);
 }
 
}
