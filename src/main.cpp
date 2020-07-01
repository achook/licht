#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "leds.h"

#define PWM_RANGE 255
#define PWM_FREQ 1000

const char* ssid = "WAK 2.4";
const char* password =  "Wietnam_2021";

/*
IPAddress ip(192,168,0,81);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
IPAddress dns_1(192,168,0,99);
IPAddress dns_2(192,168,1,98);
*/

const char* mqtt_server = "mqtt.server";
const int mqtt_port = 1883;
const char* mqtt_device = "device 2";
const char* mqtt_user = "esp8266_2";
const char* mqtt_password = "rCR7cLvKigWivyq6";
const char* mqtt_topic = "device/2/mode";
const char* mqtt_status_topic = "device/2/status";

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

color_t led_color = {255, 255, 255};
uint8_t led_mode = 0;

void callback(char* topic, byte* payload, unsigned int length) {
    if (strcmp(mqtt_topic, topic) != 0) return;
    if (length != 9) return;

    char message[9];
    memcpy(message, payload, 9);

    int op_code;
    long int color_raw;

    sscanf(message, "%x:%lx", &op_code, &color_raw);

    Serial.print("Codes: ");
    Serial.print(op_code);
    Serial.print(":");
    Serial.println(color_raw);

    color_t color;

    color.blue = color_raw & 0b11111111;
    color.green = color_raw >> 8 & 0b11111111;
    color.red = color_raw >> 16 & 0b11111111;

    Serial.print(color.red);
    Serial.print(":");
    Serial.print(color.green);
    Serial.print(":");
    Serial.println(color.blue);

    led_color = color;
    led_mode = op_code;
}

static void make_status_message(char *status) {
    long int color_raw;

    color_raw = led_color.red;
    color_raw <<= 8;
    color_raw |= led_color.green;
    color_raw <<= 8;
    color_raw |= led_color.blue;

    sprintf(status, "%02x:%06lx", led_mode, color_raw);
}

void setup() {
    analogWriteFreq(PWM_FREQ);
    analogWriteRange(PWM_RANGE);

    Serial.begin(9600);
    Serial.println("Serial monitor initialized");
    
    Serial.print("Connecting to network: ");
    Serial.print(ssid);
    // WiFi.config(ip, gateway, subnet, dns_1, dns_2);
    // WiFi.config(ip, gateway, subnet);
    WiFi.hostname("LED");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected with IP adress: ");
    Serial.println(WiFi.localIP());

    Serial.print("Setting MQTT server ");
    Serial.print(mqtt_server);
    Serial.print(" on port ");
    Serial.println(mqtt_port);
    mqtt_client.setServer(mqtt_server, mqtt_port);
    mqtt_client.setCallback(callback);
}

void loop() {
    while (!mqtt_client.connected()) {
        Serial.println("Connecting to MQTT");

        if (mqtt_client.connect(mqtt_device, mqtt_user, mqtt_password)) {
            Serial.print("Connected as "); 
            Serial.print(mqtt_device);
            Serial.print(" as user "); 
            Serial.println(mqtt_user);

            mqtt_client.subscribe(mqtt_topic);
        } else {
            Serial.print("Failed with state: ");
            Serial.println(mqtt_client.state());
            delay(1000);
        }
    }

    switch (led_mode) {
        case 0:
            single_color({0,0,0});
            break;

        case 1:
            single_color(led_color); 
            break;

        case 2:
            blink(led_color);
            break;

        case 3:
            wave();
            break;

        case 4:
            cycle_blink(COLORS, COLORS_LENGTH);
            break;
        
        default:
            led_mode = 0;
            break;
    }

    delay(10);

    char status[10];
    make_status_message(status);
    mqtt_client.publish(mqtt_status_topic, status);

    mqtt_client.loop();
}