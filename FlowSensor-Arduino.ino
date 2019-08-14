volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // FlowSensor Input
unsigned char motorCutoff = 12; // Relay Output
unsigned long currentTime;
unsigned long cloopTime;

unsigned long previousMillis = 0; // last time update
unsigned long currentMillis;
long interval = 120000; // interval at which to do something (milliseconds)
int minimum_flow = 50; // We expect to flow of water atleast 50L per hour, otherwise we default it to dryrun
bool dry_run,first_run_state;

//Float Sensor Connection, Terminal 1 Gnd, Terminal 2 D8
#define FLOAT_SENSOR  8

void flow () // Interrupt function
{
  flow_frequency++;
}



void setup()
{
  
  pinMode(flowsensor, INPUT);
  pinMode(motorCutoff, OUTPUT);
  digitalWrite(motorCutoff, HIGH);
  digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
  pinMode(FLOAT_SENSOR, INPUT_PULLUP);
  Serial.begin(9600);
  attachInterrupt(0, flow, RISING); // Setup Interrupt
  sei(); // Enable interrupts
  currentTime = millis();
  cloopTime = currentTime;
  dry_run = false;
//  first_run_state = true;
  Serial.println("Intiating 30 seconds Delay");
  delay(30000);
}

void loop ()
{
//   delay(1000);
//   if (first_run_state)
//    {
////      For first run, the status of first_run_state is true and will be set false after 30secs.
//      Serial.print(": Just started, waits for 30 sec before actual calculations, First Run state is :"); 
//      Serial.println(first_run_state); 
//      delay(1000*30);
//      first_run_state = !first_run_state; //Toggle first_run state
//      Serial.print("Current First Run state is :"); 
//      Serial.println(first_run_state); 
//    }

    if(digitalRead(FLOAT_SENSOR) == LOW) 
    {
    Serial.print("Float Sensor Reading is Low");
    Serial.println(": Tank is still empty");
    delay(1000*1);
    // turn Motor on:
//    digitalWrite(motorCutoff, HIGH);
    } 
    else 
    {
    Serial.print("Float Sensor Reading is High"); 
    Serial.println(": Tank is Full, Turning off Motor now");
    delay(1000*1);
    // turn Motor off:
    digitalWrite(motorCutoff, LOW);
    }
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter
      Serial.print(l_hour, DEC); // Print litres/hour
      Serial.println(" L/hour");

      if (l_hour<minimum_flow)
      {
        currentMillis = millis();
        if (not dry_run)
        {
          dry_run = true;
          currentMillis = millis();
          previousMillis = currentMillis;
        }
        else if(currentMillis - previousMillis > interval) 
          {
            previousMillis = currentMillis;
            if (l_hour<minimum_flow && dry_run )
            {
             Serial.print(l_hour, DEC); // Print litres/hour
             Serial.print(" L/hour; Currently DRY Running");
             delay(1000*5);
             Serial.println(" :Sending Turn Off Signal to Motor Now");
             //At this stage send TURN OFF Signal to Motor
             digitalWrite(motorCutoff, LOW);
            }
          }
        
      }
      else
      {
//      digitalWrite(motorCutoff, HIGH);
      dry_run = false;
      previousMillis = 0;
      currentMillis = millis();
      }
   }
}
