<?php
// Get the measurements from the query parameters
$windSpeed = $_GET['wind_speed'];
$windDirection = $_GET['wind_direction'];
$temperature = $_GET['temperature'];
$humidity = $_GET['humidity'];

// Database connection configuration
$servername = "localhost";
$username = "root"; // Change to your MySQL username
$password = ""; // Change to your MySQL password
$dbname = "your_database_name"; // Change to your database name

// Create a new connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check the connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Prepare the SQL statement to insert the measurements into the "weather2" table
$sql = "INSERT INTO weather2 (wind_speed, wind_direction, temperature, humidity) VALUES ('$windSpeed', '$windDirection', '$temperature', '$humidity')";

// Execute the SQL statement
if ($conn->query($sql) === TRUE) {
    echo "Measurements saved successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

// Close the database connection
$conn->close();
?>
