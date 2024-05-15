#include <Arduino.h>

#define INPUT_PIN 0
#define SHORT_DURATION 100
#define LONG_DURATION 300
#define SPACE_DURATION 1000
#define OFFSET_FACTOR 0.1  // 10% offset
#define MAX_READINGS 200    // Maximum aantal metingen voor het configureren van de lichtsensor

unsigned long startTime;
unsigned long endTime;
int lightValue;
double baseline;  // Variabele om het basale lichtniveau op te slaan
String morseBuffer = "";

// Morsecode mapping naar alfabet (A-Z)
const char* morseTable[] = {
    ".-",    // A
    "-...",  // B
    "-.-.",  // C
    "-..",   // D
    ".",     // E
    "..-.",  // F
    "--.",   // G
    "....",  // H
    "..",    // I
    ".---",  // J
    "-.-",   // K
    ".-..",  // L
    "--",    // M
    "-.",    // N
    "---",   // O
    ".--.",  // P
    "--.-",  // Q
    ".-.",   // R
    "...",   // S
    "-",     // T
    "..-",   // U
    "...-",  // V
    ".--",   // W
    "-..-",  // X
    "-.--",  // Y
    "--.."   // Z
};

// Functie om te controleren of de gemeten duur binnen het acceptabele bereik ligt
bool isWithinOffset(unsigned long duration, unsigned long targetDuration) {
    unsigned long lowerBound = targetDuration * (1 - OFFSET_FACTOR);
    unsigned long upperBound = targetDuration * (1 + OFFSET_FACTOR);
    return duration >= lowerBound && duration <= upperBound;
}

// Functie om de lichtsensor te configureren en het basislichtniveau te bepalen
double configureLightSensor() {
    int readings[MAX_READINGS];  // Array om metingen op te slaan
    int count = 0;

    // Verzamel metingen
    while (count < MAX_READINGS) {
        readings[count++] = analogRead(INPUT_PIN);
    }

    // Bereken het gemiddelde van de metingen
    long sum = 0;
    for (int i = 0; i < count; i++) {
        sum += readings[i];
    }
    double mean = double(sum) / count;

    return mean;
}

void setup() {
    Serial.begin(9600);
    pinMode(INPUT_PIN, INPUT);

    // Configureer de lichtsensor en bepaal het basislichtniveau
    baseline = configureLightSensor();
    Serial.print("Baseline light level set at: ");
    Serial.println(baseline);
}

void decodeMorse(String morseCode) {
    for (int i = 0; i < 26; i++) {
        if (morseCode == morseTable[i]) {
            Serial.print((char)('A' + i)); // Converteer naar het bijbehorende alfabetisch karakter
            return;
        }
    }
    Serial.print("?"); // Onbekende Morse-sequentie
}

void loop() {
    startTime = millis();

    // Wacht tot het lichtniveau onder de basislijn daalt (d.w.z. een detectie van een stip of streep)
    while (analogRead(INPUT_PIN) >= baseline) {
        // Wacht...
    }

    endTime = millis();
    unsigned long duration = endTime - startTime;

    if (duration >= 50) {
        if (isWithinOffset(duration, SHORT_DURATION)) {
            morseBuffer += ".";
        }
        else if (isWithinOffset(duration, LONG_DURATION)) {
            morseBuffer += "-";
        }
        else if (isWithinOffset(duration, SPACE_DURATION)) {
            decodeMorse(morseBuffer); // Decodeer de opgebouwde Morse-sequentie
            morseBuffer = ""; // Reset de buffer voor de volgende karakter
            Serial.print(" "); // Voeg een spatie toe tussen woorden
        }
    }

    delay(10); // Wacht kort voordat de volgende meting wordt uitgevoerd
}
