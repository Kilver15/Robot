from hx711 import HX711
import time
import RPi.GPIO as GPIO
#ESTE CODIGO SOLO ES PARA CALIBRAR LA CELDA DE CARGA EN BASE A UN PESO QUE SE CONOCE
# POR EJEMPLO SI MI CELULAR SE QUE PESA 189gr, LO PESO Y LO QUE ME DE LO DIVIDO
#ENTRE EL PESO DEL CEL, LUEGO ESE NUMERO LO PONGO EN LA hx.set_reference_unit(113) del Bascula.py
DT = 2
SCK = 3

celda = HX711(DT, SCK)

def setup():
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(DT, GPIO.IN)
    GPIO.setup(SCK, GPIO.OUT)
    
    celda.set_scale()
    celda.tare()
    
    print("Para obtener factor de escala:")
    print("Colocar un peso conocido (10 seg.)")
    time.sleep(10)
    print(celda.get_units(10))
    print("Hecho. Dividir el valor mostrado por el peso colocado")

def loop():
    pass

if __name__ == '__main__':
    setup()
    while True:
        loop()