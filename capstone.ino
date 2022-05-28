// left motor
#define EN1 6
#define IN1 2
#define IN2 3
// right motor
#define EN2 9
#define IN3 4
#define IN4 5
// ultrasonic sensor
#define TRIG 11
#define ECHO 10
// IR receiver
#define IR A3

unsigned long duration;
long ir_zero, ir_reading;

void setup() {
  // put your setup code here, to run once:
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(IR, INPUT);
  
  turn_right();
  digitalWrite(TRIG, LOW);

  ir_zero = 0;
  for(int i = 0; i < 100; i++) {
    ir_zero += analogRead(IR);
  }
  ir_zero /= 100;
  
  Serial.begin(9600);
}

void stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
}

void go_forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
}

void go_backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, HIGH);
}

void turn_left() {
  // left motor backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  //right motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turn_right() {
  // left motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  //right motor backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void send_pulse() {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(15);
  digitalWrite(TRIG, LOW);
}

void loop() {
  //accelerate
  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, HIGH);

  ir_reading = constrain(analogRead(IR), 0, ir_zero);
  
  // read ultrasonic sensor
  send_pulse();
  duration = pulseIn(ECHO, HIGH);
  
  Serial.println(ir_reading);
}
