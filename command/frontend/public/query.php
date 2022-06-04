
<?php
// PHP code to establish connection
// with the localserver
//$connect = mysqli_connect("192.168.43.247", "root", "", "sensordata", 80 , "");
//mysqli_connect(host, username, password, dbname, port, socket)


//if ($mysqli -> connect_errno) {
  //  echo "Failed to connect to MySQL: " . $mysqli -> connect_error;
   // exit();
 // }
//else{
 //   $sql = "SELECT *FROM sensordata;"
  //  $result = mysqli_query($connect, $sql);
 //   $json_array = array [];
  //  while($row = mysqli_fetch_assoc($result)){
   //     $json_array[] = $row;
 ///   }
  //  echo json_encode($json_array);// to put one row on the web

//}
// Username is root
$user = 'root';
$password = "";

// Database name is gfg
$database = 'sensordata';

// Server is localhost with
// port number 3308
$servername='192.168.158.188';
$mysqli = new mysqli($servername, $user, $password, $database);
//$connect = mysqli_connect("192.168.43.247", "root", "", "sensordata", 80 , "");
//$mysqli = new mysqli("localhost","my_user","my_password","my_db");

// Checking for connections
if ($mysqli->connect_error) {
	die('Connect Error (' .
	$mysqli->connect_errno . ') '.
	$mysqli->connect_error);
}

// SQL query to select data from database
$sql = "SELECT * FROM sensordata  ";
$result = $mysqli->query($sql);
$mysqli->close();
// HTML code to display data in tabular format

?>
<!DOCTYPE html>
<html lang="en">

<head>
	<meta charset="UTF-8">
	<title>GFG User Details</title>
	<!-- CSS FOR STYLING THE PAGE -->
	<style>
		table {
			margin: 0 auto;
			font-size: large;
			border: 1px solid black;
		}

		h1 {
			text-align: center;
			color: #006600;
			font-size: xx-large;
			font-family: 'Gill Sans', 'Gill Sans MT',
			' Calibri', 'Trebuchet MS', 'sans-serif';
		}

		td {
			background-color: #E4F5D4;
			border: 1px solid black;
		}

		th,
		td {
			font-weight: bold;
			border: 1px solid black;
			padding: 10px;
			text-align: center;
		}

		td {
			font-weight: lighter;
		}
	</style>
</head>

<body>
	<section>
		<!-- TABLE CONSTRUCTION-->
		<table>
			<tr>
				<th>ID</th>
				<th>Sensor</th>
				<th>Location</th>
				<th>Value 1</th>
                <th>Value 2</th>
                <th>Value 3</th>
                <th>Timestamp</th>
			</tr>
			<!-- PHP CODE TO FETCH DATA FROM ROWS-->
			<?php // LOOP TILL END OF DATA
				while($rows=$result->fetch_assoc())
				{
			?>
			<tr>
				<!--FETCHING DATA FROM EACH
					ROW OF EVERY COLUMN-->
				<td><?php echo $rows['id'];?></td>
				<td><?php echo $rows['sensor'];?></td>
				<td><?php echo $rows['location'];?></td>
				<td><?php echo $rows['value1'];?></td>
                <td><?php echo $rows['value2'];?></td>
                <td><?php echo $rows['value3'];?></td>
                <td><?php echo $rows['reading_time'];?></td>
			</tr>
			<?php
				}
			?>
		</table>
	</section>
</body>

</html>
