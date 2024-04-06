int rangoDeDistancia = 20; // Rango en cm para parar si detecta algo
int distancia = 25;

void setup() {
 Serial.begin(115200);
}

void loop() {
 if (Serial.available() > 0) { // Verifica si hay datos disponibles para leer
    String data = Serial.readStringUntil('\n'); // Lee la cadena de texto hasta el carácter de nueva línea
    int xIndex = data.indexOf("Eje X:"); // Encuentra el índice de inicio de la cadena "Eje X:"
    int yIndex = data.indexOf("Eje Y:"); // Encuentra el índice de inicio de la cadena "Eje Y:"
    int zIndex = data.indexOf("Eje Z:"); // Encuentra el índice de inicio de la cadena "Eje Z:"
    int distIndex = data.indexOf("Distancia:"); // Encuentra el índice de inicio de la cadena "Distancia:"
    int colorIndex = data.indexOf("Color:");
    int pesoIndex = data.indexOf("Peso:");

    if (xIndex >= 0 && yIndex >= 0 && zIndex >= 0 && distIndex >= 0) {
      float x = data.substring(xIndex + 7, yIndex).toFloat(); // Extrae el valor de X
      float y = data.substring(yIndex + 7, zIndex).toFloat(); // Extrae el valor de Y
      float z = data.substring(zIndex + 7).toFloat(); // Extrae el valor de Z
      distancia = data.substring(distIndex + 10).toInt(); // Extrae el valor de distancia
      String color = data.substring(colorIndex + 7, data.indexOf(',', colorIndex)); // Asegúrate de extraer solo el nombre del color
      float peso = data.substring(pesoIndex + 6).toFloat();

      // Aquí puedes usar los valores de x, y, z y distancia como necesites
      Serial.print("Valores recibidos: X = ");
      Serial.print(x);
      Serial.print(", Y = ");
      Serial.print(y);
      Serial.print(", Z = ");
      Serial.print(z);
      Serial.print(", Distancia = ");
      Serial.print(distancia);
      Serial.print(", Color = ");
      Serial.print(color);
      Serial.print(", Peso = ");
      Serial.println(peso);

      // Lógica de control del robot basada en la distancia y los valores del giroscopio
      if (distancia > rangoDeDistancia) {
        // Robot avanza
        Serial.println("Sigue");
      } else {
        // Robot para
        Serial.println("Para");
      }

      if (strcmp(color.c_str(), "Negro") == 0) {
        // Actions for black color
        Serial.println("Avanza");
        } else if (strcmp(color.c_str(), "Verde") == 0) {
        // Actions for green color
        Serial.println("Gira");
        } else if (strcmp(color.c_str(), "Rojo") == 0) {
        // Actions for red color
        Serial.println("Para");
        } else {
        // Actions for other colors
        Serial.println("Otro");
        }
    }
  }
}
