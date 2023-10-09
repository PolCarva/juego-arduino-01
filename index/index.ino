#include <Adafruit_NeoPixel.h>

#define PIN 5
#define NUMPIXELS 8

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int generatedSequence[NUMPIXELS];

#define BUTTONS_PER_TEAM 4

int buttonPins1[BUTTONS_PER_TEAM] = {13, 12, 11, 10};
int buttonPins2[BUTTONS_PER_TEAM] = {9, 8, 7, 6};

int playerSequences1[NUMPIXELS];
int playerSequences2[NUMPIXELS];

int currentButtonIndex1 = 0;
int currentButtonIndex2 = 0;

int currentButtonIndex1Reverse = 8;
int currentButtonIndex2Reverse = 8;

bool gameFinished = false; // Controlar si el juego ha finalizado
String winningTeam; // Almacena el nombre del equipo ganador

void setup() {
  Serial.begin(9600);
  pixels.begin();
  
  for (int i = 0; i < BUTTONS_PER_TEAM; i++) {
    pinMode(buttonPins1[i], INPUT_PULLUP);
    pinMode(buttonPins2[i], INPUT_PULLUP);
  }

  randomSeed(analogRead(0));
  generarColoresAleatorios();
}

void loop() {
    Serial.println(generatedSequence[0]);
  while (!gameFinished) {
    checkTeam(buttonPins1, playerSequences1, currentButtonIndex1,  currentButtonIndex1Reverse, "Equipo 1");
    checkTeam(buttonPins2, playerSequences2, currentButtonIndex2, currentButtonIndex1Reverse, "Equipo 2");
  }
  
  Serial.println(winningTeam + " GANÓ!");
  delay(3000); // Pausa para que los jugadores vean el resultado
  resetGame();
}

void checkTeam(int buttonPins[], int playerSequences[], int &currentButtonIndex,int &currentButtonIndexReverse, String teamName) {
  updateTeam(buttonPins, playerSequences, currentButtonIndex, currentButtonIndexReverse, teamName);
  
  if (completedSequence(currentButtonIndex, currentButtonIndexReverse)) {
    if (!gameFinished) { // Verifica si el juego ya fue ganado por el otro equipo
      gameFinished = true;
      winningTeam = teamName;
    }
  }
}

int getButtonPressed(int buttonPins[]) {
  for (int i = 0; i < BUTTONS_PER_TEAM; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      return i;
    }
  }
  return -1;
}

void updateTeam(int buttonPins[], int playerSequences[], int &currentButtonIndex, int &currentButtonIndexReverse, String teamName) {
  int buttonPressed = getButtonPressed(buttonPins);

  if (buttonPressed != -1) {
    switch (buttonPressed) {
      case 0:
        Serial.println(teamName + " ha presionado ROJO");
        break;
      case 1:
        Serial.println(teamName + " ha presionado AMARILLO");
        break;
      case 2:
        Serial.println(teamName + " ha presionado VERDE");
        break;
      case 3:
        Serial.println(teamName + " ha presionado AZUL");
        break;
    }

    playerSequences[currentButtonIndex] = buttonPressed;
    if(playerSequences[currentButtonIndex] == generatedSequence[currentButtonIndex]) {
      currentButtonIndex++;} else {
      currentButtonIndex = 0;
    }
    if(playerSequences[currentButtonIndexReverse] == generatedSequence[currentButtonIndexReverse]) {
    currentButtonIndexReverse--;
    } else {
    currentButtonIndexReverse = 8;
    }
    delay(300);  // Debounce
  }
}

bool completedSequence(int &currentButtonIndex, int &currentButtonIndexReverse) {
  return currentButtonIndex >= NUMPIXELS || currentButtonIndexReverse <= 0;
}

void resetGame() {
  gameFinished = false;
  currentButtonIndex1 = 0;
  currentButtonIndex2 = 0;
  for (int i = 0; i < NUMPIXELS; i++) {
    playerSequences1[i] = -1;
    playerSequences2[i] = -1;
  }
  generarColoresAleatorios();
}

void generarColoresAleatorios() {
  for (int i = 0; i < NUMPIXELS; i++) {
    int randomColor = random(4);
    generatedSequence[i] = randomColor;

    switch (randomColor) {
      case 0:
        pixels.setPixelColor(i, pixels.Color(150, 0, 0));
        break;
      case 1:
        pixels.setPixelColor(i, pixels.Color(150, 150, 0));
        break;
      case 2:
        pixels.setPixelColor(i, pixels.Color(0, 150, 0));
        break;
      case 3:
        pixels.setPixelColor(i, pixels.Color(0, 0, 150));
        break;
    }
  }
  pixels.show();

}
