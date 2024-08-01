#define BLYNK_PRINT Serial

#define PUMP D5

#define BLYNK_TEMPLATE_ID "TMPLJIGihkM6"

#define BLYNK_DEVICE_NAME "SPM"

#include <OneWire.h>
#include <SPI.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D2

OneWire oneWire(ONE_WIRE_BUS); // Fixed typo: OneWire
DallasTemperature sensors(&oneWire);

char auth[] = "Oh9CdXv9VdSpN2fVm62pnPYOg3KLA-qs";
char ssid[] = "Livin's Galaxy";
char pass[] = "jehovahjireh";

WidgetLED ledl(V7); // Fixed typo: WidgetLED

#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE); // Fixed typo: DHT

SimpleTimer timer;

void sendSensor() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Blynk.virtualWrite(V5, h);
    Blynk.virtualWrite(V6, t);
}

void blinkLedWidget() { // Fixed function name and added braces
    if (ledl.getValue()) {
        ledl.off();
        Serial.println("MOTOR on D5: off");
        digitalWrite(D5, LOW);
    } else {
        ledl.on();
        Serial.println("MOTOR on D5: on");
        digitalWrite(D5, HIGH);
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(D5, OUTPUT);
    pinMode(D4, INPUT);

    dht.begin();
    timer.setInterval(1000L, sendSensor);
    Blynk.begin(auth, ssid, pass);
    timer.setInterval(1000L, blinkLedWidget); // Fixed function name

    sensors.begin();
}

int sensor = 0;
int output = 0;

void sendTemps() {
    sensor = analogRead(A0);
    output = map(sensor, 0, 1023, 0, 100); // Fixed map function

    delay(1000);

    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);

    Serial.println(temp);
    Serial.print("moisture = ");
    Serial.print(output);
    Serial.println("%");

    Blynk.virtualWrite(V1, temp); // Fixed V1
    Blynk.virtualWrite(V2, output);
    
    delay(1000);
}

void loop() {
    if (output <= 40) {
        digitalWrite(D5, HIGH);
    } else if (output > 40) {
        digitalWrite(D5, LOW);
    }

    Blynk.run();
    timer.run();
    sendTemps();
}
