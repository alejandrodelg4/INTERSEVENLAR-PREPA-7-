const int motor1EnablePin = 12;  
const int motor1Input1 = 11;      
const int motor1Input2 = 10;      
int motor1Speed = 100; 

const int motor2EnablePin = 9;    
const int motor2Input1 = 8;     
const int motor2Input2 = 7;     
int motor2Speed = 100; 

const int motor3EnablePin = 6;  
const int motor3Input1 = 5;      
const int motor3Input2 = 4; 
int motor3Speed = 100; 

const int motor4EnablePin = 3;  
const int motor4Input1 = 2;      
const int motor4Input2 = 1; 
int motor4Speed = 100; 

void setup() {
  
  pinMode(motor1EnablePin, OUTPUT);
  pinMode(motor1Input1, OUTPUT);
  pinMode(motor1Input2, OUTPUT);

  pinMode(motor2EnablePin, OUTPUT);
  pinMode(motor2Input1, OUTPUT);
  pinMode(motor2Input2, OUTPUT);


  pinMode(motor3EnablePin, OUTPUT);
  pinMode(motor3Input1, OUTPUT);
  pinMode(motor3Input2, OUTPUT);


  pinMode(motor4EnablePin, OUTPUT);
  pinMode(motor4Input1, OUTPUT);
  pinMode(motor4Input2, OUTPUT);

  
  digitalWrite(motor1EnablePin, LOW);
  digitalWrite(motor1Input1, LOW);
  digitalWrite(motor1Input2, LOW);

  digitalWrite(motor2EnablePin, LOW);
  digitalWrite(motor2Input1, LOW);
  digitalWrite(motor2Input2, LOW);


  digitalWrite(motor3EnablePin, LOW);
  digitalWrite(motor3Input1, LOW);
  digitalWrite(motor3Input2, LOW);


  digitalWrite(motor4EnablePin, LOW);
  digitalWrite(motor4Input1, LOW);
  digitalWrite(motor4Input2, LOW);
}

void loop() {

  analogWrite(motor1EnablePin, motor1Speed);
  digitalWrite(motor1EnablePin, HIGH);
  digitalWrite(motor1Input1, HIGH);
  digitalWrite(motor1Input2, LOW);

  analogWrite(motor2EnablePin, motor2Speed);
  digitalWrite(motor2EnablePin, HIGH);
  digitalWrite(motor2Input1, LOW);
  digitalWrite(motor2Input2, HIGH);

  analogWrite(motor3EnablePin, motor3Speed);
  digitalWrite(motor3EnablePin, HIGH);
  digitalWrite(motor3Input1, HIGH);
  digitalWrite(motor3Input2, LOW);

  analogWrite(motor4EnablePin, motor4Speed);
  digitalWrite(motor4EnablePin, HIGH);
  digitalWrite(motor4Input1, LOW);
  digitalWrite(motor4Input2, HIGH);


  delay(2000); 

  digitalWrite(motor1EnablePin, LOW);
  digitalWrite(motor2EnablePin, LOW);
  digitalWrite(motor3EnablePin, LOW);
  digitalWrite(motor4EnablePin, LOW);

  delay(3000); 
}
