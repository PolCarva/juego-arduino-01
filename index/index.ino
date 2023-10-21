#include <Adafruit_NeoPixel.h>

#define PIN 5
#define PIN_USER_1 4
#define PIN_USER_2 3
#define NUMPIXELS 8

#define BUTTONS_PER_TEAM 4

// Crea el objeto para la primera tira de LED en PIN_USER_1
Adafruit_NeoPixel strip1(NUMPIXELS, PIN_USER_1, NEO_GRB + NEO_KHZ800);

// Crea el objeto para la segunda tira de LED en PIN_USER_2
Adafruit_NeoPixel strip2(NUMPIXELS, PIN_USER_2, NEO_GRB + NEO_KHZ800);

// Crea el objeto para la tira de LED original en PIN
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int generatedSequence[NUMPIXELS];

int buttonPins1[BUTTONS_PER_TEAM] = { 13, 12, 11, 10 };
int buttonPins2[BUTTONS_PER_TEAM] = { 9, 8, 7, 6 };

int currentButtonIndex1 = 0;
int currentButtonIndex2 = 0;

int currentButtonIndex1Reverse = NUMPIXELS - 1;
int currentButtonIndex2Reverse = NUMPIXELS - 1;

bool gameFinished = false;  // Controlar si el juego ha finalizado
String winningTeam;         // Almacena el nombre del equipo ganador

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
  while (!gameFinished) {
    checkTeam(buttonPins1, currentButtonIndex1, currentButtonIndex1Reverse, "Equipo 1");
    checkTeam(buttonPins2, currentButtonIndex2, currentButtonIndex2Reverse, "Equipo 2");
  }

  Serial.println(winningTeam + " GANÓ!");
  resetGame();
}

void checkTeam(int buttonPins[], int &currentButtonIndex, int &currentButtonIndexReverse, String teamName) {
  updateTeam(buttonPins, currentButtonIndex, currentButtonIndexReverse, teamName);

  if (completedSequence(currentButtonIndex, currentButtonIndexReverse)) {
    if (!gameFinished) {  // Verifica si el juego ya fue ganado por el otro equipo
      gameFinished = true;
      winningTeam = teamName;
    }
  }

  // Comprueba si ambas secuencias se completaron incorrectamente
  if (currentButtonIndex >= NUMPIXELS && currentButtonIndexReverse <= -1) {
    // Ambas secuencias se completaron incorrectamente, apaga la tira del jugador
    turnOffPlayerStrip(teamName);
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

void updateTeam(int buttonPins[], int &currentButtonIndex, int &currentButtonIndexReverse, String teamName) {
  int buttonPressed = getButtonPressed(buttonPins);

  if (buttonPressed != -1) {

    if (currentButtonIndex < NUMPIXELS && buttonPressed == generatedSequence[currentButtonIndex]) {
      currentButtonIndex++;
    } else {
      // La secuencia ingresada no es correcta.
      currentButtonIndex = 0;
    }
    // Secuencia Reverse
    if (currentButtonIndexReverse >= 0 && buttonPressed == generatedSequence[currentButtonIndexReverse]) {
      currentButtonIndexReverse--;
    } else {
      currentButtonIndexReverse = NUMPIXELS - 1;
    }

    if (currentButtonIndex < (NUMPIXELS - 1 - currentButtonIndexReverse)) {
      // Muestro en la tira del jugador la secuencia en orden REVERSA
      //Si cambió de Mostrar normal a Mostrar alrevez, borrar las luces que no deberian mostrarse
      turnOffExcess(teamName, currentButtonIndexReverse, true);


    } else {
      // Muestro la normal
      //Si cambió de Mostrar alrevez a Mostrar normal, borrar las luces que no deberian mostrarse

      turnOffExcess(teamName, currentButtonIndex, false);
    }

    delay(300);  // Debounce
  }
}

bool completedSequence(int &currentButtonIndex, int &currentButtonIndexReverse) {
  return currentButtonIndex >= NUMPIXELS || currentButtonIndexReverse <= -1;
}

void resetGame() {
  gameFinished = false;
  currentButtonIndex1 = 0;
  currentButtonIndex2 = 0;
  currentButtonIndex1Reverse = NUMPIXELS - 1;
  currentButtonIndex2Reverse = NUMPIXELS - 1;

  for (int i = 0; i < NUMPIXELS; i++) {
    strip1.setPixelColor(i, strip1.Color(0, 0, 0));
    strip2.setPixelColor(i, strip2.Color(0, 0, 0));
  }

  strip1.show();
  strip2.show();

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

void turnOnPlayerStrip(String teamName, int index, int color) {
  int R = 0;
  int G = 0;
  int B = 0;

  switch (color) {
    case 0:
      R = 150;
      G = 0;
      B = 0;
      break;
    case 1:
      R = 150;
      G = 150;
      B = 0;
      break;
    case 2:
      R = 0;
      G = 150;
      B = 0;
      break;
    case 3:
      R = 0;
      G = 0;
      B = 150;
      break;
  }

  if (teamName == "Equipo 1") {
    strip1.setPixelColor(index, pixels.Color(R, G, B));
    strip1.show();
  } else {
    strip2.setPixelColor(index, pixels.Color(R, G, B));
    strip2.show();
  }
}


void turnOffPlayerStrip(String teamName) {
  if (teamName == "Equipo 1") {
    for (int i = 0; i < NUMPIXELS; i++) {
      strip1.setPixelColor(i, strip1.Color(0, 0, 0));
    }
    strip1.show();
  } else {
    for (int i = 0; i < NUMPIXELS; i++) {
      strip2.setPixelColor(i, strip2.Color(0, 0, 0));
    }
    strip2.show();
  }
}

void turnOffExcess(String teamName, int index, bool isReverse) {
  turnOffPlayerStrip(teamName);

  if (isReverse) {
    //Recorro desde atras el main strip i veces y prendo esos colores
    for (int i = NUMPIXELS - 1; i > index; i--) {
      turnOnPlayerStrip(teamName, i, generatedSequence[i]);
    }

  } else {
    //Recorro el main strip hasta i y prendo esos colores
    for (int i = 0; i < index; i++) {
      turnOnPlayerStrip(teamName, i, generatedSequence[i]);
    }
  }
