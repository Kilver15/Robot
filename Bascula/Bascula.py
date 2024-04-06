from hx711 import HX711
import time
import serial

DT = 2  # DT of HX711 to GPIO pin 2
SCK = 3  # SCK of HX711 to GPIO pin 3

hx = HX711(DT, SCK)
hx.set_reading_format("MSB")
hx.set_reference_unit(113)
hx.reset()
hx.tare()

ser = serial.Serial('/dev/ttyUSB0', 115200)  # Replace '/dev/ttyUSB0' with the appropriate serial port

while True:
    weight = hx.get_weight(10)
    print("Valor (gramos): ", weight)
    ser.write(str(weight).encode())  # Send weight via serial
    hx.power_down()
    time.sleep(3)  # delay of 3 seconds
    hx.power_up()
