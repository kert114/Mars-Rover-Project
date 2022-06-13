const mysql = require('mysql')
const db2 = mysql.createConnection({
host: "localhost",
user: "root",
password: "",
database:"sensordata2" 
})

module.exports = db2;