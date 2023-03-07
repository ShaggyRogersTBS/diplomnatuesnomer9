<!DOCTYPE html>
<html>
<head>
    <title>Weather Data</title>
    <link rel="stylesheet" href="styles2.css">
    <script type="text/javascript" src="sidemenu.js"></script>
    <script src="https://d3js.org/d3.v6.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/mysql@2.0.0/index.js"></script>
    <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.1/jquery.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/js/bootstrap.min.js"></script>
    <script src="main.js"></script>
</head>
<body>
    <div class="data-table">
        <h1 class = "tableone-text">Станция №1</h1>
        <table>
            <tr>
                <th>Air Pressure (hPa)</th>
                <th>Humidity (g.m-3)</th>
                <th>Temperature (°C)</th>
                <th>Wind Speed (km/h)</th>
                <th>Wind Direction (°)</th>
            </tr>
            <?php
                $server = "localhost";
                $username = "root";
                $password = "";
                $database = "weather_diplomna";
                // Create connection
                $conn = mysqli_connect($server, $username, $password, $database);
                // Check connection
                if (!$conn) {
                    die("Connection failed: " . mysqli_connect_error());
                }
                $sql = "SELECT pressure, humidity, temperature, wind_speed, wind_direction FROM weather";
                $result = mysqli_query($conn, $sql);
                if (mysqli_num_rows($result) > 0) {
                    // Output data of each row
                    while($row = mysqli_fetch_assoc($result)) {
                        echo "<tr>";
                        echo "<td>" . $row["pressure"] . "</td>";
                        echo "<td>" . $row["humidity"] . "</td>";
                        echo "<td>" . $row["temperature"] . "</td>";
                        echo "<td>" . $row["wind_speed"] . "</td>";
                        echo "<td>" . $row["wind_direction"] . "</td>";
                        echo "</tr>";
                    }
                }
                mysqli_close($conn);
            ?>
        </table>

    </div>

    <div class ="minmaxtemp">

    </div>
    <div class="data-table2">
        <h1 class = "tabletwo-text">Станция №2</h1>
        <table>

            <tr>
                <th>Air Pressure (hPa)</th>
                <th>Humidity (g.m-3)</th>
                <th>Temperature (°C)</th>
                <th>Wind Speed (km/h)</th>
                <th>Wind Direction (°)</th>
            </tr>
            <?php
                $server = "localhost";
                $username = "root";
                $password = "";
                $database = "weather_diplomna";
                // Create connection
                $conn = mysqli_connect($server, $username, $password, $database);
                // Check connection
                if (!$conn) {
                    die("Connection failed: " . mysqli_connect_error());
                }
                $sql = "SELECT pressure, humidity, temperature, wind_speed, wind_direction FROM weather2";
                $result = mysqli_query($conn, $sql);
                if (mysqli_num_rows($result) > 0) {
                    // Output data of each row
                    while($row = mysqli_fetch_assoc($result)) {
                        echo "<tr>";
                        echo "<td>" . $row["pressure"] . "</td>";
                        echo "<td>" . $row["humidity"] . "</td>";
                        echo "<td>" . $row["temperature"] . "</td>";
                        echo "<td>" . $row["wind_speed"] . "</td>";
                        echo "<td>" . $row["wind_direction"] . "</td>";
                        echo "</tr>";
                    }
                }
                mysqli_close($conn);
            ?>
        </table>

    </div>
    <div class="nav">
        <a href='index.php'>HOME</a>
        <a class="active" href='weather-data.php'>WEATHER INFO</a>
        <a href="http://localhost/phpmyadmin">ADMIN INFO</a>
        <a href="#about">ABOUT</a>
    </div>
    <div class="graph" id="graph">
            <h1>Температура</h1>
            <?php
            $servername = "localhost";
            $username = "root";
            $password = "";
            $dbname = "weather_diplomna";

        // Create connection
            $conn = mysqli_connect($servername, $username, $password, $dbname);

        // Check connection
            if (!$conn) {
                die("Connection failed: " . mysqli_connect_error());
            }

            $sql = "SELECT MIN(temperature) as min_temp, MAX(temperature) as max_temp, AVG(temperature) as avg_temp FROM weather";
            $result = mysqli_query($conn, $sql);

            if (mysqli_num_rows($result) > 0) {
            // Output data of each row
                while($row = mysqli_fetch_assoc($result)) {
                    $avg_temp = number_format((float)$row['avg_temp'], 2, '.', '');
                    echo "Средната температура за последните 24 часа е " . $avg_temp . "°C." ."<br>";
                    echo "Най-ниската температура за последните 24 часа е " . $row["min_temp"] . "°C." ."<br>";
                    echo "Най-високата температура за последните 24 часа е " . $row["max_temp"] . "°C." ."<br>";
                }
            } else {
                echo "0 results";
            }

            mysqli_close($conn);
            ?>
    </div>

    <div class="graph" id="graphprec" style="display:none">
            <h1>Налягане на въздуха</h1>
            <?php
            $servername = "localhost";
            $username = "root";
            $password = "";
            $dbname = "weather_diplomna";

        // Create connection
            $conn = mysqli_connect($servername, $username, $password, $dbname);

        // Check connection
            if (!$conn) {
                die("Connection failed: " . mysqli_connect_error());
            }

            $sql = "SELECT MIN(pressure) as min_prec, MAX(pressure) as max_prec, AVG(pressure) as avg_prec FROM weather";
            $result = mysqli_query($conn, $sql);

            if (mysqli_num_rows($result) > 0) {
            // Output data of each row
                while($row = mysqli_fetch_assoc($result)) {
                    $avg_prec = number_format((float)$row['avg_prec'], 2, '.', '');
                    echo "Средното налягане на въздуха за последните 24 часа е " . $avg_prec . "hPa." . "<br>";
                    echo "Най-ниското налягане на въздуха за последните 24 часа е " . $row["min_prec"] . "hPa." ."<br>";
                    echo "Най-високото налягане на въздуха за последните 24 часа е " . $row["max_prec"] . "hPa." ."<br>";
                }
            } else {
                echo "0 results";
            }

            mysqli_close($conn);
            ?>
    </div>
    <div class="graph" id="graphhum" style="display:none">
            <h1>Влажност на въздуха</h1>
            <?php
            $servername = "localhost";
            $username = "root";
            $password = "";
            $dbname = "weather_diplomna";

        // Create connection
            $conn = mysqli_connect($servername, $username, $password, $dbname);

        // Check connection
            if (!$conn) {
                die("Connection failed: " . mysqli_connect_error());
            }

            $sql = "SELECT MIN(humidity) as min_hum, MAX(humidity) as max_hum, AVG(humidity) as avg_hum FROM weather";
            $result = mysqli_query($conn, $sql);

            if (mysqli_num_rows($result) > 0) {
            // Output data of each row
                while($row = mysqli_fetch_assoc($result)) {
                    $avg_hum = number_format((float)$row['avg_hum'], 2, '.', '');
                    echo "Средната влажност на въздуха за последните 24 часа е " . $avg_hum . "g.m-3." . "<br>";
                    echo "Най-ниската влажност на въздуха за последните 24 часа е " . $row["min_hum"] . "g.m-3." ."<br>";
                    echo "Най-високата влажност на въздуха за последните 24 часа е " . $row["max_hum"] . "g.m-3." ."<br>";
                }
            } else {
                echo "0 results";
            }

            mysqli_close($conn);
            ?>
    </div>
    <div class="graph" id="graphwinds" style="display:none">
            <h1>Скорост на вятъра</h1>
            <?php
            $servername = "localhost";
            $username = "root";
            $password = "";
            $dbname = "weather_diplomna";

        // Create connection
            $conn = mysqli_connect($servername, $username, $password, $dbname);

        // Check connection
            if (!$conn) {
                die("Connection failed: " . mysqli_connect_error());
            }

            $sql = "SELECT MIN(wind_speed) as min_winds, MAX(wind_speed) as max_winds, AVG(wind_speed) as avg_winds FROM weather";
            $result = mysqli_query($conn, $sql);

            if (mysqli_num_rows($result) > 0) {
            // Output data of each row
                while($row = mysqli_fetch_assoc($result)) {
                    $avg_winds = number_format((float)$row['avg_winds'], 2, '.', '');
                    echo "Средната скорост на вятъра за последните 24 часа е " . $avg_winds . "km/h." . "<br>";
                    echo "Най-ниската скорост на вятъра за последните 24 часа е " . $row["min_winds"] . "km/h." ."<br>";
                    echo "Най-високата скорост на вятъра за последните 24 часа е " . $row["max_winds"] . "km/h." ."<br>";
                }
            } else {
                echo "0 results";
            }

            mysqli_close($conn);
            ?>
    </div>
    <div class="graph2" id="graph2">
            <h1>Температура</h1>
            <?php
            $servername = "localhost";
            $username = "root";
            $password = "";
            $dbname = "weather_diplomna";

        // Create connection
            $conn = mysqli_connect($servername, $username, $password, $dbname);

        // Check connection
            if (!$conn) {
                die("Connection failed: " . mysqli_connect_error());
            }

            $sql = "SELECT MIN(temperature) as min_temp, MAX(temperature) as max_temp, AVG(temperature) as avg_temp FROM weather2";
            $result = mysqli_query($conn, $sql);

            if (mysqli_num_rows($result) > 0) {
            // Output data of each row
                while($row = mysqli_fetch_assoc($result)) {
                    $avg_temp = number_format((float)$row['avg_temp'], 2, '.', '');
                    echo "Средната температура за последните 24 часа е " . $avg_temp . "°C." . "<br>";
                    echo "Най-ниската температура за последните 24 часа е " . $row["min_temp"] . "°C." ."<br>";
                    echo "Най-високата температура за последните 24 часа е " . $row["max_temp"] . '°C.' ."<br>";
                }
            } else {
                echo "0 results";
            }

            mysqli_close($conn);
            ?>
    </div>
    <div class="graph2" id="graphprec2" style="display:none">
            <h1>Налягане</h1>
            <?php
            $servername = "localhost";
            $username = "root";
            $password = "";
            $dbname = "weather_diplomna";

        // Create connection
            $conn = mysqli_connect($servername, $username, $password, $dbname);

        // Check connection
            if (!$conn) {
                die("Connection failed: " . mysqli_connect_error());
            }

            $sql = "SELECT MIN(pressure) as min_prec, MAX(pressure) as max_prec, AVG(pressure) as avg_prec FROM weather2";
            $result = mysqli_query($conn, $sql);

            if (mysqli_num_rows($result) > 0) {
            // Output data of each row
                while($row = mysqli_fetch_assoc($result)) {
                    $avg_prec = number_format((float)$row['avg_prec'], 2, '.', '');
                    echo "Средното налягане за последните 24 часа е " . $avg_prec . "hPa." ."<br>";
                    echo "Най-ниското налягане за последните 24 часа е " . $row["min_prec"] . "hPa." ."<br>";
                    echo "Най-високото налягане за последните 24 часа е " . $row["max_prec"] . "hPa." ."<br>";
                }
            } else {
                echo "0 results";
            }

            mysqli_close($conn);
            ?>
    </div>
    <div class="graph2" id="graphhum2" style="display:none">
            <h1>Влажност на въздуха</h1>
            <?php
            $servername = "localhost";
            $username = "root";
            $password = "";
            $dbname = "weather_diplomna";

        // Create connection
            $conn = mysqli_connect($servername, $username, $password, $dbname);

        // Check connection
            if (!$conn) {
                die("Connection failed: " . mysqli_connect_error());
            }

            $sql = "SELECT MIN(humidity) as min_hum, MAX(humidity) as max_hum, AVG(humidity) as avg_hum FROM weather2";
            $result = mysqli_query($conn, $sql);

            if (mysqli_num_rows($result) > 0) {
            // Output data of each row
                while($row = mysqli_fetch_assoc($result)) {
                    $avg_hum = number_format((float)$row['avg_hum'], 2, '.', '');
                    echo "Средната влажност на въздуха за последните 24 часа е " . $avg_hum . "g.m-3" ."<br>";
                    echo "Най-ниската влажност на въздуха за последните 24 часа е " . $row["min_hum"] . "g.m-3." ."<br>";
                    echo "Най-високата влажност на въздуха за последните 24 часа е " . $row["max_hum"] . "g.m-3." ."<br>";
                }
            } else {
                echo "0 results";
            }

            mysqli_close($conn);
            ?>
    </div>
    <div class="graph2" id="graphwinds2" style="display:none">
            <h1>Скорост на вятъра</h1>
            <?php
            $servername = "localhost";
            $username = "root";
            $password = "";
            $dbname = "weather_diplomna";

        // Create connection
            $conn = mysqli_connect($servername, $username, $password, $dbname);

        // Check connection
            if (!$conn) {
                die("Connection failed: " . mysqli_connect_error());
            }

            $sql = "SELECT MIN(wind_speed) as min_winds, MAX(wind_speed) as max_winds, AVG(wind_speed) as avg_winds FROM weather2";
            $result = mysqli_query($conn, $sql);

            if (mysqli_num_rows($result) > 0) {
            // Output data of each row
                while($row = mysqli_fetch_assoc($result)) {
                    $avg_winds = number_format((float)$row['avg_winds'], 2, '.', '');
                    echo "Средната скорост на вятъра за последните 24 часа е ". $avg_winds . "km/h." ."<br>";
                    echo "Най-ниската скорост на вятъра за последните 24 часа е " . $row["min_winds"] . "km/h." ."<br>";
                    echo "Най-високата скорост на вятъра за последните 24 часа е " . $row["max_winds"] . "km/h." ."<br>";
                }
            } else {
                echo "0 results";
            }

            mysqli_close($conn);
            ?>
    </div>
    <div class="dropdown">
        <button class="dropbtn">Станция №1</button>
        <div class="dropdown-content">
        <a href="#graph" onclick="showData('graph')">Температура</a>
        <a href="#graphprec" onclick="showData('graphprec')">Налягане</a>
        <a href="#graphhum" onclick="showData('graphhum')">Влажност на въздуха</a>
        <a href="#graphwinds" onclick="showData('graphwinds')">Скорост на вятъра</a>
    </div>
    </div>
    <div class="dropdown2">
        <button class="dropbtn2">Станция №2</button>
        <div class="dropdown-content2">
        <a href="#graph2" onclick="showData2('graph2')">Температура</a>
        <a href="#graphprec2" onclick="showData2('graphprec2')">Налягане</a>
        <a href="#graphhum2" onclick="showData2('graphhum2')">Влажност на въздуха</a>
        <a href="#graphwinds2" onclick="showData2('graphwinds2')">Скорост на вятъра</a>
    </div>
    </div>
</html>
