#include <WiFi.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <HTTPClient.h>

#define BMP_SDA 18 // SDA пин за BMP180
#define BMP_SCL 19 // SCL пин за BMP180
#define DHT_PIN 23 // номер на пин за инфо на DHT11
const int wind_dir_pin = 36; // номер на пин за посока на вятъра
const char* ssid = "slashcrash"; // име на WiFi мрежа
const char* password = "esp32esp"; // парола на WiFi мрежа
const char* url_base = "http://78.154.15.171/phpscript.php?" //линк към уеб сървър

Adafruit_BMP085 bmp(BMP_SDA, BMP_SCL);
DHT dht(DHT_PIN, DHT11);
WiFiClient client;

// Начало на сесии за WiFI, DHT11, BMP180
void setup() {
  Serial.begin(9600);
  bmp.begin();
  dht.begin();
  connectToWiFi();
  //connectToMySQL();
}

// Първоначално измерване посока на вятъра
float Calculate_WindDirection() {
  int winddirection = analogRead(wind_dir_pin); // VP = 36 VN = 39
  return map(winddirection,0,3095,0,359);
}

// Измерване на BMP и DHT + HTTP заявки към уеб сървър
void loop() {
  float temperature = bmp.readTemperature();                       
  float pressure = bmp.readPressure() / 100.0F; 
  float humidity = dht.readHumidity(); 

  String url = String(url_base);
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

// Свързване към WiFi
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

