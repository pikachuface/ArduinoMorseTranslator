#include <Arduino.h>
#include <PS2Keyboard.h>
#include<StandardCplusplus.h>
#include<string>
#include<vector>
#include<map>

const int DataPin = 3;
const int IRQpin =  5;

PS2Keyboard keyboard;

std::vector<char> inputQueue;
std::vector<std::string> outputQueue;

std::map<char,std::string> Dicitonary;


void setup() 
{
  
  delay(1000);
  attachInterrupt(digitalPinToInterrupt(DataPin),InputInterupt, CHANGE);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
}

void loop() 
{

}

void InputInterupt()
{
  if (keyboard.available())
  {
    inputQueue.push_back(tolower(keyboard.read()));
  }
}

String TranslateToMorse(char input)
{
  if (inputQueue.size()>0)
  {
    switch (input)
    {
    case 'a':
      outputQueue.push_back()
      break;
    }
  }
}

void SetDictionary()
{
  Dicitonary['a']





}

  