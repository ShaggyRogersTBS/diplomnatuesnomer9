#include <WiFi.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <HTTPClient.h>

#define BMP_SDA 18 // Define the SDA pin for the BMP180 sensor
#define BMP_SCL 19 // Define the SCL pin for the BMP180 sensor
#define DHT_PIN 23 // Define the data pin for the DHT11 sensor

// Replace with your network credentials and MySQL server details
const char* ssid = "slashcrash";
const char* password = "esp32esp";

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

void loop() {
  float temperature = bmp.readTemperature();                       
  float pressure = bmp.readPressure() / 100.0F; 
  float humidity = dht.readHumidity(); 

  String url = "http://78.154.15.171/phpscript.php?";
  url += "temperature=" + String(temperature);
  url += "&pressure=" + String(pressure);
  url += "&humidity=" + String(humidity);
  url += "&wind_direction=" + String(wind_direction);
  url += "&wind_speed=" + String(wind_speed);

  WiFiClient client;
  HTTPClient http;
  http.begin(client, url);  
  int httpCode = http.POST(); 

  if (httpCode > 0) {
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end(); 

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

