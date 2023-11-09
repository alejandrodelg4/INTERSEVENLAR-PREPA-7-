#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>
#include <VL53L0X.h>

//#AGREGAR LO DEL BLUTU, QUE CUANDO RECIBA LA SEÑAL DEL RFID EL DRON EMPIECE A VOLAR



MPU6050 mpu;
VL53L0X sensor;

motor1; 
motor2;  
motor3;  
motor4;  

int targetRoll = 0;    // Ángulo de balanceo
int targetPitch = 0;   // Ángulo de inclinación 
int maxAltitude = 100; // Altura máxima que debe volar, le puse 10 cm por seguridad xd
int roll, pitch, altitude;

void setup() {
  Wire.begin();
  mpu.initialize();
  motor1.attach(9);  
  motor2.attach(10); 
  motor3.attach(11); 
  motor4.attach(12); 
  Serial.begin(9600);
  sensor.init();
  sensor.setTimeout(500);
  sensor.startContinuous();
}

void loop() {
  // DATOS DEL MPU 6050 
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // CalculaR el ángulo de balanceo (roll) y el ángulo de inclinación (pitch)
  roll = atan2(ay, az) * (180.0 / M_PI);
  pitch = atan2(-ax, sqrt(ay * ay + az * az)) * (180.0 / M_PI);

  // Aqui se calcula la altura del sensor VL53L0X
  altitude = sensor.readRangeContinuousMillimeters();

  // Calcular la diferencia entre el ángulo deseado y el ángulo real
  int rollError = targetRoll - roll;
  int pitchError = targetPitch - pitch;

  //control de estabilidad para ajustar los motores o servos
  int motor1Speed = 90 - rollError;  // Ajustar la velocidad del motor 1
  int motor2Speed = 90 - rollError;  // Ajustar la velocidad del motor 2
  int motor3Speed = 90 + pitchError; // Ajustar la velocidad del motor 3
  int motor4Speed = 90 + pitchError; // Ajustar la velocidad del motor 4

  // Limita los valores de velocidad entre 0 y 1100 en nuestro caso pero falta ajustar ya montado el dron
  motor1Speed = constrain(motor1Speed, 0, 1100);
  motor2Speed = constrain(motor2Speed, 0, 1100);
  motor3Speed = constrain(motor3Speed, 0, 1100);
  motor4Speed = constrain(motor4Speed, 0, 1100);

  // control de altura
  if (altitude > maxAltitude) {
    // Se tien que ajustar los valores para que baje
    // Reducir la velocidad de los motores o servos para descender despacio
    motor1Speed -= 10; // se ajusta el valor dependiendo del modelo del motor el de nosotros es un 2200KV
    motor2Speed -= 10;
    motor3Speed -= 10; 
    motor4Speed -= 10; 
  } else if (altitude < maxAltitude) {
    // Ajustar los motores para subir
    // aumentar la velocidad de los motores o servos para ascender despacio
    motor1Speed += 10; // se ajusta el valor dependiendo del modelo del motor el de nosotros es un 2200KV
    motor2Speed += 10; 
    motor3Speed += 10;
    motor4Speed += 10; 
  }

  // Limita los valores de velocidad entre 0 y 1100 o hasta 2000, no sé todavía no lo pruebo bn xd
  motor1Speed = constrain(motor1Speed, 0, 1100);
  motor2Speed = constrain(motor2Speed, 0, 1100);
  motor3Speed = constrain(motor3Speed, 0, 1100);
  motor4Speed = constrain(motor4Speed, 0, 1100);

  // Controla los motores 
  motor1.write(motor1Speed);
  motor2.write(motor2Speed);
  motor3.write(motor3Speed);
  motor4.write(motor4Speed);

  // Imprimirlos valores de roll, pitch, altura y las velocidades de los motores
  Serial.print("Roll: ");
  Serial.print(roll);
  Serial.print("   Pitch: ");
  Serial.print(pitch);
  Serial.print("   Altura: ");
  Serial.print(altitude);
  Serial.print("   Motor1: ");
  Serial.print(motor1Speed);
  Serial.print("   Motor2: ");
  Serial.print(motor2Speed);
  Serial.print("   Motor3: ");
  Serial.print(motor3Speed);
  Serial.print("   Motor4: ");
  Serial.println(motor4Speed);
