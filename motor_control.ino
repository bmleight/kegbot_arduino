#include <Servo.h>
#include "WString.h"

Servo myservo1; // create servo object to control a RoboClaw channel
Servo myservo2; // create servo object to control a RoboClaw channel

char command = 'S';
char commands[256];

int speed_left = 0, speed_right = 0;

int pos = 0; // variable to store the servo position
const int num_ir = 4;
int irPin[num_ir];
float readings[num_ir];

float distance = 0;

int have_command = 0;

uint8_t i = 0;

void setup()
{
      pinMode(13, OUTPUT);
      
    Serial.begin(9600);
    
//    
//    while (!Serial) {
//      ; // wait for serial port to connect. Needed for Leonardo only
//    }
//    
    myservo1.attach(5);
    myservo2.attach(6);
    
    for(int i=0; i<num_ir; i++) {
      irPin[i] = i;
      readings[i] = 0;
    }
    
    //establishContact();  // send a byte to establish contact until receiver responds 
    
}

void loop()
{
    
    readCommand2();
    
    readIr();
    sendReadings();
    
    //wander();
    
    moveRobot2();
    
    //Serial.write('t');
    
}

void wander() {
  
  float minDistance = 1000;
  
  for(int i=0; i<num_ir; i++) {
    if(readings[i] < minDistance) {
      minDistance = readings[i];
    }
  }
  
  if(minDistance >= 75) {
    //forward
    myservo1.writeMicroseconds(1425);
    myservo2.writeMicroseconds(1425);
    //Serial.println("Forward!");
  } else {
    //turn right
    myservo1.writeMicroseconds(1575);
    myservo2.writeMicroseconds(1575);
    //Serial.println("Right!");
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
  
  //Serial.print("{readings:{0:");
  //Serial.print(readings[0]);
  //Serial.print(",1:");
  //Serial.print("\t\t\t");
  //Serial.println(readings[1]);
  //Serial.println("}}");
}

void readCommand2() {
  
  char character;
  
  while (Serial.available()) {
    
    // read the incoming byte:
    character = Serial.read();
    
    if(character > 0) {
      
      commands[i] = character;
      
      i++;
      
      //command = Serial.read();
      
      //Serial.print('A');
      //delay(300);
      
    }
    
    if(character == '\n') {
      have_command = 1;
    }
  }
  
  if(have_command) {
    
    //Serial.println(commands);
    int result = sscanf(commands,"{left:%d,right:%d}", &speed_left, &speed_right);
    //Serial.println(result);
    
    
//    Serial.print("left ");
//    Serial.print(speed_left);
//    Serial.print("right ");
//    Serial.println(speed_right);
    
    have_command = 0;
    
    i = 0;
    
    //{left:34,right:93}
    
  }
  
}

void moveRobot2() {
    
    int left = 1500 - (int)(speed_left * 1.5);
    int right = 1500 - (int)(speed_right * 1.5);
    
    //Serial.println(left);
    //Serial.println(right);
    
    myservo1.writeMicroseconds(left);
    myservo2.writeMicroseconds(right);
    
}

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
    
}
