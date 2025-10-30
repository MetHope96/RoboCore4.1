#include "RoboCore41.h" // Magia nera. Non toccare se non sai cosa fai.

// Inizializzazione della scheda. Fa BEEP. Emozionante.
void boardInit() {
  pinMode(BUZZ, OUTPUT); // Dice al pin del buzzer di smettere di essere pigro.
  playStartupMelody();   // Suona qualcosa per dimostrare che è vivo.
}

// Passa roba da una seriale all’altra. Tipo traduttore simultaneo.
void serialPassthrough(Stream &from, Stream &to) {
    while (from.available()) to.write(from.read()); // Sposta byte come se fosse trasloco.
    while (to.available()) from.write(to.read());   // Anche al contrario, perché parità.
}

// === Sezione LED ===

// Accende un LED. Duh.
void ledOn(uint8_t ledPin) {
    digitalWrite(ledPin, HIGH); // Questo lo dovresti sapere.
}

// Spegne un LED. Rivoluzionario.
void ledOff(uint8_t ledPin) {
    digitalWrite(ledPin, LOW);
}

// Fa lampeggiare il LED come se fosse un party.
void ledToggle(uint8_t ledPin) {
    digitalWrite(ledPin, !digitalRead(ledPin)); // Se è acceso, spegnilo. Se è spento, accendilo. Easy.
}

// Blinka il LED ogni tot millisecondi, tipo intermittenza natalizia.
void ledBlink(uint8_t ledPin, uint32_t intervalMs) {
    static unsigned long lastToggleTime = 0; // Quando ha blinkato l’ultima volta?
    static bool state = false;               // LED acceso o spento? Boh.
    unsigned long now = millis();

    if (now - lastToggleTime >= intervalMs) {
        state = !state; // Cambia stato, perché perché no.
        digitalWrite(ledPin, state ? HIGH : LOW);
        lastToggleTime = now; // Ricorda quando hai blinkato. La memoria è importante.
    }
}

// Fa BEEP per un tempo. Non abbastanza per infastidire il vicino.
void buzzBeep(uint8_t buzzerPin, uint16_t durationMs) {
    digitalWrite(buzzerPin, HIGH);
    delay(durationMs);
    digitalWrite(buzzerPin, LOW); // Fine della sinfonia.
}

// Legge un bottone ma con pazienza. Debounce: il batti cinque dell’elettronica.
bool readButtonDebounced(uint8_t pin, uint32_t debounceTimeMs) {
    static uint32_t lastChangeTime = 0;
    static bool lastState = HIGH;
    bool currentState = digitalRead(pin);
    uint32_t now = millis();

    if (currentState != lastState) {
        lastChangeTime = now; // Cambiato! Ma vediamo se è serio...
    }

    if ((now - lastChangeTime) > debounceTimeMs) {
        if (currentState != lastState) {
            lastState = currentState;
            return currentState == LOW; // Premuto o solo timido?
        }
    }
    return false;
}

// Legge la media analogica. Perché una sola lettura è noiosa.
int readAnalogAvg(uint8_t analogPin, uint8_t samples) {
    long sum = 0;
    for (uint8_t i = 0; i < samples; i++) {
        sum += analogRead(analogPin); // Somma tutto, che poi dividiamo.
        delay(5); // Attendi 5ms perché l’universo lo vuole.
    }
    return sum / samples; // Media. Matematica. Wow.
}

// Legge un analogico ma lo fa sembrare serio con offset e scala.
float readAnalogCalibrated(uint8_t analogPin, float offset, float scale) {
    int raw = analogRead(analogPin);
    return (raw + offset) * scale; // Così sembra importante.
}

// Inizializza SPI. Altrimenti fa finta di non funzionare.
#include <SPI.h>
void spiInitDefault() {
    SPI.begin(); // E questo è tutto, SPI parte così.
}

// Dice chi siamo al mondo. Tipo biglietto da visita.
void printBoardInfo(Stream &serial) {
    serial.println(F("RoboCore41 Board"));
    serial.println(F("Firmware version: 1.0")); // Neanche beta, ma già ci piace.
    serial.println(F("Teensy 4.1 based")); // Sì, è un Teensy. No, non è una bibita.
}

// Timer non bloccante. Perché bloccare è da boomer.
bool checkTimeout(unsigned long &lastTime, unsigned long intervalMs) {
    unsigned long now = millis();
    if (now - lastTime >= intervalMs) {
        lastTime = now;
        return true; // È tempo di fare cose.
    }
    return false; // Non ancora. Vai a prendere un caffè.
}

// Log con timestamp. Per sapere quando qualcosa è accaduto, tipo CSI.
void logMessage(Stream &serial, const char *msg) {
    serial.print(millis());
    serial.print(F(" ms: "));
    serial.println(msg);
}

// Suona una melodia startup per farti sentire importante.
void playStartupMelody() {
  auto playNote = [](int freq, int durationMs) {
    if (freq == 0) {
      delay(durationMs); // Silenzio... poetico.
    } else {
      tone(BUZZ, freq, durationMs * 0.9); // Fa la nota, quasi per tutto il tempo.
      delay(durationMs);
      noTone(BUZZ); // Spegne il suono. Educato.
    }
  };

  // Un mucchio di note e durate. Musica, tipo MIDI del 1998.
  int melody[] = {
    494, 16, 988, 16, 740, 16, 622, 16,
    988, 32, 740, -16, 622, 8, 523, 16,
    1047, 16, 1568, 16, 1319, 16, 1047, 32, 1568, -16, 1319, 8,
    494, 16, 988, 16, 740, 16, 622, 16, 988, 32,
    740, -16, 622, 8, 622, 32, 659, 32, 698, 32,
    698, 32, 740, 32, 784, 32, 784, 32, 831, 32, 880, 16, 988, 8
  };

  const int tempo = 105; // Non è Spotify, ma ci si arrangia.
  const int wholenote = (60000 * 4) / tempo; // Matematica per musicisti pigri.
  const int notes = sizeof(melody) / sizeof(int) / 2;

  for (int i = 0; i < notes; i++) {
    int note = melody[i * 2];
    int durationCode = melody[i * 2 + 1];
    int duration;

    if (durationCode > 0) {
      duration = wholenote / durationCode;
    } else {
      duration = (wholenote / abs(durationCode)) * 1.5; // Dura più a lungo. Perché lo dice lui.
    }

    playNote(note, duration); // E suona. La musica è nell’aria.
  }
}
