var fs = require("fs");
const express = require("express");
const app = express();
var url = require("url");
var http = require("http");
var WebSocket = require("ws");
const { connect, connection, closeDB } = require("./database");

const constant = {
	BR_OFF: "BR_OFF",
	BR_ON: "BR_ON",
};
// function gửi yêu cầu(response) từ phía server hoặc nhận yêu cầu (request) của client gửi lên
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(express.static("public"));
// create http server
var server = http.createServer(app);
var ws = new WebSocket.Server({
	server,
});

var clients = [];
connect();
function broadcast(socket, data) {
	console.log(clients.length);
	for (var i = 0; i < clients.length; i++) {
		if (clients[i] != socket) {
			clients[i].send(data);
		}
	}
}
function checkUID(uid) {
	return new Promise((resolve, reject) => {
		connection.query(
			`SELECT * FROM rfidcard WHERE rfid="${uid}"`,
			(err, result) => {
				if (err) reject(err);
				if (result.length === 0) resolve(false);
				else resolve(true);
			}
		);
	});
}

ws.on("connection", function (socket, req) {
	clients.push(socket);
	console.log("connected!");

	socket.on("message", function (message) {
		console.log("received: %s", message);
		if (message.toString().includes("UID#")) {
			let UID = message.toString().substring(4);
			checkUID(UID).then((existed) => {
				if (existed) {
					broadcast(socket, constant.BR_ON);
				} else {
					broadcast(socket, "Ma the sai");
				}
			});
		}
		broadcast(socket, message.toString());
	});

	socket.on("close", function () {
		var index = clients.indexOf(socket);
		clients.splice(index, 1);
		console.log("disconnected");
	});
});

server.listen(3000, function () {
	console.log("listening on: 3000");
});
