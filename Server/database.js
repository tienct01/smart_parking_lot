var mysql = require("mysql");
var connection = mysql.createConnection({
	host: "localhost",
	user: "duytien",
	password: "123456",
	database: "smart_parking",
});

var connect = function () {
	connection.connect(function (err) {
		if (!err) {
			console.log("Database is connected!");
		} else {
			console.log("Database is error!", err);
		}
	});
};

var closeDB = function () {
	connection.end(function (err) {
		if (!err) console.log("Closed database");
	});
};

module.exports = { connection, connect, closeDB };
