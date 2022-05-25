<?php
include 'connect.php';

if(ifset($_POST)){
    $sensor_type=$_POST["sensor_recived"];
    $sensor_value=$_POST["sensor_recived"];

    $pdo = Database::connect();
    $pdo->setAttribute(PDO::ATTR_ERRMODE,PDO::ERRMODE_EXCEPTION);
    $Ssql = "INSERT INTO research(esnor,value1,access_time)
                            VALUES('$sensor_type', '$sensor_value','')";
    $q=$pdo->prepare($sql);
    $q->execute(array());
    Database::disconnect();
    echo "Insert Success";                    
        
}
?>