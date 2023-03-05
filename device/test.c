#include <WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <DHT.h>

#define DHTPIN 23 // Define the pin to which the DHT11 sensor is connected
#define DHTTYPE DHT11 // Define the type of DHT sensor

// Replace with your network credentials and MySQL server details
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* server = "your_SERVER_IP_ADDRESS";
const char* user = "your_USERNAME";
const char* passwordDB = "your_PASSWORD";
const char* database = "weather_diplomna";

// Create an instance of the DHT sensor and WiFiClient object
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(9600);
  dht.begin();
  connectToWiFi();
  connectToMySQL();
}

void loop() {
  float temperature = dht.readTemperature(); // Read temperature value from the DHT11 sensor
  float humidity = dht.readHumidity(); // Read humidity value from the DHT11 sensor
  
  if (isnan(temperature) || isnan(humidity)) { // Check if any value is NaN
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Construct the SQL query to insert the temperature and humidity values into the database
  String query = "INSERT INTO `weather_data` (`temperature`, `humidity`) VALUES ('";
  query += temperature;
  query += "', '";
  query += humidity;
  query += "')";

  // Execute the SQL query
  MySQL_Cursor* cursor = new MySQL_Cursor(&client);
  cursor->execute(query);
  delete cursor;

  Serial.println("Data uploaded to MySQL database!");
  
  delay(30000); // Wait for 30 seconds before uploading the next set of data
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
