/******************************************************************************
  Micro-OLED-Shield-Example.ino
  SparkFun Micro OLED Library Hello World Example
  Jim Lindblom @ SparkFun Electronics
  Original Creation Date: June 22, 2015

  This sketch prints a friendly, recognizable logo on the OLED Shield, then
  goes on to demo the Micro OLED library's functionality drawing pixels,
  lines, shapes, and text.

  Hardware Connections:
	This sketch was written specifically for the Photon Micro OLED Shield,
	which does all the wiring for you. If you have a Micro OLED breakout,
	use the following hardware setup:

    MicroOLED ------------- Photon
      GND ------------------- GND
      VDD ------------------- 3.3V (VCC)
    D1/MOSI ----------------- A5 (don't change)
    D0/SCK ------------------ A3 (don't change)
      D2
      D/C ------------------- D6 (can be any digital pin)
      RST ------------------- D7 (can be any digital pin)
      CS  ------------------- A2 (can be any digital pin)

  Development environment specifics:
  	IDE: Particle Build
  	Hardware Platform: Particle Photon
                       SparkFun Photon Micro OLED Shield

  This code is beerware; if you see me (or any other SparkFun
  employee) at the local, and you've found our code helpful,
  please buy us a round!

  Distributed as-is; no warranty is given.
*******************************************************************************/
#include "SparkFunMicroOLED.h"  // Include MicroOLED library
#include "math.h"

//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////
// Declare a MicroOLED object. If no parameters are supplied, default pins are
// used, which will work for the Photon Micro OLED Shield (RST=D7, DC=D6, CS=A2)
MicroOLED oled;
//MicroOLED oled(MODE_I2C, D7, 0);    // Example I2C declaration RST=D7, DC=LOW (0)

//SYSTEM_MODE(MANUAL);
String sText = "Hello";

int ledPin = D0;
int pirPin = A0;
int speakerPin = TX;

int val;
bool bOn=true;

int motionVal;

int pos=0;
String motionSong="G7,16";

void setup()
{
    Serial.begin(9600);
    Particle.function("setText", setText);
    oled.begin();    // Initialize the OLED
    oled.clear(ALL); // Clear the display's internal memory
    oled.display();  // Display what's in the buffer (splashscreen)
    delay(1000);     // Delay 1000 ms
    randomSeed(analogRead(A0) + analogRead(A1));
    //oPrint("Hello", 0);
    //oled.setColor(0xff0000);
    oled.clear(PAGE);
    oled.setCursor(0,0);
    oled.setFontType(1);
    oled.print(sText);
    oled.display();
    delay(500);
    oled.scrollRight(0,110);

	Particle.function("lightOn", lightOn);

	pinMode(speakerPin, OUTPUT);
    pinMode(ledPin, OUTPUT);

    Particle.function("setSong", setSong);

    playSong("C4,1000");

}

int setText(String sCommand)
{
    sText = sCommand;
    oled.clear(ALL);
    oled.display();
    oled.setCursor(0,0);
    oled.setFontType(1);
    oled.print(sText);
    oled.display();
    delay(500);
    oled.scrollRight(0,110);

    return 1;
}

void loop()
{
	//This will run in a loop
    if(bOn)
        analogWrite(ledPin, 255);  // analogRead values go from 0 to 4095, analogWrite values from 0 to 255
    else
        analogWrite(ledPin, 0);

    motionVal = 0;
    for(int i = 0; i < 100; i += 1)
    {
        int rval = analogRead(pirPin);
        if (rval > motionVal)
        {
            motionVal = rval;
        }
        delay(10);
    }
    if(motionVal > 1000)
    {
        bOn=true;
        playSong(motionSong);
        oled.clear(ALL);
        oled.display();
        oPrint("Motion!",1);
        delay(500);
        oled.clear(ALL);
        oled.display();
        oPrint("Waiting", 1);
        oled.scrollRight(0,110);

    }
    else
    {
        bOn=false;
    }

}

