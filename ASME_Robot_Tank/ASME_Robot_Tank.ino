

uint8_t M1_INPUT = A0;
uint8_t M2_INPUT = A1;

uint8_t M1_OUTPUT = 10;
uint8_t M2_OUTPUT = 11;
/* Usefull Reference
 *  
 *  https://arduino-info.wikispaces.com/Arduino-PWM-Frequency
 */
void setup() {
  // put your setup code here, to run once:
  pinMode(M1_INPUT,INPUT);
  pinMode(M2_INPUT,INPUT);

  pinMode(M1_OUTPUT,OUTPUT);
  pinMode(M2_OUTPUT,OUTPUT);
  // set timer 2 divisor to 1 for PWM frequency of 31372.55 Hz (D3, D11)
  TCCR2B = TCCR2B & B11111000 | B00000001;   
  
  // set timer 1 divisor to 1 for PWM frequency of 31372.55 Hz (D9, D10)
  TCCR1B = TCCR1B & B11111000 | B00000001;

  // set timer 0 divisor to     1 for PWM frequency of 62500.00 Hz (D5,D6)
  //TCCR0B = TCCR0B & B11111000 | B00000001;    
  
  Serial.begin(9600);
setMotor(M1_OUTPUT,128);
setMotor(M2_OUTPUT,128);

  
}

void loop() {
  uint16_t M1_Value = pulseIn(M1_INPUT,HIGH);
  uint16_t M2_Value = pulseIn(M2_INPUT,HIGH);

  Serial.print(M1_Value);
  Serial.print("    "); 
  Serial.println(M2_Value);

  int16_t M1_Drive = map(M1_Value,1950,1100,32,224);
  int16_t M2_Drive = map(M2_Value,1050,1900,32,224);

  
setMotor(M1_OUTPUT,M1_Drive);
setMotor(M2_OUTPUT,M2_Drive);
delay(20);

 /*
 delay(2000);
 analogWrite(M1,0);
 delay(2000);
 analogWrite(M1,128);
 delay(2000);
*/
}
void setMotor(uint8_t motor, int16_t value){
  if(value < -100){
   Serial.println("Controller Not Connected");
   value = 128;
  }
  else if(value > 255){
    value = 255;
  }
  else if(value<0){
    value = 0;
  }else if(abs(value-128)<20){
    value = 128;
  }
  else{
    
  }
 // Serial.print(motor);
 // Serial.print(": ");
 // Serial.println(value);
  analogWrite(motor, value);
}


