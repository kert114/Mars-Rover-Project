<?php
$servername = "localhost";
$username   = "root";
$password   = "";
$dbname     = "sensordata2";
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
    $trp = mysqli_query($conn, "SELECT * from esp32data");
    $rows = array();
    while($r = mysqli_fetch_assoc($trp)) {
        $rows[] = $r;
    }
    print json_encode($rows); //convert php data to json data
    $json = json_encode($rows);
    //$url1=$_SERVER['http://localhost/query.php'];
    header("Refresh: 2");
    header("Refresh: 2; url=../../../Users/mahas/Documents/GSB2/command/backend/routes/handler.js");
    if (file_put_contents("../../../Users/mahas/Documents/GSB2/command/frontend/src/data.json", $json))
        echo "JSON file created successfully...";
    else 
        echo "Oops! Error creating json file...";

?>