#include <Servo.h>
#include "WString.h"

Servo myservo1; // create servo object to control a RoboClaw channel
Servo myservo2; // create servo object to control a RoboClaw channel

/* KEGBOT MODES */
const int USER_DRIVE = 1;
const int ROBOT_DRIVE = 2;

char command = 'S';
char commands[256];
int command_index = 0;

int speed_left = 0, speed_right = 0;

int pos = 0; // variable to store the servo position
const int num_ir = 4;
int irPin[num_ir];
float readings[num_ir];

float distance = 0;

int have_command = 0;

int count = 0;

int mode = USER_DRIVE;

void setup()
{
      
    Serial.begin(9600);
    
    myservo1.attach(5);
    myservo2.attach(6);
    
    for(int i=0; i<num_ir; i++) {
      irPin[i] = i;
      readings[i] = 0;
    }
    
}

void loop()
{
    
    readCommand2();
    
    readIr();
    
    if(count < 1) {
        sendReadings();
    }
    
    if(mode == USER_DRIVE) {
        moveRobot2();
    } else {
        wander();
    }
    
    count++;
    count%=75;
    
}

void wander() {
  
  float minDistance = 1000;
  
  for(int i=0; i<num_ir; i++) {
    if(readings[i] < minDistance) {
      minDistance = readings[i];
    }
  }
  
  if(minDistance >= 30) {
    //forward
    myservo1.writeMicroseconds(1450);
    myservo2.writeMicroseconds(1450);
  } else {
    //turn right
    myservo1.writeMicroseconds(1450);
    myservo2.writeMicroseconds(1550);
  }
  
}

void readIr(){
  
  float volts = 0;
  
  for(int i=0; i<num_ir; i++) {
    volts = analogRead(irPin[i])*0.0048828125;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
    readings[i] = 65*pow(volts, -1.10);
  }
  
}

void sendReadings() {
  
  Serial.print("{\"readings\":[");
  
  for(int i=0; i<num_ir; i++) {
    Serial.print(readings[i]);
    if(i < num_ir-1) {
      Serial.print(",");
    }
  }
  
  Serial.println("]}");
  
}

void readCommand2() {
  
  int result;
  char character;
  
  while (Serial.available()) {
    
    // read the incoming byte:
    character = Serial.read();
    
    if(character > 0) {
      commands[command_index] = character;
      command_index++;
    }
    
    if(character == '\n') {
      have_command = 1;
    }
  }
  
  if(have_command) {
    
    //if the command is to change a mode
    if(strstr(commands, "mode") != NULL) {
      result = sscanf(commands,"{mode:%d}", &mode);
    } else {
      result = sscanf(commands,"{left:%d,right:%d}", &speed_left, &speed_right);
    }
    
    have_command = 0;
    command_index = 0;
    
  }
  
}

void moveRobot2() {
    
    int left = 1500 - (int)(speed_left * 1.25);
    int right = 1500 - (int)(speed_right * 1.25);
    
    //Serial.println(left);
    //Serial.println(right);
    
    myservo1.writeMicroseconds(left);
    myservo2.writeMicroseconds(right);
    
}
/*
void readCommand1() {
  while (Serial.available()) {
    
    command = Serial.read();
    
    //Serial.print('A');   // send a capital A
    //delay(300);
  }
}

void moveRobot1() {
    
    int left = 1500;
    int right = 1500;
  
    switch(command) {
        case 'w':
          left = 1450;
          right = 1450;
          break;
        case 'a':
          left = 1550;
          right = 1450;
          break;
        case 'd':
          left = 1450;
          right = 1550;
          break;
        case 's':
          left = 1550;
          right = 1550;
          break;
    }
    
    myservo1.writeMicroseconds(left);
    myservo2.writeMicroseconds(right);
    
}*/
