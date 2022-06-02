<?php
class Database{
    private static $dbName = 'db_sensor';
    private static $dbHost ='192.168.43.247';
    private static $dbUsername = 'root';
    private static $dbUserPassword ='';

    private static $cont = null;

    public function __construct(){
        die('Init function is not allowed');
    }
    public static function connect() {

        if(null==self::$cont){
            try {
                self::$cont = new PDO("mysql:host=".self::$dbHost.";"."dbname".self::$dbName,
                    self::$dbUsername,self::$dbUserPassword);
            }
            catch(PDOException $e){
                die($e->getMessage());
            }
        }
        return self::$cont;
}
public static function disconnect(){
    self::$cont =null;
}

}

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
				<td><?php echo $rows['ensor'];?></td>
				<td><?php echo $rows['ocation'];?></td>
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
