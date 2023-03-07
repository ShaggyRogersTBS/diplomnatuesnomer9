#include <WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>

#define BMP_SDA 18 // Define the SDA pin for the BMP180 sensor
#define BMP_SCL 19 // Define the SCL pin for the BMP180 sensor
#define DHT_PIN 23 // Define the data pin for the DHT11 sensor

// Replace with your network credentials and MySQL server details
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* server = "your_SERVER_IP_ADDRESS";
const char* user = "your_USERNAME";
const char* passwordDB = "your_PASSWORD";
const char* database = "weather_diplomna";

// Create instances of the BMP180 sensor, DHT11 sensor, and WiFiClient object
Adafruit_BMP085 bmp(BMP_SDA, BMP_SCL);
DHT dht(DHT_PIN, DHT11);
WiFiClient client;

void setup() {
  Serial.begin(9600);
  bmp.begin();
  dht.begin();
  connectToWiFi();
  connectToMySQL();
}

float Calculate_WindDirection() {
  int winddirection = analogRead(36); // VP = 36 VN = 39
  return map(winddirection,0,3095,0,359);
}

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

void loop() {
  float temperature = bmp.readTemperature();                        // Read temperature value from the BMP180 sensor
  float pressure = bmp.readPressure() / 100.0F; // Read air pressure value from the BMP180 sensor and convert from Pa to hPa
  float humidity = dht.readHumidity(); // Read air humidity value from the DHT11 sensor

  String url = "http://example.com/weather_data.php?";
  url += "temperature=" + String(temperature);
  url += "&pressure=" + String(pressure);
  url += "&humidity=" + String(humidity);
  url += "&wind_direction=" + String(wind_direction);
  url += "&wind_speed=" + String(wind_speed);

  WiFiClient client;
  HTTPClient http;
  http.begin(client, url);  // Specify request destination
  int httpCode = http.GET(); // Send the HTTP request

  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end(); // Close HTTP connection

  Serial.println("Data uploaded to server!");

  delay(600000);
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectToMySQL() {
  Serial.print("Connecting to MySQL server...");
  if (client.connect(server, 3306, user, passwordDB, database)) {
    Serial.println("MySQL server connected!");
  } else {
    Serial.println("MySQL server connection failed!");
  }
}
