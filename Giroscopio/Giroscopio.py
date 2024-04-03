import smbus
import serial

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
ser = serial.Serial('/dev/ttyUSB0', 115200)  # Replace '/dev/ttyUSB0' with the appropriate serial port

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

# Ejemplo de uso
x, y, z = read_gyro()
data = f"Eje X: {x}, Eje Y: {y}, Eje Z: {z}"
ser.write(data.encode())  # Send the data via serial
print(data)