
void setup() {
 Serial.begin(115200); // Inicializa la comunicación serial a 9600 baudios
}

void loop() {
 if (Serial.available() > 0) { // Verifica si hay datos disponibles para leer
    String data = Serial.readStringUntil('\n'); // Lee la cadena de texto hasta el carácter de nueva línea
    int xIndex = data.indexOf("Eje X:"); // Encuentra el índice de inicio de la cadena "Eje X:"
    int yIndex = data.indexOf("Eje Y:"); // Encuentra el índice de inicio de la cadena "Eje Y:"
    int zIndex = data.indexOf("Eje Z:"); // Encuentra el índice de inicio de la cadena "Eje Z:"

    if (xIndex >= 0 && yIndex >= 0 && zIndex >= 0) {
      float x = data.substring(xIndex + 7, yIndex).toFloat(); // Extrae el valor de X
      float y = data.substring(yIndex + 7, zIndex).toFloat(); // Extrae el valor de Y
      float z = data.substring(zIndex + 7).toFloat(); // Extrae el valor de Z

      // Aquí puedes usar los valores de x, y, z como necesites
      Serial.print("Valores recibidos: X = ");
      Serial.print(x);
      Serial.print(", Y = ");
      Serial.print(y);
      Serial.print(", Z = ");
      Serial.println(z);
    }
 }
}
