import argparse
from motor.motor_client import MotorClient

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--ip_address", help="ip address", required=True)
    parser.add_argument("--port", help="port", required=True)
    parser.add_argument("--file_name")

    args = parser.parse_args()

    ip_address = args.ip_address
    port = (int)(args.port)
    file_name = args.file_name

    motor_client = MotorClient(ip_address, port, file_name)
    motor_client.run()

