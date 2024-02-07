#include <MeAurigaLab.h> // Contains sensor and port handler
#include "scheduler.h" // Contains FreeRTOS and ESFree

MeUltrasonicSensor ultraSensor(PORT_7); //Obstecle sensor
MeLineFollower lineFinder(PORT_9); //Linefollower


MeEncoderOnBoard encoder1(SLOT1); //Knyter motor encodern
MeEncoderOnBoard encoder2(SLOT2); // till rätt motor kontakt

TaskHandle_t xHandle1 = NULL; // varibles containing pointers
TaskHandle_t xHandle2 = NULL; // to tasks
TaskHandle_t xHandle3 = NULL; // to tasks

char c1 = 'a';
char c2 = 'b';
char c3 = 'c';

MeBuzzer buzzer;
int lineread;
const int high=120;
const int mid=120;
const int low=75;
const int off=0;

const int distance = 25; //distance from object
int count = 0;
int flag = 0;
int sensor;
/*----------------------------------------------------------------*/
// Hjälp funktioner
void forward () {
   encoder1.setMotorPwm(-high);
   encoder2.setMotorPwm(high);
   encoder1.updateSpeed(); // Verkställer hastighetsändringen
}

void right() {
  encoder1.setMotorPwm(0);
  encoder2.setMotorPwm(mid);
  encoder1.updateSpeed(); // Verkställer hastighetsändringen
}

void left() {
  encoder1.setMotorPwm(-mid);
  encoder2.setMotorPwm(0);
  encoder1.updateSpeed(); // Verkställer hastighetsändringen
}

void stop1() {
  encoder1.setMotorPwm(off);
  encoder2.setMotorPwm(off);
  encoder1.updateSpeed(); // Verkställer hastighetsändringen
}
void bothWhite(){
  encoder1.setMotorPwm(low);
  encoder2.setMotorPwm(-low);
  encoder1.updateSpeed(); // Verkställer hastighetsändringen
}
void rotate(){
  encoder1.setMotorPwm(-low);
  encoder2.setMotorPwm(low);
  encoder1.updateSpeed(); // Verkställer hastighetsändringen
}

/*----------------------------------------------------------------*/
// Follwo line function
void Followline ()
{
  lineread = lineFinder.readSensors ();
  if(sensor <= distance){
    right();
    delay(500);
    forward();
    delay(300);
    left();
    delay(300);
    forward();
    delay(300); //500 VAR DET N<ÄR DET FUNKADE
    left();
    delay(300);

    
  }
  else 
  {
    switch(lineread)
    {
      case 0:
        forward();
        break;
      case 1:
        left();
        break;
      case 2:
        right();
        break;
      default: break;
    }
  }
}

/*----------------------------------------------------------------*/
// Avoid obstacles
void TaskBlip (void *pvParameters) {
  sensor = ultraSensor.distanceCm ();
  if(sensor <= distance){
     buzzer.tone (400 , 200);
  }
}

void setup() {
  sensor = 100;
  Serial.begin (9600);
  
  TCCR1A = _BV (WGM10);
  TCCR1B = _BV (CS11) | _BV (WGM12);
  TCCR2A = _BV (WGM21) | _BV (WGM20);
  TCCR2B = _BV(CS21); 

  vSchedulerInit ();
  
  
  vSchedulerPeriodicTaskCreate(Followline, "t1", configMINIMAL_STACK_SIZE , &c1, 1, &xHandle1 , pdMS_TO_TICKS (0), pdMS_TO_TICKS (0) , pdMS_TO_TICKS (0), pdMS_TO_TICKS (0));
  vSchedulerPeriodicTaskCreate(TaskBlip , "t2",   configMINIMAL_STACK_SIZE , &c2, 1, &xHandle2 , pdMS_TO_TICKS(0), pdMS_TO_TICKS(0) , pdMS_TO_TICKS(0) , pdMS_TO_TICKS (0));
 
  vSchedulerStart ();
  
}



void loop() {
}




    
