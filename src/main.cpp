#include<Arduino.h>
#include <ArduinoSTL.h>
#include <PS2Keyboard.h>
#include<vector>
#include<string>
#include<map>


const int DataPin = 3;
const int IRQpin =  5;

PS2Keyboard keyboard;

std::vector<char> inputQueue;

std::map<char, std::string> Dicitonary;


void setup()
{
  SetDictionary();
  delay(1000);
  attachInterrupt(digitalPinToInterrupt(DataPin), InputInterupt, CHANGE);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
}

void loop()
{
  delay(20);
  if (inputQueue.size() > 0)
  {
    char toTranslateChar = inputQueue[0];
    std::string outputMorse = TranslateToMorse(toTranslateChar);
    SerialOutput(toTranslateChar, outputMorse);
    SoundOutput(outputMorse);
    inputQueue.erase(0);
  }
}



void InputInterupt()
{
  if (keyboard.available())
  {
    inputQueue.push_back(tolower(keyboard.read()));
  }
}


std::string TranslateToMorse(char input)
{
  return Dicitonary[input];
}



void SerialOutput(char currentChar, std::string translation)
{
  Serial.print("Char: ");
  Serial.print(currentChar);
  Serial.print(" Translation: ");
  Serial.print(translation.c_str());
  Serial.println();
}

void SoundOutput(std::string translation)
{
  for (size_t i = 0; i < translation.length(); i++)
  {
    if (translation[i] == '.')
    {
      /*Code for short beep*/
    } else
    {
      /*Code for long beep*/
    }
  }
}





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