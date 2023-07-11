import socket
import os
import time


class LightClient:
    def __init__(self, ip_address, port):
        self.__ip_address = ip_address
        self.__port = port
        self.__command_adc_channel_0 = "2 0\n"
        self.__command_adc_channel_1 = "2 1\n"
        self.__sleep_interval_ms = 100

    def run(self):
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((self.__ip_address, self.__port))
                while True:
                    s.sendall(self.__command_adc_channel_0.encode())
                    adc_left = s.recv(1024)
                    print(adc_left)
                    s.sendall(self.__command_adc_channel_1.encode())
                    adc_right = s.recv(1024)
                    print(adc_right)
                    time.sleep(self.__sleep_interval_ms / 1000)
        except KeyboardInterrupt:
            return
        except Exception as e:
            print(f"Error {e}")
