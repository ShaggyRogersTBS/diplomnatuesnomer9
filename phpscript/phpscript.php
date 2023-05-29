<?php
// Get the measurements from the query parameters
$windSpeed = $_GET['wind_speed'];
$windDirection = $_GET['wind_direction'];
$temperature = $_GET['temperature'];
$humidity = $_GET['humidity'];
$date = date('d-m-y');  // Промени формата на датата на "d-m-y"
$time = date('H:i');   // Промени формата на часа на "H:i"

// Database connection configuration
$servername = "localhost";
$username = "root"; // Промени съответно за вашия MySQL потребител
$password = ""; // Промени съответно за вашия MySQL парола
$dbname = "your_database_name"; // Промени съответно за името на вашата база данни

// Създаване на нова връзка
$conn = new mysqli($servername, $username, $password, $dbname);

// Проверка на връзката
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Подготовка на SQL заявката за вмъкване на измерванията в таблицата "weather2"
$sql = "INSERT INTO weather2 (wind_speed, wind_direction, temperature, humidity, date, time) VALUES ('$windSpeed', '$windDirection', '$temperature', '$humidity', '$date', '$time')";

// Изпълнение на SQL заявката
if ($conn->query($sql) === TRUE) {
    echo "Measurements saved successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

// Затваряне на връзката с базата данни
$conn->close();
?>
