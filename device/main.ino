#include <WiFi.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <HTTPClient.h>

#define BMP_SDA 18 // SDA пин за BMP180
#define BMP_SCL 19 // SCL пин за BMP180
#define DHT_PIN 23 // номер на пин за инфо на DHT11
#define WS_Calibration  1.1 // Фактор за калибрация на скоростта на вятъра
#define WS_Samples      10  // Номер на измервания на вятъра за средна скорост
#define WindSensorPin   15  // Само пинове, които поддържат interrupt
const int wind_dir_pin = 36; // номер на пин за посока на вятъра
const char* ssid = "slashcrash"; // име на WiFi мрежа
const char* password = "esp32esp"; // парола на WiFi мрежа
const char* url_base = "http://78.154.15.171/phpscript.php?"; //линк към уеб сървър

Adafruit_BMP085 bmp;
DHT dht(DHT_PIN, DHT11);
WiFiClient client;

// DECLARE
static unsigned int   Last_Event_Time;
static float WindSpeed;
float wind_direction;
float wind_speed;
float WSpeedReadings[WS_Samples]; // To hold readings from the Wind Speed Sensor
int   WS_Samples_Index = 0;       // The index of the current wind speed reading
float WS_Total         = 0;       // The running wind speed total
float WS_Average       = 0;       // The wind speed average

void IRAM_ATTR MeasureWindSpeed_ISR() {
  portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
  portENTER_CRITICAL_ISR(&timerMux);
  Last_Event_Time = millis();    // Record current time for next event calculations
  portEXIT_CRITICAL_ISR(&timerMux);
}

// Начало на сесии за WiFI, DHT11, BMP180
void setup() {
  Serial.begin(9600);
  Wire.begin(BMP_SDA, BMP_SCL);
  bmp.begin();
  dht.begin();
  connectToWiFi();
  for (int index = 0; index < WS_Samples; index++) { // Now clear the Wind Speed average array
    WSpeedReadings[index] = 0;
  }
  //connectToMySQL();
}

// Първоначално измерване посока на вятъра
float Calculate_WindDirection() {
  int winddirection = analogRead(wind_dir_pin); // VP = 36 VN = 39
  return map(winddirection,0,3095,0,359);
}

// Първоначално измерване на скорост на вятъра
float Calculate_WindSpeed() {
  if ((millis() - Last_Event_Time) > 2) { // Ignore short time intervals to debounce switch contacts
    WindSpeed = (1.00F / (((millis() - Last_Event_Time) / 1000.00F) * 2)) * WS_Calibration; // Calculate wind speed
  }
  // Calculate average wind speed
  WS_Total                         = WS_Total - WSpeedReadings[WS_Samples_Index]; // Subtract the last reading:
  WSpeedReadings[WS_Samples_Index] = WindSpeed;                                   // Add the reading to the total:
  WS_Total                         = WS_Total + WSpeedReadings[WS_Samples_Index]; // Advance to the next position in the array:
  WS_Samples_Index                 = WS_Samples_Index + 1;                        // If we're at the end of the array...
  if (WS_Samples_Index >= WS_Samples) {                                           // ...wrap around to the beginning:
    WS_Samples_Index = 0;
  }
  WindSpeed = WS_Total / WS_Samples;                                              // calculate the average wind speed:
  WindSpeed = WindSpeed * 1.60934;                             // Convert to kph if in Metric mode
  return WindSpeed;
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
  url += "&wind_speed=" + String(WindSpeed);

  WiFiClient client;
  HTTPClient http;
  http.begin(client, url);  
  int httpCode = http.POST(url); 

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
