/* Useful Reference
 *  https://arduino-info.wikispaces.com/Arduino-PWM-Frequency
 */
#define NUM_INPUTS    4
#define NUM_MOTORS    4
 
#define LEFT_JOY_Y    A0
#define RIGHT_JOY_X   A1
#define RIGHT_JOY_Y   A2
#define LEFT_JOY_X    A3

#define RIGHT_DRIVE   10
#define LEFT_DRIVE    11
#define M3_OUTPUT     3
#define M4_OUTPUT     9

#define DISABLE_PIN   2

bool disabled = true;
uint8_t motors[NUM_MOTORS] = {RIGHT_DRIVE,LEFT_DRIVE,M3_OUTPUT,M4_OUTPUT};
void setup(){

  pinMode(LEFT_DRIVE,OUTPUT);
  pinMode(RIGHT_DRIVE,OUTPUT);
  
  pinMode(LEFT_JOY_X,INPUT);
  pinMode(LEFT_JOY_Y,INPUT);
  pinMode(RIGHT_JOY_X,INPUT);
  pinMode(RIGHT_JOY_Y,INPUT);

  pinMode(DISABLE_PIN,OUTPUT);
  
  // set timer 2 divisor to 1 for PWM frequency of 31372.55 Hz (D3, D11)
  TCCR2B = TCCR2B & B11111000 | B00000001;   
  
  // set timer 1 divisor to 1 for PWM frequency of 31372.55 Hz (D9, D10)
  TCCR1B = TCCR1B & B11111000 | B00000001;

  // set timer 0 divisor to     1 for PWM frequency of 62500.00 Hz (D5,D6)
  //TCCR0B = TCCR0B & B11111000 | B00000001;    
  
  Serial.begin(9600);

  setMotor(RIGHT_DRIVE,128);
  setMotor(LEFT_DRIVE,128);

  
}

void loop() {
  uint16_t LEFT_JOY_Y_VALUE = pulseIn(LEFT_JOY_Y,HIGH,50000);
  uint16_t RIGHT_JOY_X_VALUE = pulseIn(RIGHT_JOY_X,HIGH,50000);
  uint16_t RIGHT_JOY_Y_VALUE = pulseIn(RIGHT_JOY_Y,HIGH,50000);
  uint16_t LEFT_JOY_X_VALUE = pulseIn(LEFT_JOY_X,HIGH,50000);

  uint16_t inputs[4] = {LEFT_JOY_Y_VALUE,RIGHT_JOY_X_VALUE,RIGHT_JOY_Y_VALUE,LEFT_JOY_X_VALUE};
  checkConnection(inputs);

  int16_t power = map(LEFT_JOY_Y_VALUE,1950,1100,224,32);
  int16_t turn = map(RIGHT_JOY_X_VALUE,1050,2000,96,-96);
  
  setMotor(RIGHT_DRIVE, map(power - turn,0,255,255,0));
  setMotor(LEFT_DRIVE, power + turn);

}
void setMotor(uint8_t motor, int16_t value){
  if(value > 255){
    value = 255;
  }
  else if(value<0){
    value = 0;
  }else if(abs(value-128)<20){
    value = 128;
  }
  if(disabled){
    value = 128;
  }
  analogWrite(motor, value);
}

void checkConnection(uint16_t data[]){
  uint8_t DC_COUNT = 0;
  for(int i = 0; i< NUM_INPUTS; i++){
    Serial.print(data[i]);
    Serial.print("   ");
    if(data[i] < 100){
     DC_COUNT++;
    }
    
  }
  Serial.println("");
  if(DC_COUNT >= NUM_INPUTS){
    Serial.println("Controller Not Connected");
    disableRobot();
  }
  else{
    enableRobot();
  }
}

void disableRobot(){
  digitalWrite(DISABLE_PIN,LOW);
  disabled = true;
  for(int i = 0; i< NUM_MOTORS; i++){
    analogWrite(i,128);
  }
}
void enableRobot(){
  disabled = false;
  digitalWrite(DISABLE_PIN,HIGH);
}

