char val0;
int LED = 5; // Set LED pin at D5
int val = 0; // Read the voltage value of the photodiode

void setup () {
       pinMode (LED, OUTPUT); // LED is output
       Serial.begin (9600); // The serial port baud rate is set to 9600

//human sensor
pinMode (2, INPUT); // Define PIR as input in D2
   Serial.begin (9600);
   pinMode (13, OUTPUT); // Define LED as output in D13
   pinMode (7, OUTPUT); // Define D7 as output
   pinMode (6, OUTPUT); // Define D6 as output
}



void loop () {
  while (Serial.available()>0)
{
val0=Serial.read();// Read the value sent by Bluetooth
Serial.print(val0);// The serial port prints the read value
}
       val = analogRead (A1); // Read the voltage value of A1 Pin
       Serial.println (val0); // Serial port to view the change of voltage value

       if (val <900)
       {// Less than 1000, LED light is off
       digitalWrite (LED, HIGH);
       } 
       else 
       {// Otherwise, the LED lights up
       digitalWrite (LED, LOW);
       }
       delay (10); // Delay 10ms
       
//human detector
   Serial.println (digitalRead (2));
   delay (500); // Delay 500ms
   if (digitalRead (2) == 1) // If someone is detected walking
  {
     //digitalWrite (13, HIGH); // LED light is on
     digitalWrite (7, HIGH);
     analogWrite (6,150); // Fan rotates

   } else // If no person is detected walking
{
     //digitalWrite (13, LOW); // LED light is not on
     digitalWrite (7, LOW);
     analogWrite (6,0); // The fan does not rotate
   }
}
