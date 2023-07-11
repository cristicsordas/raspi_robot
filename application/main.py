import argparse
from motor.motor_client import MotorClient
from light.light_client import LightClient

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--app", help="name of the app to start(motor or light)", required=True)
    parser.add_argument("--ip_address", help="ip address", required=True)
    parser.add_argument("--port", help="port", required=True)
    parser.add_argument("--file_name", help="file name for the motor app", required=False)

    args = parser.parse_args()

    ip_address = args.ip_address
    port = (int)(args.port)
    file_name = args.file_name
    app = args.app

    if app == "motor":
        motor_client = MotorClient(ip_address, port, file_name)
        motor_client.run()
    elif app == "light":
        light_client = LightClient(ip_address, port)
        light_client.run()
        

