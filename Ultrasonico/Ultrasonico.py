import serial
import time

import RPi.GPIO as GPIO

# Configurar el puerto serial
ser = serial.Serial('/dev/ttyUSB0', 115200)  # Ajusta el puerto y la velocidad de baudios según corresponda

# Configurar los pines del sensor ultrasónico
TRIG_PIN = 23
ECHO_PIN = 24
GPIO.setmode(GPIO.BCM)
GPIO.setup(TRIG_PIN, GPIO.OUT)
GPIO.setup(ECHO_PIN, GPIO.IN)

def get_distance():
    # Generar un pulso de 10 microsegundos para activar el sensor
    GPIO.output(TRIG_PIN, GPIO.HIGH)
    time.sleep(0.00001)
    GPIO.output(TRIG_PIN, GPIO.LOW)

    # Medir el tiempo de respuesta del sensor
    while GPIO.input(ECHO_PIN) == 0:
        pulse_start = time.time()
    while GPIO.input(ECHO_PIN) == 1:
        pulse_end = time.time()

    # Calcular la distancia en centímetros
    pulse_duration = pulse_end - pulse_start
    distance = pulse_duration * 17150
    distance = round(distance, 2)

    return distance

try:
    while True:
        # Obtener la distancia del sensor
        distance = get_distance()

        # Enviar la distancia a través de la comunicación serial
        ser.write(str(distance).encode())

        # Esperar un segundo antes de tomar la siguiente lectura
        time.sleep(1)

except KeyboardInterrupt:
    # Detener el programa si se presiona Ctrl+C
    GPIO.cleanup()
    ser.close()