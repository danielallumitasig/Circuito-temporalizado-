// Pines según tu esquema en Proteus
const int boton = 5;   // Pulsador con resistencia Pull-Up R1 (10k) en pin 5
const int led = 12;    // LED D1 con resistencia R2 (220 ohm) en pin 12

unsigned long tiempoInicio = 0;
int estado = 0;
int lecturaAnterior = HIGH;
int estadoBoton = HIGH;
unsigned long tiempoRebote = 0;

void setup() {
  pinMode(boton, INPUT);     // Usa la resistencia física R1 de 10k externa
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

void loop() {
  unsigned long ahora = millis();
  int lectura = digitalRead(boton);

  // Filtro antirrebote (40 ms)
  if (lectura != lecturaAnterior) {
    tiempoRebote = ahora;
    lecturaAnterior = lectura;
  }

  if (ahora - tiempoRebote >= 40) {
    if (lectura != estadoBoton) {
      estadoBoton = lectura;
      // En Pull-Up: en reposo lee 5V (HIGH); al presionar lee 0V (LOW)
      if (estadoBoton == LOW && estado == 0) {
        estado = 1;
        tiempoInicio = ahora;
      }
    }
  }

  // Estado 1: Espera 5 segundos antes de encender el LED
  if (estado == 1) {
    if (ahora - tiempoInicio >= 5000) {
      digitalWrite(led, HIGH);
      estado = 2;
      tiempoInicio = ahora;
    }
  }

  // Estado 2: Mantiene encendido el LED durante 10 segundos y luego lo apaga
  if (estado == 2) {
    if (ahora - tiempoInicio >= 10000) {
      digitalWrite(led, LOW);
      estado = 0;
    }
  }
}
