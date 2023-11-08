from main import Sistema

if __name__ == "__main__":
    sistema = Sistema(r"/Users/stefanyrangel/PycharmProjects/voz/vosk-model-small-es-0.42")
    sistema.voz_cara()

import serial
import subprocess
import time

arduino = serial.Serial('/dev/cu.usbmodem1301', 9600)

while True:
    try:
        signal = arduino.read().decode()  # Lee un carácter desde Arduino y lo decodifica a cadena
        if signal == 'w':
            print("Se recibió la señal 'w' desde Arduino. Ejecutando main.py...")
            # Ejecuta main.py como un proceso separado
            subprocess.run(["python", "main.py"])
        time.sleep(1)  # Espera un segundo antes de verificar nuevamente
    except Exception as e:
        print(f"Error de comunicación con Arduino: {str(e)}")
        time.sleep(1)  # Espera un segundo antes de reintentar

