# Test serial comm Opi-ESP32
import serial
import threading
import time

line = '' 
port = '/dev/ttyUSB0' 
baud = 115200 

SerialObj = serial.Serial(port, baud, timeout=3)
alive_thread = True

def read_thread(SerialObj):
    global line

    while alive_thread:
        for c in SerialObj.read():
            line += (chr(c))
            if line.endswith('\n'):
                print(line)
                line = ''

    SerialObj.close()


def main():
    thread = threading.Thread(target=read_thread, args=(SerialObj,))
    thread.start()

    led = ['RED', 'BLUE', 'GREEN']

    while True:
        for color in led:
            strcmd = color
            print('[LED] ' + strcmd)
            strcmd += '\n'
            SerialObj.write(strcmd.encode())
            time.sleep(1)
        # 키보드 입력 받아서 명령
        # 목 제어, 표정

    alive_thread = False

if __name__ == '__main__':
    main()