void oPrint(String sItem, int nFont)
{
    oled.clear(ALL);            // Clear the display
    oled.setCursor(0, 0);        // Set cursor to top-left
    oled.setFontType(nFont);         // Smallest font
    oled.print(sItem);          // Print "A0"
    oled.display();
}
void textExamples()
{

  // Demonstrate font 2. 10x16. Only numbers and '.' are defined.
  // This font looks like 7-segment displays.
  // Lets use this big-ish font to display readings from the
  // analog pins.
  for (int i=0; i<25; i++)
  {
    oled.clear(PAGE);            // Clear the display
    oled.setCursor(0, 0);        // Set cursor to top-left
    oled.setFontType(0);         // Smallest font
    oled.print("A0:");          // Print "A0"
    oled.setFontType(0);         // 7-segment font
    oled.print(analogRead(A0));  // Print a0 reading
    oled.setCursor(0, 16);       // Set cursor to top-middle-left
    oled.setFontType(0);         // Repeat
    oled.print("A1:");
    oled.setFontType(0);
    oled.print(analogRead(A1));
    oled.setCursor(0, 32);
    oled.setFontType(0);
    oled.print("A7:");
    oled.setFontType(0);
    oled.print(analogRead(A7));
    oled.display();
    delay(100);
  }

return;

  // Demonstrate font 3. 12x48. Stopwatch demo.
  oled.setFontType(3);  // Use the biggest font
  int ms = 0;
  int s = 0;
  while (s <= 50)
  {
    oled.clear(PAGE);     // Clear the display
    oled.setCursor(0, 0); // Set cursor to top-left
    if (s < 10)
      oled.print("00");   // Print "00" if s is 1 digit
    else if (s < 100)
      oled.print("0");    // Print "0" if s is 2 digits
    oled.print(s);        // Print s's value
    oled.print(":");      // Print ":"
    oled.print(ms);       // Print ms value
    oled.display();       // Draw on the screen
    ms++;         // Increment ms
    if (ms >= 10) // If ms is >= 10
    {
      ms = 0;     // Set ms back to 0
      s++;        // and increment s
    }
    delay(1);
  }
}

// Center and print a small title
// This function is quick and dirty. Only works for titles one
// line long.
void printTitle(String title, int font)
{
  int middleX = oled.getLCDWidth() / 2;
  int middleY = oled.getLCDHeight() / 2;

  oled.clear(PAGE);
  oled.setFontType(font);
  // Try to set the cursor in the middle of the screen
  oled.setCursor(middleX - (oled.getFontWidth() * (title.length()/2)),
                 middleY - (oled.getFontWidth() / 2));
  // Print the title:
  oled.print(title);
  oled.display();
  delay(1500);
  oled.clear(PAGE);
}


int lightOn(String input)
{
    if(strcmp(input, "on")==0)
        bOn=TRUE;
    else
        bOn=FALSE;

    return 1;
}
int setSong(String input)
{
    motionSong = input;
    return 1;
}
//
// stolen from InternetButton
//
void playSong(String song){
    char inputStr[200];
    song.toCharArray(inputStr,200);

    Serial.println(inputStr);

    char *note = strtok(inputStr,",");
    char *duration = strtok(NULL,",");
    playNote(note,atoi(duration));

    while(duration != NULL){
        note = strtok(NULL,",");
        Serial.println(note);
        duration = strtok(NULL,", \n");
        Serial.println(duration);
        //if(atoi(duration) <= 0){
        //    break;
        //}
        playNote(note,atoi(duration));
    }
}

void playNote(String note, int duration){
    int noteNum = 0;
    int octave = 5;
    int freq = 256;

     //if(9 - int(command.charAt(1)) != null){
    char octavo[5];
    String tempString = note.substring(1,2);
    tempString.toCharArray(octavo,5);
    octave = atoi(octavo);
    //}

    if(duration != 0){
        duration = 1000/duration;
    }

    switch(note.charAt(0)){
        case 'C':
            noteNum = 0;
            break;
        case 'D':
            noteNum = 2;
            break;
        case 'E':
            noteNum = 4;
            break;
        case 'F':
            noteNum = 5;
            break;
        case 'G':
            noteNum = 7;
            break;
        case 'A':
            noteNum = 9;
            break;
        case 'B':
            noteNum = 11;
            break;
        case 'R':          // Rest note
            octave = -1;
            break;
        default:
            break;
            //return -1;
    }

    // based on equation at http://www.phy.mtu.edu/~suits/NoteFreqCalcs.html and the Verdi tuning
    // fn = f0*(2^1/12)^n where n = number of half-steps from the reference frequency f0
    freq = float(256*pow(1.05946,(     12.0*(octave-4)        +noteNum)));
    //          C4^  (2^1/12)^    12 half-steps in an octave      ^how many extra half-steps within that octave, 0 for a C

    tone(speakerPin,int(freq),duration);
    delay(duration);
    noTone(speakerPin);
    //return freq;
}
