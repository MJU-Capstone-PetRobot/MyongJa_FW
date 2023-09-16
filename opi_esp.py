# Test serial comm Opi-ESP32
import serial
import threading
import time

port = '/dev/ttyUSB0' 
baud = 115200 

SerialObj = serial.Serial(port, baud, timeout=3)
alive_thread = True

esp_packet = ""
opi_packet = ""

touch = ""
co_ppm = ""
distance = ""
bat_state = ""

def read_thread(SerialObj):
    global esp_packet
    msg = ''

    while alive_thread:
        for c in SerialObj.read():
            esp_packet += (chr(c))
            if esp_packet.endswith('\n'):
                esp_packet = esp_packet.strip()

                if esp_packet[0] == '<' and esp_packet[len(esp_packet) -1] == '>':
                    msg = ""
                    msg = "[ESP->OPI] "
                    msg += esp_packet + " "

                    if esp_packet[1] == 'T':
                        touch = esp_packet[2]
                        msg += "[TOUCH] " + touch
                    elif esp_packet[1] == 'C':
                        co_ppm = esp_packet[2:-1]
                        msg += "[CO_PPM] " + co_ppm + "ppm"
                    elif esp_packet[1] == 'D':
                        distance = esp_packet[2:-1]
                        msg += "[DISTANCE] " + distance + "mm"
                    elif esp_packet[1] == 'B':
                        bat_state = esp_packet[2:-1]
                        msg += "[BAT] " + bat_state
                    else:
                        msg += "[ERROR]"

                    print(msg)
                else:
                    print(esp_packet)
                esp_packet = ""

    SerialObj.close()


def main():
    thread = threading.Thread(target=read_thread, args=(SerialObj,))
    thread.start()

    while True:
        opi_packet = "(E3)"
        SerialObj.write(opi_packet.encode())
        time.sleep(1)

        opi_packet = "(N1,1,80,1)"
        SerialObj.write(opi_packet.encode())
        time.sleep(1)

        # 목 제어, 표정

    alive_thread = False

if __name__ == '__main__':
    main()