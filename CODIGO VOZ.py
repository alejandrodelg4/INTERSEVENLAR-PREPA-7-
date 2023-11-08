from vosk import Model, KaldiRecognizer
import pyaudio
import cv2
import time
import threading
import osascript
import serial
import numpy as np
import math
from tensorflow import keras
from cvzone.ClassificationModule import Classifier
classifier = Classifier("Model/keras_model.h5", "Model/labels.txt")
class Sistema:
    def __init__(self, model_path):
        self.model = Model(model_path)
        self.recognizer = KaldiRecognizer(self.model, 16000)
        self.mic = pyaudio.PyAudio()
        self.face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
        self.detecting_faces = False 
        self.face_thread = None  
        self.arduino = serial.Serial('/dev/cu.usbmodem11401', 9600)
        self.stop_event = threading.Event()
        self.stefany_found = False
        self.unknown_found = False
        self.face_detected = False


    def enviar_comando_arduino(self, comando):
        self.arduino.write(comando.encode())

    def get_command(self):
        stream = self.mic.open(format=pyaudio.paInt16, channels=1, rate=16000, input=True, frames_per_buffer=8192)
        listening = True
        while listening:
            stream.start_stream()
            try:
                data = stream.read(4096)
                if self.recognizer.AcceptWaveform(data):
                    result = self.recognizer.Result()
                    response = result.lower().strip()
                    stream.stop_stream()
                    return response
            except OSError:
                pass

    def voz_cara(self):
        while True:
            print("Escuchando")
            command = self.get_command()
            if not command:
                continue  # Vuelve a escuchar si no se obtiene un comando

            if "hola" in command:
                print("1")
                osascript.osascript('say "Hola humano"')
                self.enviar_comando_arduino("c")
            elif "caras" in command and not self.detecting_faces:
                osascript.osascript('say "Iniciando la detección de caras"')
                self.enviar_comando_arduino("c")
                self.detecting_faces = True
                self.face_thread = threading.Thread(target=self.start_face_detection)
                self.face_thread.start()
            elif "detener detección de caras" in command and self.detecting_faces:
                osascript.osascript('say "Deteniendo la detección de caras"')
                self.enviar_comando_arduino("c")
                self.detecting_faces = False
                self.stop_event.set()
            elif "baila conmigo" in command:
                osascript.osascript('say "Bailando"')
                self.enviar_comando_arduino("c")
                self.enviar_comando_arduino("b")
            elif "sigue" in command and not self.detecting_faces:
                osascript.osascript('say "Te sigo "')
                self.enviar_comando_arduino("c")
                self.enviar_comando_arduino("s")
                self.detecting_faces = True
                self.face_thread = threading.Thread(target=self.start_seguimiento)
                self.face_thread.start()
            elif "no sigas" in command and self.detecting_faces:
                osascript.osascript('say "No te sigo"')
                self.enviar_comando_arduino("c")
                self.detecting_faces = False
                self.stop_event.set()
            elif "hora de irnos" in command:
                osascript.osascript('say "Hasta luego"')
                self.enviar_comando_arduino("c")
                return
            elif "conceder deseo" in command:
                osascript.osascript('say "No gracias"')
                self.enviar_comando_arduino("c")
            elif "Eres Stefany" in command and not self.stefany_found:
                osascript.osascript('say "Eres Stefany"')
                self.stefany_found = True  # Establecer el estado a True para evitar repeticiones
                self.enviar_comando_arduino("c")
            elif "No se quien eres" in command and not self.unknown_found:
                osascript.osascript('say "No se quien eres"')
                self.unknown_found = True  # Establecer el estado a True para evitar repeticiones
                self.enviar_comando_arduino("c")
            else:
                osascript.osascript('say "No entiendo ese comando"')
                self.enviar_comando_arduino("c")

    def start_seguimiento(self):
        cap = cv2.VideoCapture(1)
        while self.detecting_faces:
            ret, img = cap.read()
            if not ret:
                break

            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

            # Detectar caras en la imagen
            faces = self.face_cascade.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5)

            if len(faces) > 0:
                x, y, w, h = faces[0]
                cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)
                center_x = int(x + w / 2)

                # Convertir la posición horizontal en un ángulo para el servo (0-180 grados)
                angle_x = int((center_x / 640) * 180)
                angle_x = min(max(angle_x, 0), 180)

                # Enviar el ángulo al Arduino a través de la conexión serial
                self.arduino.write(bytes(str(angle_x) + '\n', 'ascii'))


            if cv2.waitKey(1) & 0xFF == ord('q'):
               self.detecting_faces = False

        cap.release()
        cv2.destroyAllWindows()


    def start_face_detection(self):
        cap = cv2.VideoCapture(1)  # Abre la cámara (ajusta el número de la cámara según tu configuración)
        modelo = keras.models.load_model("Model/keras_model.h5")
        modelo.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])
        face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

        offset = 20
        imgSize = 300

        folder = "Data/C"
        counter = 0

        labels = ["CARA DE STEFANY", "DESCONOCIDO"]

        while self.detecting_faces:
            success, img = cap.read()
            imgOutput = img.copy()
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5)

            for (x, y, w, h) in faces:
                # Ajustar el tamaño de la región de interés (ROI) para la cara
                x = max(0, x - offset)
                y = max(0, y - offset)
                w = min(img.shape[1], x + w + offset) - x
                h = min(img.shape[0], y + h + offset) - y

                imgCrop = img[y:y + h, x:x + w]

                if imgCrop.shape[0] > 0 and imgCrop.shape[1] > 0:
                    imgWhite = np.ones((imgSize, imgSize, 3), np.uint8) * 255

                    aspectRatio = h / w

                    if aspectRatio > 1:
                        k = imgSize / h
                        wCal = math.ceil(k * w)
                        imgResize = cv2.resize(imgCrop, (wCal, imgSize))
                        imgResizeShape = imgResize.shape
                        wGap = math.ceil((imgSize - wCal) / 2)
                        imgWhite[:, wGap:wCal + wGap] = imgResize
                        prediction, index = classifier.getPrediction(imgWhite, draw=False)
                        if index < len(labels):
                            label = labels[index]
                        else:
                            label = "Unknown"

                        print(prediction, label)

                    else:
                        k = imgSize / w
                        hCal = math.ceil(k * h)
                        imgResize = cv2.resize(imgCrop, (imgSize, hCal))
                        imgResizeShape = imgResize.shape
                        hGap = math.ceil((imgSize - hCal) / 2)
                        imgWhite[hGap:hCal + hGap, :] = imgResize
                        prediction, index = classifier.getPrediction(imgWhite, draw=False)
                        if index < len(labels):
                            label = labels[index]
                        else:
                            label = "Unknown"

                        print(prediction, label)

                    cv2.rectangle(imgOutput, (x - offset, y - offset - 50),
                                  (x - offset + 90, y - offset - 50 + 50), (255, 0, 255), cv2.FILLED)
                    cv2.putText(imgOutput, label, (x, y - 26), cv2.FONT_HERSHEY_COMPLEX, 1.7, (255, 255, 255), 2)
                    cv2.rectangle(imgOutput, (x - offset, y - offset),
                                  (x + w + offset, y + h + offset), (255, 0, 255), 4)

                    # Dibuja un rectángulo verde con (0,255,0) y un rectángulo rosa con los 3 en (255)
                    cv2.rectangle(imgOutput, (x - offset, y - offset), (x + w + offset, y + h + offset), (0, 255, 0), 2)

                    if label == "CARA DE STEFANY" and not self.stefany_found:
                        self.last_detected_name = "Eres Stefany"
                        osascript.osascript('say "Eres Stefany"')
                        self.stefany_found = True  # Establecer el estado a True para evitar repeticiones
                        self.enviar_comando_arduino("b")

                    elif label == "DESCONOCIDO" and not self.unknown_found:
                        self.last_detected_name = "No se quién eres"
                        osascript.osascript('say "No se quien eres"')
                        self.unknown_found = True  # Establecer el estado a True para evitar repeticiones
                        self.enviar_comando_arduino("b")


        if cv2.waitKey(1) == ord('q'):
                self.detecting_faces = False

        if not self.face_detected:
            self.stefany_found = False
            self.unknown_found = False


        cap.release()
        cv2.destroyAllWindows()



    @classmethod
    def process_commands(cls):
        pass

if __name__ == "__main__":
    sistema = Sistema(r"/Users/stefanyrangel/PycharmProjects/voz/vosk-model-small-es-0.42")
    sistema.voz_cara()
