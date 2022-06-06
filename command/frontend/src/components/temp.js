app.post("/move", (req,res) => {
    console.log("Request received: " + JSON.stringify(req.body));
    publish('toESP32/dir', req.body.direction)
    res.send("Received direction " + req.body.direction);
});

app.post("/mode", (req, res) => {
	publish('toESP32/mode', req.body.mode);
	res.send("Updated mode " + req.body.mode);
})

// Incorrect route
app.use((req, res, next) => {
    res.status(404).send("404: This route doesn't exist");
});

// Server
// const httpServer = http.createServer(app);
const httpsServer = https.createServer(SSL_options, app);


httpsServer.listen(HTTPS_port, () => {
    console.log(`Listening at URL https://debonair.duckdns.org:${HTTPS_port}`);
})

// Handles shutting down application on critical errors
process.on('SIGTERM', () => {
	httpsServer.close(() => {
		console.log('HTTPS server terminated');
	});
	client.end(() => {
		console.log('MQTT client disconnected');
	});
	db_client.close(() => {
		console.log("Disconnected from MongoDB");
	});
})