#include <Adafruit_NeoPixel.h>

// Pin de strip de leds
#define PIN 5

// Cantidad de leds
#define NUMPIXELS 3

// Seteo de NeoPixels
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pixels.begin();  // Inicia NeoPixel
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');  // Lee una línea del puerto serie

    protocolo(input);
  }
}

void protocolo(String txt) {
  // Crear una copia mutable de la cadena txt
  char inputBuffer[64]; // Ajusta el tamaño según tus necesidades
  txt.toCharArray(inputBuffer, sizeof(inputBuffer));

  // Divide la cadena en instrucciones separadas por comas
  char *token = strtok(inputBuffer, ",");

  while (token != NULL) {
    // Procesa cada instrucción
    int letraPos = encontrarLetra(token[0]);
    char R = token[1];
    char G = token[2];
    char B = token[3];
    int transformedR = 0;
    int transformedG = 0;
    int transformedB = 0;

    if (R == '1') {
      transformedR = 150;
    }
    if (G == '1') {
      transformedG = 150;
    }
    if (B == '1') {
      transformedB = 150;
    }

    Serial.println("Protocolo:");
    Serial.println(letraPos);
    Serial.println(transformedR);
    Serial.println(transformedG);
    Serial.println(transformedB);
    prenderLed(letraPos, transformedR, transformedG, transformedB);
    
    // Avanza al siguiente token
    token = strtok(NULL, ",");
  }
}

void prenderLed(int i, int R, int G, int B) {
  pixels.setPixelColor(i, pixels.Color(R, G, B));
  pixels.show();  // Prende
}

int encontrarLetra(char letra) {
  char abecedario[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  for (int i = 0; i < sizeof(abecedario); i++) {
    if (abecedario[i] == letra) {
      return i;  // Si se encuentra la letra, retorna la posición.
    }
  }

  // Si el bucle termina y no se encuentra la letra, retorna -1.
  return -1;
}
