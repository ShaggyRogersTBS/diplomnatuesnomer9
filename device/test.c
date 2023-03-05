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

void loop() {
  float temperature = bmp.readTemperature(); // Read temperature value from the BMP180 sensor
  float pressure = bmp.readPressure() / 100.0F; // Read air pressure value from the BMP180 sensor and convert from Pa to hPa
  float humidity = dht.readHumidity(); // Read air humidity value from the DHT11 sensor

  // Construct the SQL query to insert the temperature, pressure, and humidity values into the database
  String query = "INSERT INTO `weather_data` (`temperature`, `pressure`, `humidity`, `wind_direction`) VALUES ('";
  query += temperature;
  query += "', '";
  query += pressure;
  query += "', '";
  query += humidity;
  query += "', '";
  query += wind_direction;
  query += "')";

  // Execute the SQL query
  MySQL_Cursor* cursor = new MySQL_Cursor(&conn);
  cursor->execute(query);
  delete cursor;

  Serial.println("Data uploaded to MySQL database!");

  delay(600000); // Wait for 10 minutes before uploading the next set of data
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
