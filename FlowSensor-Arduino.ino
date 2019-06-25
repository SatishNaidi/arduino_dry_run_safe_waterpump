volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // FlowSensor Input
unsigned char motorCutoff = 13; // Relay Output
unsigned long currentTime;
unsigned long cloopTime;

unsigned long previousMillis = 0; // last time update
unsigned long currentMillis;
long interval = 10000; // interval at which to do something (milliseconds)
int minimum_flow = 50; // We expect to flow of water atleast 50L per hour, otherwise we default it to dryrun
bool dry_run,first_run_state;

void flow () // Interrupt function
{
  flow_frequency++;
}

void setup()
{
  pinMode(flowsensor, INPUT);
  pinMode(motorCutoff, OUTPUT);
  digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
  Serial.begin(9600);
  attachInterrupt(0, flow, RISING); // Setup Interrupt
  sei(); // Enable interrupts
  currentTime = millis();
  cloopTime = currentTime;
  dry_run = false;
  first_run_state = true;
}

void loop ()
{
   if (first_run_state)
    {
      Serial.print(first_run_state);
      Serial.println(": Just started, waits for 30 sec before actual calculations"); 
      delay(1000*3);
      first_run_state = !first_run_state; //Toggle first_run state
      Serial.print(first_run_state);
      Serial.println(": Just started, waits for 30 sec before actual calculations"); 
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
              Serial.println(" L/hour; Currently DRY Running");
//              At this stage send TURN OFF Signal to Motor
             digitalWrite(motorCutoff, HIGH);
            }
          }    
      }
      else
      {
      digitalWrite(motorCutoff, LOW);
      dry_run = false;
      previousMillis = 0;
      currentMillis = millis();
      }
   }
}
