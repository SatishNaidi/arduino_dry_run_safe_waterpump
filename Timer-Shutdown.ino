
const int RelayPin =  8;      // the number of the PUMP pin
const int pulseInput = 11;                // the number of the PULSE pin

unsigned long startMillis = 0;    // will store last time status was updated


// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long interval = 240000;          // interval of 4 minutes

//240000
void setup() {
  pinMode(RelayPin, OUTPUT); 
  digitalWrite(RelayPin, HIGH);
  Serial.begin(9600);
  Serial.println("Begin");
}

void loop()
{
  delay(5000);
  unsigned long currentMillis = millis(); 
  Serial.print("Current wait time: ");
  Serial.println(currentMillis);
  Serial.print("Interval Time: ");
  Serial.println(interval);
  if (currentMillis > interval){
    Serial.print("Intial Delay:");
    Serial.println(interval);
    Serial.print("Wait time is completed: ");
    Serial.println(currentMillis);
    digitalWrite(RelayPin, LOW);
  }
 else {
    digitalWrite(RelayPin, HIGH);
  }
}
