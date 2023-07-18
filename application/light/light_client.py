import socket
import os
import time
import binascii

dict_commands = {
    "right": "1 6 0 3000 0 3000 0 0 0 0",
    "left": "1 6 0 0 0 0 0 3000 0 3000",
    "move": "1 6 0 3000 0 3000 0 3000 0 3000",
    "stop": "1 0"
}


class AdcValue:
    def __init__(self, message, side):
        self.__message_data = bytearray(message)
        self.__side = side

    def log(self):
        if self.__message_data[0] == 3:
            print(f"{self.__side} {binascii.hexlify(self.__message_data)}")
            print(self.value())

    def value(self):
        return self.__message_data[1] * 10 + self.__message_data[2]


class CarController:
    def __init__(self, socket):
        self.__current_action = "stop"
        self.__min_value = 10
        self.__offset_tolerance = 5
        self.__socket = socket

    def execute_current_action(self):
        if self.__current_action in dict_commands:
            message = dict_commands[self.__current_action] + "\n"
            self.__socket.sendall(message.encode())

    def set_action(self, adc_left, adc_right):
        action = "stop"
        if adc_left.value() < self.__min_value and adc_right.value() < self.__min_value:
            action = "stop"
        elif abs(adc_right.value() - adc_left.value()) < self.__offset_tolerance:
            action = "move"
        elif adc_left.value() + self.__offset_tolerance > adc_right.value():
            action = "left"
        elif adc_right.value() + self.__offset_tolerance > adc_left.value():
            action = "right"

        if action != self.__current_action:
            self.__current_action = action
            self.execute_current_action()


class LightClient:
    def __init__(self, ip_address, port):
        self.__ip_address = ip_address
        self.__port = port
        self.__command_adc_channel_0 = "2 0\n"
        self.__command_adc_channel_1 = "2 1\n"
        self.__sleep_interval_ms = 200

    def run(self):
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((self.__ip_address, self.__port))
                car_controller = CarController(s)
                while True:
                    s.sendall(self.__command_adc_channel_0.encode())
                    msg_left = s.recv(1024)
                    adc_left = AdcValue(msg_left, "left")
                    adc_left.log()
                    time.sleep(self.__sleep_interval_ms / 1000)
                    s.sendall(self.__command_adc_channel_1.encode())
                    msg_right = s.recv(1024)
                    adc_right = AdcValue(msg_right, "right")
                    adc_right.log()
                    time.sleep(self.__sleep_interval_ms / 1000)
                    car_controller.set_action(adc_left, adc_right)
        except KeyboardInterrupt:
            return
        except Exception as e:
            print(f"Error {e}")
