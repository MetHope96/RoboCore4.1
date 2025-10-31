#include <RoboCore41.h>
#include <TimeLib.h>
#include <FlexCAN_T4.h>
#include <SPI.h>
#include <NativeEthernet.h>

const int ledPins[] = { LED1, LED2, LED3, LED4, LED5 };
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;
FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> can2;
CAN_message_t msg;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 27);
EthernetServer server(80);

void ethernetWebTest() {
  EthernetClient client = server.available();
  if (client) {
    String currentLine = "";
    String request = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            if (request.indexOf("GET /on") >= 0) {
              digitalWrite(LED_BUILTIN, HIGH);
            } else if (request.indexOf("GET /off") >= 0) {
              digitalWrite(LED_BUILTIN, LOW);
            }

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<head><title>Teensy Ethernet LED</title></head>");
            client.println("<body>");
            client.println("<h1>Controllo LED Teensy via Ethernet</h1>");
            client.println("<p><a href=\"/on\"><button>ON</button></a></p>");
            client.println("<p><a href=\"/off\"><button>OFF</button></a></p>");
            client.println("</body>");
            client.println("</html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    delay(1);
    client.stop();
  }
}

void testCAN1() {
    static uint32_t lastSend = 0;
    if (millis() - lastSend > 1000) {
        CAN_message_t out;
        out.id = 0x123;
        out.len = 8;
        for (int i = 0; i < 8; i++) out.buf[i] = i;
        can1.write(out);
        Serial.println("CAN1: Messaggio inviato");
        lastSend = millis();
    }

    while (can1.read(msg)) {
        Serial.print("CAN1 RX -> ID: 0x"); Serial.print(msg.id, HEX);
        Serial.print(" Data:");
        for (int i = 0; i < msg.len; i++) {
            Serial.print(" ");
            Serial.print(msg.buf[i], HEX);
        }
        Serial.println();
    }
}

void testCAN2() {
    static uint32_t lastSend = 0;
    if (millis() - lastSend > 1500) {
        CAN_message_t out;
        out.id = 0x456;
        out.len = 8;
        for (int i = 0; i < 8; i++) out.buf[i] = 8 - i;
        can2.write(out);
        Serial.println("CAN2: Messaggio inviato");
        lastSend = millis();
    }

    while (can2.read(msg)) {
        Serial.print("CAN2 RX -> ID: 0x"); Serial.print(msg.id, HEX);
        Serial.print(" Data:");
        for (int i = 0; i < msg.len; i++) {
            Serial.print(" ");
            Serial.print(msg.buf[i], HEX);
        }
        Serial.println();
    }
}

void testTempoRTC() {
    unsigned long start = millis();

    time_t t = now();

    Serial.print("Data/Ora: ");
    Serial.print(day(t)); Serial.print("/");
    Serial.print(month(t)); Serial.print("/");
    Serial.print(year(t)); Serial.print(" ");
    Serial.print(hour(t)); Serial.print(":");
    Serial.print(minute(t)); Serial.print(":");
    Serial.println(second(t));

    unsigned long end = millis();
    Serial.print("Tempo esecuzione stampa RTC: ");
    Serial.print(end - start);
    Serial.println(" ms");
}

void ledTest() {
    for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(300);
    digitalWrite(ledPins[i], LOW);
  }
}

void setup() {
    boardInit();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    for (int i = 0; i < numLeds; i++) {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW);
    }
    Serial.begin(115200);
    Serial1.begin(9600);

    can1.begin();
    can1.setBaudRate(500000);

    can2.begin();
    can2.setBaudRate(500000);

    time_t t = Teensy3Clock.get();
    if (year(t) < 2023) {
        setTime(14, 30, 0, 29, 7, 2025);
        Teensy3Clock.set(now());
        Serial.println("RTC non valido, impostato orario.");
    } else {
        setTime(t);
        Serial.println("RTC valido, orario caricato.");
    }

    Ethernet.begin(mac, ip);
    server.begin();

    Serial.println("RoboCore4.1 ready!");
    printBoardInfo(Serial);
}

void loop() {
    ledTest();
    testTempoRTC();
    testCAN1();
    testCAN2();
    ethernetWebTest();
    serialPassthrough(Serial, Serial1);
    serialPassthrough(Serial1, Serial);
}
