// macros

// !!!! interrupt pins are only 2 and 3 
#define encoder_pin_a 2
#define encoder_pin_b 3

// for simulation
        // toggle direction pin for simulation
        #define toggle_pin 4
        #define first_direction_pin 5
        #define second_direction_pin 6 

////////////////////////////////////////////////////////////////////////////

// global variables
long doubled_counter = 0;

// for simulation
          int first_direction = 1 ; // boolean means that I want motor to rotates in the first direction
          
          //states of the push button 
          int prev_state = 0;
          int current_state = 0;

////////////////////////////////////////////////////////////////////////////

// functions declarations

void pin_a_trigger();
void pin_b_trigger();

// for simulation
            void toggle_if_rising();

////////////////////////////////////////////////////////////////////////////

// main program 

void setup() {

  Serial.begin(9600);

  // pullup to set the input signal to up by default
  pinMode (encoder_pin_a , INPUT_PULLUP);
  pinMode (encoder_pin_b , INPUT_PULLUP);

  // for simulation
          //deploying simulation pins
          pinMode( first_direction_pin , OUTPUT );
          pinMode( second_direction_pin , OUTPUT );
          pinMode(toggle_pin , INPUT);

          // motor by default rotates in the first direction
          digitalWrite(first_direction_pin, first_direction);
          digitalWrite(second_direction_pin, !first_direction);

  // attach interrupt attaches a function to work whenever a change is done in the pin_a of the encoder
  attachInterrupt(digitalPinToInterrupt(encoder_pin_a), pin_a_trigger , CHANGE);
  
  // for simulation
              attachInterrupt(digitalPinToInterrupt(toggle_pin), toggle_if_rising , RISING);
}

void loop() {

  Serial.print("the counter is ");
  Serial.println((doubled_counter/2));
  toggle_if_rising();
}

////////////////////////////////////////////////////////////////////////////

// functions definitions

void pin_a_trigger(){

  if (digitalRead(encoder_pin_a) != digitalRead(encoder_pin_b)){
    doubled_counter ++;
  }
  
  else{
    doubled_counter --;
  }

}

//for simulation  
                void toggle_if_rising(){
                  
                  current_state = digitalRead(toggle_pin);

                  // if the signal is rising rising
                  if( current_state > prev_state){
                    first_direction = !first_direction;
                    digitalWrite(first_direction_pin, first_direction);
                    digitalWrite(second_direction_pin, !first_direction);
                    Serial.println("ytahhhak");
                  }

                  prev_state = current_state;
                }
