#include<Arduino.h> //Lib for the Plaform.io
#include <PS2Keyboard.h> //Lib for the PS2 keyboard

//////C++ standard library
#include <ArduinoSTL.h> 
#include<vector>
#include<string>
#include<map>
///////

//Pins for the PS2 keyboard
const int DataPin = 3;
const int IRQpin =  5;

PS2Keyboard keyboard;

//Switches between qwertz(true) and qwerty(false) layouts
bool qwertzEnabled = true;

//Pin for the buzzer
const int BuzzerPin = 4;
const int frequency = 445; // in Hz
const int oneDotDuration = 400; // in ms


//Queue filled with chars for the translation
std::vector<char> inputQueue;
//Dictonary with the char -> morse covertion
std::map<char, std::string> Dicitonary;

//Runs at the boot time
void setup()
{
  delay(2000);
  SetDictionary();
  //Sets up the interrupt method for input
  attachInterrupt(digitalPinToInterrupt(DataPin), InputInterupt, CHANGE);
  //Start keyBoard
  keyboard.begin(DataPin, IRQpin);
  //assigns the buzzer pin
  pinMode(BuzzerPin, OUTPUT);
  //starts the serial port
  Serial.begin(9600);
}

//Always repets itself
void loop()
{
  delay(20);
  if (inputQueue.size() > 0)
  {
    char toTranslateChar = KeyboardLayoutApply(inputQueue[0]);
    std::string outputMorse = TranslateToMorse(toTranslateChar);
    SerialOutput(toTranslateChar, outputMorse);
    SoundOutput(outputMorse);
    inputQueue.erase(0);
  }
}


//Method which is called when keyboard is pressed (called by the Interrupt method)
void InputInterupt()
{
  if (keyboard.available())
  {
    inputQueue.push_back(tolower(keyboard.read()));
  }
}

//Translates char into morse code
std::string TranslateToMorse(char input)
{ 
  return Dicitonary[input];
}

//changes char depending on layout
char KeyboardLayoutApply(char input)
{
  if (qwertzEnabled)
  {
    switch (input)
    {
    case 'z':
      return 'y';
    case 'y':
      return 'z';
    }
  }
  return input;
}


//Output method for Serial port
void SerialOutput(char currentChar, std::string translation)
{
  Serial.print("Char: ");
  Serial.print(currentChar);
  Serial.print(" Translation: ");
  Serial.println(translation.c_str());
}

//Output method for the buzzer
void SoundOutput(std::string translation)
{
  //Loop that goes trough all of the lines and dots in the translation(string)
  for (size_t i = 0; i < translation.length(); i++)
  {
    tone(BuzzerPin,frequency); //Starts beep

    //Decides how long the beep should be
    if (translation[i] == '.') delay(oneDotDuration); //Short beep
    else delay(oneDotDuration*3); //Long beep
    
    noTone(BuzzerPin); //Ends the beep
    delay(oneDotDuration); //Space between beeps
  }
  delay(oneDotDuration*3); //Space between chars
}




//Sets up the Dictionary
void SetDictionary()
{
  Dicitonary['a'] = "._";
  Dicitonary['b'] = "_...";
  Dicitonary['c'] = "_._.";
  Dicitonary['d'] = "_..";
  Dicitonary['e'] = ".";
  Dicitonary['f'] = ".._.";
  Dicitonary['g'] = "__.";
  Dicitonary['h'] = "....";
  Dicitonary['i'] = "..";
  Dicitonary['j'] = ".___";
  Dicitonary['k'] = "_._";
  Dicitonary['l'] = "._..";
  Dicitonary['m'] = "__";
  Dicitonary['n'] = "_.";
  Dicitonary['o'] = "___";
  Dicitonary['p'] = ".__.";
  Dicitonary['q'] = "__._";
  Dicitonary['r'] = "._.";
  Dicitonary['s'] = "...";
  Dicitonary['t'] = "_";
  Dicitonary['u'] = ".._";
  Dicitonary['v'] = "..._";
  Dicitonary['w'] = ".__";
  Dicitonary['x'] = "_.._";
  Dicitonary['y'] = "_.__";
  Dicitonary['z'] = "__..";
}