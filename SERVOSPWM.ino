#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_PWMServoDriver.h>



Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver(0x40);

unsigned int pos0=172; // ancho de pulso en cuentas para pocicion 0°
unsigned int pos180=565; // ancho de pulso en cuentas para la pocicion 180°

void setup() {
  servos.begin();  
  servos.setPWMFreq(60); //Frecuecia PWM de 60Hz o T=16,66ms
}

void setServo(uint8_t n_servo, int angulo) {
  int duty;
  duty=map(angulo,0,180,pos0, pos180);
  servos.setPWM(n_servo, 0, duty);  
}

void loop() {

    setServo(0,0);
    setServo(1,220);
    setServo(2,130);
    setServo(3,0);
    setServo(4,0);
    setServo(5,0);
    delay(1000);
  
    setServo(0,130);
    setServo(1,150);
    setServo(2,180);
    setServo(3,130);
    setServo(4,100);
    setServo(5,100);
    delay(1000);
  

}
