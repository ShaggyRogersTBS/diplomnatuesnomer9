#include <WiFi.h>
#include <HTTPClient.h>
//#include <Wire.h>
#include <time.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;             // I2C

const char* ssid     = "slashcrash";
const char* password = "esp32esp";
const char* serverURL = "http://172.20.10.2/weather.php";
const char* Timezone = "EST-2METDST,M3.5.0/01,M10.5.0/02"; // Most of Europe

String      Format   = "M";       // Time format M for dd-mm-yy and 23:59:59, "I" for mm-dd-yy and 12:59:59 PM, "X" for Metric units but WSpeed in MPH

#define pressure_offset 3.5       // Air pressure calibration, adjust for your altitude
#define WS_Calibration  1.1       // Wind Speed calibration factor
#define WS_Samples      10        // Number of Wind Speed samples for an average
#define WindSensorPin   4       // Only use pins that can support an interrupt
#define DHTPin          23
#define DHTType         DHT11
#define BMP180_SDA_PIN 21
#define BMP180_SCL_PIN 22

DHT dht(DHTPin, DHTType);

const int windDirectionPin = 36; // VP = 36 VN = 39
const int numReadings = 5;       // Number of readings for moving average
int windDirectionReadings[numReadings]; // Array to store wind direction readings
int windDirectionIndex = 0;      // Index of the current reading
float windDirectionTotal = 0;    // Total sum of the readings
static String         Date_str, Time_str;
volatile unsigned int local_Unix_time = 0, next_update_due = 0;
volatile unsigned int update_duration = 60 * 60; // Time duration in seconds, so synchronise every hour
static float          bme_temp, bme_humi, bme_pres, WindSpeed;
static unsigned int   Last_Event_Time;
float WSpeedReadings[WS_Samples]; // To hold readings from the Wind Speed Sensor
int   WS_Samples_Index = 0;       // The index of the current wind speed reading
float WS_Total         = 0;       // The running wind speed total
float WS_Average       = 0;       // The wind speed average
//float dht_temp = dht.readTemperature();
//float dht_humi = dht.readHumidity();

