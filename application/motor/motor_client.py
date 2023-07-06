import socket
import os
import time

MOTOR_RESOURCES = os.path.join(os.path.dirname(os.path.abspath(__file__)), '../resources')


class MotorClient:
    def __init__(self, ip_address, port, file):
        self.__lines = None
        self.__ip_address = ip_address
        self.__port = port
        self.__sleep_interval_ms = 1000
        self.read_file(file)

    def read_file(self, file):
        file_path = os.path.join(MOTOR_RESOURCES, file)
        with open(file_path) as f:
            self.__lines = f.readlines()

    def run(self):
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((self.__ip_address, self.__port))
                for line in self.__lines:
                    s.sendall(line.encode())
                    time.sleep(self.__sleep_interval_ms / 1000)
        except Exception as e:
            print(f"Error {e}")
