import serial
import time
import random
import math
import threading
import sys

port = "COM13"
baud = 1000000

SerialObj = serial.Serial(port, baud, timeout=3)
esp_thread = True
tx_thread_flag = True
esp_packet = ""

start_time = time.time()


def get_oscillating_value(freq=1):
    """Return a value oscillating between -2 and 2 based on a sine wave."""
    elapsed_time = time.time() - start_time
    return 3 * math.sin(2 * math.pi * freq * elapsed_time)


def send_oscillating_data(serial_port):
    global tx_thread_flag
    try:
        while tx_thread_flag:
            val1 = get_oscillating_value(0.5)  # Adjust frequency as needed
            val2 = get_oscillating_value(0.7)  # Slightly different frequency for variation
            data = f"(N^{val1:.2f},0,0,70)"  # Formatting the float to 2 decimal places
            serial_port.write(data.encode())
            print(f"Sent: {data}")
            time.sleep(0.05)  # Send data every 30ms hz = 1/0.03 = 33.33
    except KeyboardInterrupt:       
        tx_thread_flag = False

def receive_from_esp(SerialObj):
    global esp_packet

    while esp_thread:
        for c in SerialObj.read():
            esp_packet += (chr(c))
            if esp_packet.endswith('\n'):
                print(esp_packet)
                esp_packet = ""

    SerialObj.close()


if __name__ == "__main__":

    tx_thread = threading.Thread(target=send_oscillating_data, args=(SerialObj,))
    rx_thread = threading.Thread(target=receive_from_esp, args=(SerialObj,))

    tx_thread.start()
    rx_thread.start()

    tx_thread.join()
    rx_thread.join()


    print("프로그램 종료")