//#########################################################################################
void IRAM_ATTR MeasureWindSpeed_ISR() {
  portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
  portENTER_CRITICAL_ISR(&timerMux);
  Last_Event_Time = millis();    // Record current time for next event calculations
  portEXIT_CRITICAL_ISR(&timerMux);
}
//#########################################################################################
void IRAM_ATTR Timer_TImeout_ISR() {
  portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
  portENTER_CRITICAL_ISR(&timerMux);
  local_Unix_time++;
  portEXIT_CRITICAL_ISR(&timerMux);
}
//#########################################################################################
void setup() {
  Serial.begin(115200);
  //Wire.begin(SDA, SCL);                       
  //Wire.begin();
  //bmp.begin();           // For Adafruit sensors use address 0x77, for most 3rd party types use address 0x76              
  StartWiFi();
  dht.begin();
  Start_Time_Services();
  Setup_Interrupts_and_Initialise_Clock();       // Now setup a timer interrupt to occur every 1-second, to keep seconds accurate
  for (int index = 0; index < WS_Samples; index++) { // Now clear the Wind Speed average array
    WSpeedReadings[index] = 0;
  }
  //if (!bmp.begin(BMP085_STANDARD)) {
  //Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  //while (1) {}
  //}
}
//#########################################################################################
void loop() {
  UpdateLocalTime();
  float windSpeed = Calculate_WindSpeed();
  int windDirection = Calculate_WindDirection();
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print(String(Calculate_WindSpeed(), 1) + (Format == "I" || Format == "M" ? "mph" : "kph")); // Output wind speed in mph (X) or kph (M)
  Serial.print(" ");
  Serial.println(String(Calculate_WindDirection(), 0) + "°"); // Output wind direction in degrees
  Serial.print("DHT Temperature: ");
  Serial.print(temperature);
  Serial.print("°C");
  Serial.print(" DHT Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  //Serial.print("Temperature: ");
  //Serial.print(" °C");
  //Serial.print(" Pressure: ");
  //Serial.println(" hPa");
  Serial.print(Date_str);
  Serial.print(Time_str);

  //QUERY STRING
  String queryString = serverURL;
  queryString += "?wind_speed=" + String (windSpeed, 1);
  queryString += "&wind_direction=" + String(windDirection);
  queryString += "&temperature=" + String(temperature, 2);
  queryString += "&humidity=" + String(humidity, 2);
  queryString += "&date" + Date_str;
  queryString += "&time" + Time_str;

  // SEND REQUEST
  HTTPClient http;
  http.begin(queryString);

  int httpResponseCode = http.GET();
  if (httpResponseCode == HTTP_CODE_OK) {
    Serial.println("HTTP request sent successfully");
  } else {
    Serial.print("HTTP request failed with error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();


  delay(10000);                                                                                                           
}
//#########################################################################################
void UpdateLocalTime() {
  time_t now;
  if (local_Unix_time > next_update_due) { // only get a time synchronisation from the NTP server at the update-time delay set
    time(&now);
    Serial.println("Synchronising local time, time error was: " + String(now - local_Unix_time));
    // If this displays a negative result the interrupt clock is running fast or positive running slow
    local_Unix_time = now;
    next_update_due = local_Unix_time + update_duration;
  } else now = local_Unix_time;
  //See http://www.cplusplus.com/reference/ctime/strftime/
  char hour_output[30], day_output[30];
  if (Format == "M" || Format == "X") {
    strftime(day_output, 30, "%d-%m-%y", localtime(&now)); // Formats date as: 24-05-17
    strftime(hour_output, 30, "%T", localtime(&now));      // Formats time as: 14:05:49
  }
  else {
    strftime(day_output, 30, "%m-%d-%y", localtime(&now)); // Formats date as: 05-24-17
    strftime(hour_output, 30, "%r", localtime(&now));      // Formats time as: 2:05:49pm
  }
  Date_str = day_output;
  Time_str = hour_output;
}
//#########################################################################################
void StartWiFi() {
  /* Set the ESP to be a WiFi-client, otherwise by default, it acts as ss both a client and an access-point
      and can cause network-issues with other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  Serial.print(F("\r\nConnecting to: ")); Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.print("WiFi connected to address: "); Serial.println(WiFi.localIP());
}
//#########################################################################################
void Setup_Interrupts_and_Initialise_Clock() {
  hw_timer_t * timer = NULL;
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &Timer_TImeout_ISR, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
  pinMode(WindSensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(WindSensorPin), &MeasureWindSpeed_ISR, RISING);
  //Now get current Unix time and assign the value to local Unix time counter and start the clock.
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println(F("Failed to obtain time"));
  }
  time_t now;
  time(&now);
  local_Unix_time = now + 1; // The addition of 1 counters the NTP setup time delay
  next_update_due = local_Unix_time + update_duration;
}
//#########################################################################################
void Start_Time_Services() {
  // Now configure time services
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", Timezone, 1); // See below for other time zones
  delay(1000); // Wait for time services
}
//#########################################################################################

float Calculate_WindSpeed() {
  if ((millis() - Last_Event_Time) > 2) { // Ignore short time intervals to debounce switch contacts
    float timeDiff = (millis() - Last_Event_Time) / 1000.00F;
    if (timeDiff != 0) {
      WindSpeed = (1.00F / (timeDiff * 2)) * WS_Calibration; // Calculate wind speed
    }
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
  if (Format == "M") WindSpeed = WindSpeed * 1.60934;                             // Convert to kph if in Metric mode
  return WindSpeed;
}

float Calculate_WindDirection() {
  // Read the raw wind direction value
  int windDirectionRaw = analogRead(windDirectionPin);

  // Subtract the oldest reading from the total
  windDirectionTotal -= windDirectionReadings[windDirectionIndex];

  // Store the new reading in the array
  windDirectionReadings[windDirectionIndex] = windDirectionRaw;

  // Add the new reading to the total
  windDirectionTotal += windDirectionRaw;

  // Increment the index and wrap around if necessary
  windDirectionIndex++;
  if (windDirectionIndex >= numReadings) {
    windDirectionIndex = 0;
  }

  // Calculate the moving average
  int windDirectionAverage = windDirectionTotal / numReadings;

  // Calibration data
  const int windDirectionMin = 0;
  const int windDirectionMax = 3095;
  const int windDirectionRange = 360;

  // Map the averaged wind direction value to the calibrated range
  int calibratedDirection = map(windDirectionAverage, windDirectionMin, windDirectionMax, 0, windDirectionRange);

  // Ensure the wind direction value doesn't exceed 360 degrees
  if (calibratedDirection >= windDirectionRange) {
    calibratedDirection = calibratedDirection % windDirectionRange;
  }

  return calibratedDirection;
}
