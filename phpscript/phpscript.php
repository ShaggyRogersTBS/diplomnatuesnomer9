<?php

if(isset($_GET["temperature"], $_GET["humidity"], $_GET["wind_speed"], $_GET["pressure"], $_GET["wind_direction"])) {
   $temperature = $_GET["temperature"]; 
   $humidity = $_GET["humidity"];
   $wind_speed = $_GET["wind_speed"];
   $pressure = $_GET["pressure"];
   $wind_direction = $_GET["wind_direction"];

   $servername = "localhost";
   $username = "root";
   $password = "";
   $database_name = "weather_diplomna";

   $connection = new mysqli($servername, $username, $password, $database_name);
   if ($connection->connect_error) {
      die("MySQL connection failed: " . $connection->connect_error);
   }

   $sql = "INSERT INTO weather (temperature, humidity, wind_speed, pressure, wind_direction) 
           VALUES ($temperature, $humidity, $wind_speed, $pressure, $wind_direction)";

   if ($connection->query($sql) === TRUE) {
      echo "New record created successfully";
   } else {
      echo "Error: " . $sql . " => " . $connection->error;
   }

   $connection->close();
} else {
   echo "temperature is not set in the HTTP request";
}
?>