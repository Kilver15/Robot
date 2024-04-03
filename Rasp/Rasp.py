import smbus
import serial
import time

import RPi.GPIO as GPIO

# Define I2C bus and sensor address
bus = smbus.SMBus(1)  # Adjust bus number if needed
TCS34725_ADDRESS = 0x29

# Define serial port
ser = serial.Serial('/dev/ttyUSB0', 115200)  # Adjust port and baudrate if needed

# Dirección I2C del sensor MPU5060
MPU5060_ADDRESS = 0x68

# Registros del giroscopio
GYRO_XOUT_H = 0x43
GYRO_XOUT_L = 0x44
GYRO_YOUT_H = 0x45
GYRO_YOUT_L = 0x46
GYRO_ZOUT_H = 0x47
GYRO_ZOUT_L = 0x48

# Inicializar el bus I2C
bus = smbus.SMBus(1)

# Inicializar la comunicación serial
ser = serial.Serial('/dev/ttyUSB0', 115200) # Ajusta el puerto y la velocidad de baudios según corresponda

def read_gyro():
    # Leer los valores de los ejes X, Y y Z del giroscopio
    x = (bus.read_byte_data(MPU5060_ADDRESS, GYRO_XOUT_H) << 8) | bus.read_byte_data(MPU5060_ADDRESS, GYRO_XOUT_L)
    y = (bus.read_byte_data(MPU5060_ADDRESS, GYRO_YOUT_H) << 8) | bus.read_byte_data(MPU5060_ADDRESS, GYRO_YOUT_L)
    z = (bus.read_byte_data(MPU5060_ADDRESS, GYRO_ZOUT_H) << 8) | bus.read_byte_data(MPU5060_ADDRESS, GYRO_ZOUT_L)
    
    # Convertir los valores a unidades adecuadas (por ejemplo, grados por segundo)
    x = x / 131.0
    y = y / 131.0
    z = z / 131.0
    
    return x, y, z

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

        # Obtener los valores del giroscopio
        x, y, z = read_gyro()

        # Read color data
        bus.write_byte_data(TCS34725_ADDRESS, 0x00, 0x03)  # Enable ALS mode
        time.sleep(0.1)  # Integration time (adjust based on library)
        color_data = bus.read_i2c_block_data(TCS34725_ADDRESS, 0x0C, 8)  # Read data

        # Extract RGB values
        red = color_data[1] << 8 | color_data[0]
        green = color_data[3] << 8 | color_data[2]
        blue = color_data[5] << 8 | color_data[4]

        # Convert values to integers
        R = int(red)
        G = int(green)
        B = int(blue)

        # Color detection logic
        color = ""
        if R <= 110 and G <= 110 and B <= 110:
            color = "Negro"
        elif (R <= 120 and G >= 100 and B >= 70) or ((G - R) >= 20 and (G - B) >= 15):
            color = "Verde"
        elif (R >= 120 and G <= 105 and B >= 70) or ((R - G) >= 50 and (R - B) >= 50):
            color = "Rojo"
        else:
            color = "Otro"

        # Enviar la distancia y los valores del giroscopio a través de la comunicación serial
        data = f"Eje X: {x}, Eje Y: {y}, Eje Z: {z}, Distancia: {distance}, Color: {color}"
        ser.write(data.encode())
        # Wait for a second before taking the next reading
        time.sleep(1)

except KeyboardInterrupt:
    # Detener el programa si se presiona Ctrl+C
    GPIO.cleanup()
    ser.close()
