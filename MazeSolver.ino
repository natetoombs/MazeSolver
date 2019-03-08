// MazeSolver w/Edits by Nathan

  //Initialize Pins
    // H-Bridge Pins (Double-Check these; some have switched)
int enA = 3;  //EnableA- Will use a PWM to control the Right motor.
int in1 = 5;  //in1 and in2 also control the right motor.
int in2 = 7; 

int in4 = 2;  //These are for the left motor.
int in3 = 4;      
int enB = 6;  

    // RangeFinder Pins
int echoF = 8; //Echo(Front): Receives from the Ultrasonic
int triggerF = 9; //Trigger(Front): Produces a signal for the Ultrasonic
int echoR = 10; //Right
int triggerR = 11;
int echoL = 12; //Left
int triggerL = 13;

// Constants
#define LEFT 3
#define RIGHT 2
#define STRAIGHT 1
#define OBSTACLE_CLOSE 10  // Determines how close to a wall the robot must get before turning. May be adjusted
#define TURN_TIME 750     // How long to turn 90 degrees. Will need to be calibrated for each team
#define SHARP_TURN_TIME 600 //See my SharpRight code at bottom. We can write functions that might be
                            //able to make the robot run faster.

// Initialize variables
float straightDistance = 0, rightDistance = 0, leftDistance = 0;

void setup() {
      // Sets the pins as input pins or output pins, according to role
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(triggerF, OUTPUT);
  pinMode(echoF, INPUT);
  pinMode(triggerR, OUTPUT);
  pinMode(echoR, INPUT);
  pinMode(triggerL, OUTPUT);
  pinMode(echoL, INPUT);
  Serial.begin(9600);        // Sets the baud rate
}

// Will work, but doesn't actually guarantee a solved maze as it is right now
void loop() { //THIS IS WHERE THE MAIN MAZE-SOLVING CODE GOES
  //For example, this code will drive straight until it approaches a wall, then
  //it will go either left or right, whichever has a longer path to follow.
  GoStraight();
  straightDistance = GetDistance(STRAIGHT);
  rightDistance = GetDistance(RIGHT);
  leftDistance = GetDistance(LEFT);

  if (straightDistance < OBSTACLE_CLOSE) {
    if (leftDistance < OBSTACLE_CLOSE && rightDistance < OBSTACLE_CLOSE) {
      LeftTurn90();
      LeftTurn90();
    }
    else if (leftDistance > rightDistance) {
      LeftTurn90();
    }
    else if (rightDistance > leftDistance) {
      RightTurn90();
    }
  }
}

void GoStraight() {
    // Sets all wheels to forward     -    these may need to be changed
  digitalWrite(in1, LOW);         //The direction of each motor is determined
  digitalWrite(in2, HIGH);        //by its two "in" pins on the H-bridge. One 
  digitalWrite(in3, HIGH);        //must be high and the other low, and switching
  digitalWrite(in4, LOW);         //them changes the direction.

    // PWM to slow down the maze solver 
  for (int i = 0; i < 10; i++) {  //PWM-Pulse Width Modulation
    analogWrite(enB, 225);        //analogWrite(pin,value) uses a PWM to simulate
    analogWrite(enA, 205);        //an analog signal, using a range from 0 to 255
    delay(5);                     //(8bit) to make an output voltage between 0
    analogWrite(enB, 0);          //and 5 volts. **I'm not sure why B is 225 and
    analogWrite(enA, 0);          //A is 205; I'm guessing they should be equal?
    delay(14);                    //delay(time) is in milliseconds
  }

  return;
}

void LeftTurn90() {
  
    // Sets right wheel to forward
  digitalWrite(in1, LOW);     //Make sure that these HIGH and LOW values
  digitalWrite(in2, HIGH);    //will drive forward.
  analogWrite(enA, 205);
    // Left wheel is turned off
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 0);
    // Continues for enough time to turn 90 degrees. TURN_TIME (top of file) will need to be adjusted 
  delay(TURN_TIME);   

  return;
}

void RightTurn90() {
    // Right wheel is turned off
  digitalWrite(in1, HIGH);  //These lines might be a little redundant, it doesn't
  digitalWrite(in2, LOW);   //matter what direction in1&2 are if enA is at 0.
  analogWrite(enA, 0);
    // Set left wheel to forward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 205);
    // Continues for enough time to turn 90 degrees. TURN_TIME (top of file) will need to be adjusted
  delay(TURN_TIME);
  return;
}

double GetDistance(int directions) {
  double duration, distance;

  if (directions == STRAIGHT) {
      // Sequence to activate the rangeFinders and read the data
    digitalWrite(triggerF, LOW);

    delayMicroseconds(2);
    digitalWrite(triggerF, HIGH);

    delayMicroseconds(10);              //pulseIn works by counting how long it takes for the sound
    duration = pulseIn(echoF, HIGH);    //to bounce back to the sensor (duration). The distance is then
    distance = (duration / 2) * 0.0343; //that duration/2 (because the sound goes there and back), and 
                                        //the 0.0343 converts the distance to cm.
    //Serial.println("distance1 =  ");
    //Serial.println(distance);         //Here is some code for debugging.
  } else if (directions == RIGHT) {
      // Sequence to activate the rangeFinders and read the data
    digitalWrite(triggerR, LOW);

    delayMicroseconds(2);
    digitalWrite(triggerR, HIGH);

    delayMicroseconds(10);
    duration = pulseIn(echoR, HIGH);
    distance = (duration / 2) * 0.0343;

    //Serial.println("distance2 =  ");
    //Serial.println(distance);
  } else if (directions == LEFT) {
      // Sequence to activate the rangeFinders and read the data
    digitalWrite(triggerL, LOW);

    delayMicroseconds(3);
    digitalWrite(triggerL, HIGH);

    delayMicroseconds(10);
    duration = pulseIn(echoL, HIGH);
    distance = (duration / 2) * 0.0343;

    // Serial.println("distance3 =  ");
    //Serial.println(distance);
  }
  return distance;

}

void SharpRight() {
    // Right wheel goes forward
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 205);
    //Left wheel goes backward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 205);
    // Continues for enough time to turn 90 degrees. SHARP_TURN_TIME (top of file) will need to be adjusted
  delay(SHARP_TURN_TIME);
  return;
}
