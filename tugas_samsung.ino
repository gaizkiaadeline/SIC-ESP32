#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// WiFi credentials
const char *WIFI_SSID = "Kosta Vania 22C";
const char *WIFI_PASS = "KV123123123";

// Pin assignments
#define DHTPIN 27    // DHT22 sensor pin
#define DHTTYPE DHT11  // DHT sensor type
#define MQ135_PIN 34  // MQ-135 analog pin

// Global objects
DHT dht(DHTPIN, DHTTYPE);

// Server settings
const char *SERVER_URL = "http://192.168.0.138/data";  // Adjust with your server's IP address and endpoint

// Function to get temperature data
float get_temperature_data() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println(F("Error reading temperature!"));
    return 0.0;  // Return default value on error
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(t);
    Serial.println(F("°C"));
    return t;
  }
}

// Function to get humidity data
float get_humidity_data() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println(F("Error reading humidity!"));
    return 0.0;  // Return default value on error
  } else {
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.println(F("%"));
    return h;
  }
}

// Function to get MQ-135 gas data
int get_gas_data() {
  int gasValue = analogRead(MQ135_PIN);
  Serial.print(F("Gas value: "));
  Serial.println(gasValue);
  return gasValue;
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");

  dht.begin();
}

void loop() {
  float temperature = get_temperature_data();
  float humidity = get_humidity_data();
  int gasValue = get_gas_data();

  // Prepare JSON payload
  String postData = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + ",\"gas\":" + String(gasValue) + "}";

  // Send HTTP POST request to server
  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(postData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();

  delay(5000);  // Wait for 5 seconds before next reading
}
