const mysql = require('mysql')
const db3 = mysql.createConnection({
host: "localhost",
user: "root",
password: "",
database:"drivingmode" 
})

module.exports = db3;