import time
import serial
from Adafruit_IO import Client
import sys

# Adafruit IO credentials
ADAFRUIT_IO_KEY = 'aio_uMmf66Ik5zulgHjYDawjhSVy9Htx'
ADAFRUIT_IO_USERNAME = 'nfajg03'

# Create an instance of the Adafruit IO client
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# Initialize serial connection
nano = serial.Serial(port='COM7', baudrate=9600, timeout=None)

# obtener el valor de los pots
pot_1_feed = aio.feeds('proyecto2-pg.pot-1')
pot_2_feed = aio.feeds('proyecto2-pg.pot-2')
pot_3_feed = aio.feeds('proyecto2-pg.pot-3')
pot_4_feed = aio.feeds('proyecto2-pg.pot-4')


def send_to_arduino(data):
    nano.write(f"{data}\n".encode('utf-8'))
    print(f"Sent to Arduino: {data}")

while True:
    try:

        #leer el modo en que se encuentra todo
        modo=nano.read().decode('ascii').rstrip()
        print(modo)
        #modo 0 actualiza los potenciometros de adafruit
        if modo.startswith("0 POT1:"): 
            pot1_value = modo.split(":")[1]
            aio.send_data(pot_1_feed.key, pot1_value)
        if modo.startswith("0 POT2:"): 
            pot2_value = modo.split(":")[1]
            aio.send_data(pot_2_feed.key, pot2_value)
        if modo.startswith("0 POT3:"): 
            pot3_value = modo.split(":")[1]
            aio.send_data(pot_3_feed.key, pot3_value)
        if modo.startswith("0 POT4:"): 
            pot4_value = modo.split(":")[1]
            aio.send_data(pot_4_feed.key, pot4_value)
        if modo.startswith("3"):
            # leer potenciometros de adafruit y mandarlo al nano
            pot1_data = aio.receive(pot_1_feed.key)
            pot1_value = pot1_data.value
            print ('POT1-> BOCA=',(pot1.value).zfill(3))
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

    except KeyboardInterrupt:
        print("salimos del programa")
        if nano.is_open:
            nano.close()
        sys.exit(1)
            
