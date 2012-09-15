	/*
  SparkFun Electronics 2010
  Playing with IR remote control
  
  IR Receiver TSOP382: Supply voltage of 2.5V to 5.5V
  With the curved front facing you, pin 1 is on the left.
  Attach
    Pin 1: To pin 2 on Arduino
    Pin 2: GND
    Pin 3: 5V
  
  This is based on pmalmsten's code found on the Arduino forum from 2007:
  http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1176098434/0

  This code works with super cheapo remotes. If you want to look at the individual timing
  of the bits, use this code:
  http://www.arduino.cc/playground/Code/InfraredReceivers
  
  This code clips a lot of the incoming IR blips, but what is left is identifiable as key codes.

*/

int irPin = 2; //Sensor pin 1 wired to Arduino's pin 2
int statLED = 13; //Toggle the status LED every time Power is pressed
int start_bit = 2200; //Start bit threshold (Microseconds)
int bin_1 = 1000; //Binary 1 threshold (Microseconds)
int bin_0 = 400; //Binary 0 threshold (Microseconds)
int brightness = 0;    // how bright the LED is
int fadeAmount = 20;    // how many points to fade the LED by

void setup() {
  pinMode(statLED, OUTPUT);
  digitalWrite(statLED, LOW);

  pinMode(irPin, INPUT);

  Serial.begin(9600);
  Serial.println("Waiting: ");
}

void loop() {
  int key = getIRKey();		    //Fetch the key
  
  if(key != 0) //Ignore keys that are zero
  {
    Serial.print("Key Recieved: ");
    switch(key)
    {
      case -1: Serial.print("CH Up");
    if (digitalRead(statLED) !=1)
  digitalWrite(statLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);               // wait for a second
  digitalWrite(statLED, LOW);    // turn the LED off by making the voltage LOW
  delay(500);               // wait for a second
  
  digitalWrite(statLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);               // wait for a second
  digitalWrite(statLED, LOW);    // turn the LED off by making the voltage LOW
  delay(500);               // wait for a second
  
  digitalWrite(statLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);               // wait for a second
  digitalWrite(statLED, LOW);    // turn the LED off by making the voltage LOW
  delay(500);               // wait for a second
  break;
      case 16: Serial.print("CH Down"); 
      if(digitalRead(statLED) !=1)
       digitalWrite(statLED, brightness);    

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade: 
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ; 
  }     
  // wait for 30 milliseconds to see the dimming effect    
  delay(30);   
      
      break;
      case 49651807: Serial.print("VOL Right"); break;
      case 49629367: Serial.print("VOL Left"); break;
      case 2: Serial.print("Mute");
if (digitalRead(statLED) !=1)
  digitalWrite(statLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(statLED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
  
  digitalWrite(statLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(statLED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
  
  digitalWrite(statLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(statLED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
  
break;
  
      case 49635487: Serial.print("AV/TV"); break;
      case 682: 
        Serial.print("Power");
        if(digitalRead(statLED) != 1) //This toggles the statLED every time power button is hit
          digitalWrite(statLED, HIGH);
        else
          digitalWrite(statLED, LOW);
        break;

      default: Serial.print(key);
    }

    Serial.println();
  }
}

int getIRKey() {
  int data[12];
  int i;

  while(pulseIn(irPin, LOW) < start_bit); //Wait for a start bit
  
  for(i = 0 ; i < 11 ; i++)
    data[i] = pulseIn(irPin, LOW); //Start measuring bits, I only want low pulses
  
  for(i = 0 ; i < 11 ; i++) //Parse them
  {	    
    if(data[i] > bin_1) //is it a 1?
      data[i] = 1;
    else if(data[i] > bin_0) //is it a 0?
      data[i] = 0;
    else
      return -1; //Flag the data as invalid; I don't know what it is! Return -1 on invalid data
  }

  int result = 0;
  for(i = 0 ; i < 11 ; i++) //Convert data bits to integer
    if(data[i] == 1) result |= (1<<i);

  return result; //Return key number
} 
