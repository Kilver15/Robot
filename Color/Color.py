import smbus
import time
import serial

# Define I2C bus and sensor address
bus = smbus.SMBus(1)  # Adjust bus number if needed
TCS34725_ADDRESS = 0x29

# Define serial port
ser = serial.Serial('COM4', 115200)  # Adjust port and baudrate if needed

# Function to read sensor data (replace with library calls if using a library)
def read_color_data():
    # I2C communication commands (replace with library calls)
    bus.write_byte_data(TCS34725_ADDRESS, 0x00, 0x03)  # Enable ALS mode
    time.sleep(0.1)  # Integration time (adjust based on library)
    data = bus.read_i2c_block_data(TCS34725_ADDRESS, 0x0C, 8)  # Read data

    # Extract RGB values (replace with library calls)
    red = data[1] << 8 | data[0]
    green = data[3] << 8 | data[2]
    blue = data[5] << 8 | data[4]

    return red, green, blue

# Main loop (modify logic based on library functions)
while True:
    red, green, blue = read_color_data()
    # Convert values to integers (adjust based on library)
    R = int(red)
    G = int(green)
    B = int(blue)

    # Color detection logic (modify based on chosen thresholds)
    color = ""
    if R <= 110 and G <= 110 and B <= 110:
        color = "Negro"
    elif (R <= 120 and G >= 100 and B >= 70) or ((G - R) >= 20 and (G - B) >= 15):
        color = "Verde"
    elif (R >= 120 and G <= 105 and B >= 70) or ((R - G) >= 50 and (R - B) >= 50):
        color = "Rojo"
    else:
        color = "Otro"  # "Indefinido" o categoría específica según necesidades

    # Send color to Arduino via serial
    
    ser.write(f"Color: {color}".encode())

    time.sleep(0.5)  # Adjust delay as needed
