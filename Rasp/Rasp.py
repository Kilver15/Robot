import smbus
import serial
import time
from hx711 import HX711
import RPi.GPIO as GPIO

bus = smbus.SMBus(1)
TCS34725_ADDRESS = 0x29
MPU5060_ADDRESS = 0x68
GYRO_XOUT_H = 0x43
GYRO_XOUT_L = 0x44
GYRO_YOUT_H = 0x45
GYRO_YOUT_L = 0x46
GYRO_ZOUT_H = 0x47
GYRO_ZOUT_L = 0x48

ser = serial.Serial('/dev/ttyUSB0', 115200)

def read_gyro():
    x = (bus.read_byte_data(MPU5060_ADDRESS, GYRO_XOUT_H) << 8) | bus.read_byte_data(MPU5060_ADDRESS, GYRO_XOUT_L)
    y = (bus.read_byte_data(MPU5060_ADDRESS, GYRO_YOUT_H) << 8) | bus.read_byte_data(MPU5060_ADDRESS, GYRO_YOUT_L)
    z = (bus.read_byte_data(MPU5060_ADDRESS, GYRO_ZOUT_H) << 8) | bus.read_byte_data(MPU5060_ADDRESS, GYRO_ZOUT_L)
    
    x = x / 131.0
    y = y / 131.0
    z = z / 131.0
    
    return x, y, z

TRIG_PIN = 23
ECHO_PIN = 24
GPIO.setmode(GPIO.BCM)
GPIO.setup(TRIG_PIN, GPIO.OUT)
GPIO.setup(ECHO_PIN, GPIO.IN)

DT = 2
SCK = 3
hx = HX711(DT, SCK)
hx.set_reading_format("MSB")
hx.set_reference_unit(113)
hx.reset()
hx.tare()

def get_distance():
    GPIO.output(TRIG_PIN, GPIO.HIGH)
    time.sleep(0.00001)
    GPIO.output(TRIG_PIN, GPIO.LOW)
    while GPIO.input(ECHO_PIN) == 0:
        pulse_start = time.time()
    while GPIO.input(ECHO_PIN) == 1:
        pulse_end = time.time()
    
    pulse_duration = pulse_end - pulse_start
    distance = pulse_duration * 17150
    distance = round(distance, 2)
    return distance

def get_color():
    bus.write_byte_data(TCS34725_ADDRESS, 0x00, 0x03)
    time.sleep(0.1)
    color_data = bus.read_i2c_block_data(TCS34725_ADDRESS, 0x0C, 8)
    
    red = color_data[1] << 8 | color_data[0]
    green = color_data[3] << 8 | color_data[2]
    blue = color_data[5] << 8 | color_data[4]
    
    R = int(red)
    G = int(green)
    B = int(blue)
    
    color = ""
    if R <= 110 and G <= 110 and B <= 110:
        color = "Negro"
    elif (R <= 120 and G >= 100 and B >= 70) or ((G - R) >= 20 and (G - B) >= 15):
        color = "Verde"
    elif (R >= 120 and G <= 105 and B >= 70) or ((R - G) >= 50 and (R - B) >= 50):
        color = "Rojo"
    else:
        color = "Otro"
    return color

def get_peso():
    peso = hx.get_weight(10)
    hx.power_down()
    time.sleep(3)
    hx.power_up()
    return peso

while True:
    distance = get_distance()
    x, y, z = read_gyro()
    color = get_color()
    peso = get_peso()
    
    data = f"Eje X: {x}, Eje Y: {y}, Eje Z: {z}, Distancia: {distance}, Color: {color}, Peso: {peso}"
    ser.write(data.encode())
    
    time.sleep(1)

GPIO.cleanup()
