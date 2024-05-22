import time
import serial
from Adafruit_IO import Client

# Adafruit IO credentials
ADAFRUIT_IO_KEY = 'aio_uMmf66Ik5zulgHjYDawjhSVy9Htx'
ADAFRUIT_IO_USERNAME = 'nfajg03'

# Create an instance of the Adafruit IO client
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# Initialize serial connection
nano = serial.Serial(port='COM7', baudrate=9600, timeout=None)

# obtener el valor de los pots
pot_1_feed = aio.feeds('POT_1')
pot_2_feed = aio.feeds('POT_2')
pot_3_feed = aio.feeds('POT_3')
pot_4_feed = aio.feeds('POT_4')


def send_to_arduino(data):
    ser.write(f"{data}\n".encode('utf-8'))
    print(f"Sent to Arduino: {data}")

while True:
    try:

        #leer el modo en que se encuentra todo
        modo=arduino.read()
        #modo 0 actualiza los potenciometros de adafruit
        if arduino_data.startswith("0 POT1:"): 
            pot1_value = arduino_data.split(":")[1]
            aio.send_data(pot_1_feed.key, pot1_value)
        if arduino_data.startswith("0 POT2:"): 
            pot2_value = arduino_data.split(":")[1]
            aio.send_data(pot_2_feed.key, pot2_value)
        if arduino_data.startswith("0 POT3:"): 
            pot3_value = arduino_data.split(":")[1]
            aio.send_data(pot_3_feed.key, pot3_value)
        if arduino_data.startswith("0 POT4:"): 
            pot4_value = arduino_data.split(":")[1]
            aio.send_data(pot_4_feed.key, pot4_value)
        if arduino_data.startswith("3"):
            # leer potenciometros de adafruit y mandarlo al nano
            pot1_data = aio.receive(pot_1_feed.key)
            pot1_value = pot1_data.value
            send_to_arduino(f"POT1:{pot1_value}")

            pot2_data = aio.receive(pot_2_feed.key)
            pot2_value = pot2_data.value
            send_to_arduino(f"POT2:{pot2_value}")

            pot3_data = aio.receive(pot_3_feed.key)
            pot3_value = pot_data.value
            send_to_arduino(f"POT3:{pot3_value}")

            pot4_data = aio.receive(pot_4_feed.key)
            pot4_value = pot_data.value
            send_to_arduino(f"POT4:{pot4_value}")

        # Add a delay to avoid overwhelming the serial communication
        time.sleep(2)

    except Exception as e:
        print(f"Error: {e}")
        time.sleep(5)
