import serial
import time
import random
import math

start_time = time.time()

def get_oscillating_value(freq=1):
    """Return a value oscillating between -2 and 2 based on a sine wave."""
    elapsed_time = time.time() - start_time
    return 2 * math.sin(2 * math.pi * freq * elapsed_time)

def send_oscillating_data(serial_port):
    while True:
        val1 = get_oscillating_value(0.5)  # Adjust frequency as needed
        val2 = get_oscillating_value(0.7)  # Slightly different frequency for variation
        data = f"(N^{val1:.2f},0,0,70)"  # Formatting the float to 2 decimal places
        serial_port.write(data.encode())
        print(f"Sent: {data}")
        time.sleep(0.03)  # Send data every 30ms hz = 1/0.03 = 33.33

if __name__ == "__main__":
    port = "COM19"  # For Windows
    baudrate = 1000000  # Adjust as needed

    with serial.Serial(port, baudrate, timeout=1) as ser:
        send_oscillating_data(ser)

