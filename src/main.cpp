#include <Arduino.h>       //Lib for the Plaform.io
#include <PS2Keyboard.h>   //Lib for the PS2 keyboard#include <LiquidCrystal.h>
#include <LiquidCrystal.h> //Lib for the LCD display

//////C++ standard library
#include <ArduinoSTL.h>
#include <vector>
#include <string>
#include <map>
///////

//Pins for the PS2 keyboard
const int DataPin = 3;
const int IRQpin = 5;

PS2Keyboard keyboard;

//Switches between qwertz(true) and qwerty(false) layouts
#define QWERTZ true

//Pin for the buzzer
const int BuzzerPin = 4;
const int frequency = 445;      // in Hz
const int oneDotDuration = 400; // in ms

//Pins for the LCD display
const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int LCDwidth = 16, LCDheight = 2;
const char *BLANK;
const char *inputGraphics;

//If false the serial port will be closed and the code will be faster
#define SERIALOUT false

//Queue filled with chars for the translation
std::string inputQueue;
//Dictonary with the char -> morse covertion
std::map<char, std::string> Dicitonary;

//Runs at the boot time
void setup()
{
  lcd.begin(LCDwidth, LCDheight);
  BootLogo();
  //Loads the dictionary
  LoadDictionary();
  //Calibrates graphic
  CalibrateScreen();
  //Start keyBoard
  keyboard.begin(DataPin, IRQpin);
  //assigns the buzzer pin
  pinMode(BuzzerPin, OUTPUT);
#if SERIALOUT
  //starts the serial port
  Serial.begin(9600);
#endif
}

//Always repeats itself
void loop()
{
  delay(5);
  if (keyboard.available())
  {
    char input = keyboard.read();
    switch (input)
    {
    case PS2_BACKSPACE:
      if (inputQueue.length() > 0)
        inputQueue.erase(inputQueue.length() - 1);
      break;
    case PS2_ENTER:
      Output();
      inputQueue.clear();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(inputGraphics);
      break;
    default:
      input = KeyboardLayout(toUpperCase(input));
      inputQueue += input;
      break;
    }
    ClearRow(1);
    lcd.setCursor(0, 1);
    
    const char *toPrint;
    if (inputQueue.length()>LCDwidth)
      toPrint = inputQueue.substr(inputQueue.length()-LCDwidth-1).data();
    else 
      toPrint = inputQueue.data();

    lcd.write(toPrint);
  }
}

char KeyboardLayout(char input)
{
#if QWERTZ //Prepocesor for keyboard layout
  switch (input)
  {
  case 'Z':
    return 'Y';
  case 'Y':
    return 'Z';
  }
#endif
  return input;
}

//Translates char into morse code
std::string TranslateToMorse(char input)
{
  if (Dicitonary.find(input) != Dicitonary.end())
  {
    return Dicitonary[input];
  }
  else
    return "~";
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
void SoundOutput(int length, int frequency)
{
  tone(BuzzerPin, frequency);     //Starts beep
  delay(oneDotDuration * length); //Space between beeps
  noTone(BuzzerPin);              //Ends the beep
}

void VisualOutput(int index, const char *morse)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(inputQueue[index] + ":");
  lcd.write(morse);
  if (inputQueue.length() - index > 8)
    lcd.write(inputQueue.substr(index, LCDwidth).data());
  else
    lcd.write(inputQueue.substr(index).data());
}

void Output()
{
  //Loop that goes trough all the letters
  for (size_t i = 0; i < inputQueue.length(); i++)
  {
    std::string morse = TranslateToMorse(inputQueue[i]);
#if SERIALOUT
    SerialOutput(translated, morse);
#endif
    ////Sound and Visual output
    if (morse == "SPACE")
    {
      VisualOutput(i, "SPACE");
      SoundOutput(6, frequency);
    }
    else if (morse == "~")
    {
      VisualOutput(i, "ERROR");
      SoundOutput(6, 200);
    }
    else
    {
      VisualOutput(i, morse.data());
      //Loop that goes trough all of the lines and dots in the translation(string)
      for (int morseIndex = 0; i < morse.length(); i++)
      {
        //Decides how long the beep should be
        if (inputQueue[i] == '.')
          SoundOutput(1, frequency);
        else
          SoundOutput(3, frequency);
      }
      delay(oneDotDuration * 3); //Space between chars
    }
  }
}

void ClearRow(int row)
{
  lcd.setCursor(0, row);
  lcd.write(BLANK);
}

void CalibrateScreen()
{
  std::string tempBLANK(LCDwidth, ' ');
  BLANK = tempBLANK.data();
  std::string tempGraphics(LCDwidth, '-');
  tempGraphics.replace(LCDwidth / 2, 7, "↓INPUT↓");
  inputGraphics = tempGraphics.data();
}

void BootLogo()
{
  lcd.clear();
  lcd.home();
  lcd.write("MORSE TRANSLATOR");
  lcd.setCursor(0, 1);
  lcd.write("  By  Gajdušek");
  delay(300);
  lcd.write("   By  Kesner");
  delay(300);
}

//Sets up the Dictionary
void LoadDictionary()
{
  Dicitonary['A'] = "._";
  Dicitonary['B'] = "_...";
  Dicitonary['C'] = "_._.";
  Dicitonary['D'] = "_..";
  Dicitonary['E'] = ".";
  Dicitonary['F'] = ".._.";
  Dicitonary['G'] = "__.";
  Dicitonary['H'] = "....";
  Dicitonary['I'] = "..";
  Dicitonary['J'] = ".___";
  Dicitonary['K'] = "_._";
  Dicitonary['L'] = "._..";
  Dicitonary['M'] = "__";
  Dicitonary['N'] = "_.";
  Dicitonary['O'] = "___";
  Dicitonary['P'] = ".__.";
  Dicitonary['Q'] = "__._";
  Dicitonary['R'] = "._.";
  Dicitonary['S'] = "...";
  Dicitonary['T'] = "_";
  Dicitonary['U'] = ".._";
  Dicitonary['V'] = "..._";
  Dicitonary['W'] = ".__";
  Dicitonary['X'] = "_.._";
  Dicitonary['Y'] = "_.__";
  Dicitonary['Z'] = "__..";
  Dicitonary[' '] = "SPACE";
}
