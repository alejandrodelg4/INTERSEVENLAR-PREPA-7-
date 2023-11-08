#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_PWMServoDriver.h>



Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int rightElbowPos = 90;   
int leftElbowPos = 90;  

void setup() {
  pwm.begin();
  pwm.setPWMFreq(60);  
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    if (command.length() > 0) {
      
      int rightElbowTarget, leftElbowTarget;
      sscanf(command.c_str(), "%d %d", &rightElbowTarget, &leftElbowTarget);

      
      rightElbowTarget = constrain(rightElbowTarget, 0, 180);
      leftElbowTarget = constrain(leftElbowTarget, 0, 180);

      // Mueve los servos de los codos a las posiciones objetivo
      moveServo(1, rightElbowPos, rightElbowTarget);
      moveServo(2, leftElbowPos, leftElbowTarget);

    
      rightElbowPos = rightElbowTarget;
      leftElbowPos = leftElbowTarget;
    }
  }
}

void moveServo(int channel, int currentPos, int targetPos) {
  int mappedValue = map(targetPos, 0, 180, 100, 600);  // Mapea a los valores esperados por la placa controladora
  pwm.setPWM(channel, 0, mappedValue);
}
