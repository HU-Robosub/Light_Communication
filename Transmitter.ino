#include <Wire.h>

#define inputPin 0
#define outputPin 13

void setup() {
  Serial.begin(9600);
  if (!Serial) {
    exit(1);
  }
  Serial.println("Configured Serial Communication.");

  pinMode(inputPin, INPUT);
  Serial.print("Set Pin: ");
  Serial.print(inputPin);
  Serial.println(" as INPUT.");

  pinMode(outputPin, OUTPUT);
  Serial.print("Set Pin: ");
  Serial.print(outputPin);
  Serial.println("as OUTPUT.");

  Serial.println("Startup finished!");
}

void loop() {
  String userInput;
  if (Serial.available()) {
   userInput = Serial.readStringUntil('\n');
  }
  if (userInput != ""){
    Serial.println(userInput);
  }
  String morseUserInput = translateToMorse(userInput);
  if (morseUserInput != "") {
    Serial.println(morseUserInput); 
  }
  morseBasedLight(morseUserInput); 
}

void flashLight(int pin, int milli) {
  digitalWrite(pin, HIGH);
  delay(milli);
  digitalWrite(pin, LOW);
  delay(300); 
}

void morseBasedLight(String morseInput) {
  for (char Morse : morseInput) {
    switch (Morse) {
      case '.':
        flashLight(outputPin, 100);
        break;
      case '-':
        flashLight(outputPin, 300);
        break;
      case ' ':
        flashLight(outputPin, 1000);
        break;
      default:
        Serial.println("No case found, defaulting");
        break;
    }
  }
}

String translateToMorse(String inputText) {
  String morseCode = "";
  for (char ch : inputText) {
    switch (ch) {
      case 'a':
        morseCode += ".- ";
        break;
      case 'b':
        morseCode += "-... ";
        break;
      case 'c':
        morseCode += "-.-. ";
        break;
      case 'd':
        morseCode += "-.. ";
        break;
      case 'e':
        morseCode += ". " ;
        break;
      case 'f':
        morseCode += "..-. ";
        break;
      case 'g':
        morseCode += "--. ";
        break;
      case 'h':
        morseCode += ".... ";
        break;
      case 'i':
        morseCode += ".. ";
        break;
      case 'j':
        morseCode += ".--- ";
        break;
      case 'k':
        morseCode += "-.- ";
        break;
      case 'l':
        morseCode += ".-.. ";
        break;
      case 'm':
        morseCode += "-- ";
        break;
      case 'n':
        morseCode += "-. ";
        break;
      case 'o':
        morseCode += "--- ";
        break;
      case 'p':
        morseCode += ".--. ";
        break;
      case 'q':
        morseCode += "--.- ";
        break;
      case 'r':
        morseCode += ".-. ";
        break;
      case 's':
        morseCode += "... ";
        break;
      case 't':
        morseCode += "- ";
        break;
      case 'u':
        morseCode += "..- ";
        break;
      case 'v':
        morseCode += "...- ";
        break;
      case 'w':
        morseCode += ".-- ";
        break;
      case 'x':
        morseCode += "-..- ";
        break;
      case 'y':
        morseCode += "-.-- ";
        break;
      case 'z':
        morseCode += "--.. ";
        break;
      case ' ':
        morseCode += " ";
        break;
      default:
        morseCode += "";
        break;
    }
  }
  return morseCode;
}
