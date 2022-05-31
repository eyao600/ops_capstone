// left motor
#define EN1 6
#define IN1 2
#define IN2 3
#define MIN_SPEED_L 100
// right motor
#define EN2 9
#define IN3 4
#define IN4 5
#define MIN_SPEED_R 80
// ultrasonic sensor
#define TRIG 11
#define ECHO 10
// IR receiver
#define IR A3

double Kp = 0.5;
double Ki = 0;
double Kd = 2;
double curr_error, int_error, der_error;
double target_reading = 700;
double duration;
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
  
  go_forward();
  digitalWrite(TRIG, LOW);

  ir_zero = 0;
  for(int i = 0; i < 100; i++) {
    ir_zero += analogRead(IR);
  }
  ir_zero /= 100;

  curr_error = read_ultra() - target_reading;
  
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

int analog_speed = 255;
void accelerate()
{
  analogWrite(EN1, analog_speed);
  analogWrite(EN2, analog_speed);
}

int current_speed_L, current_speed_R = 255;
void loop() {
  
  ir_reading = constrain(analogRead(IR), 0, ir_zero);
  
  //read ultrasonic sensor
  send_pulse();
  duration = read_ultra();

  int spd = PID(duration);

  //unsure of how to use PID number since there are two motors
  if(spd > 0)  //turn left
  {
  current_speed_L = constrain(spd, MIN_SPEED_L + 20, 255);
  current_speed_R = current_speed_L - 35;
  }
  else if (spd < 0)  //turn right
  {
  current_speed_R = constrain(-spd, MIN_SPEED_R + 20, 255);
  current_speed_L = current_speed_R + 19;
  if (current_speed_L > 255) current_speed_L = 255;
  }
  
  analogWrite(EN1, current_speed_L);
  analogWrite(EN2, current_speed_R);

   //if (ir_reading < ir_zero - 50) stop();  //temporary

}

double read_ultra(){   //reads ultrasound duration as an average of 10 pulses
  double dur = 0;

  // average over 10 reads
  for (int i = 0; i < 10; i++){
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    dur += pulseIn(ECHO, HIGH);
  }
  dur /= 10;

  return dur;
}

double PID(double dur)  //calculates PID
{
  int time_elapsed = millis();
  int prev_error = curr_error;
  curr_error = target_reading - dur;
  int_error += time_elapsed * curr_error;
  der_error = (curr_error - prev_error) / time_elapsed;

  int out = Kp * curr_error + Ki * int_error + Kd * der_error;
  return out;
}
