#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi credentials
const char *ssid = "wifi_name";
const char *password = "wifi_password";

// Initialize the web server on port 80
WebServer server(80);

// Pin for controlling the server (e.g., relay on GPIO 5)
const int serverControlPin = 5;
// Pin for built-in LED (commonly GPIO 2 on ESP32, check your board)
const int ledPin = 2;

void handle_root()
{
	String html = "<html><body>";
	html += "<h1>ESP32 Server Control</h1>";
	html += "<p><a href=\"/start\">Start Server</a></p>";
	html += "<p><a href=\"/stop\">Force Stop Server</a></p>";
	html += "</body></html>";
	server.send(200, "text/html", html);
}

void handle_start()
{
	// Flash the built-in LED
	for (int i = 0; i < 5; i++)
	{ // Flash 5 times
		digitalWrite(ledPin, HIGH);
		delay(200); // LED on for 200ms
		digitalWrite(ledPin, LOW);
		delay(200); // LED off for 200ms
	}

	// Trigger server control (e.g., relay)
	digitalWrite(serverControlPin, HIGH); // Activate relay
	digitalWrite(ledPin, HIGH); 		  // keep lighting up 
	delay(1000);						  // Hold for 1 second
	digitalWrite(serverControlPin, LOW);  // Deactivate
	digitalWrite(ledPin, LOW); 		      // turn off LED light

	server.send(200, "text/plain", "Server start command sent! LED flashed.");
}

void handle_stop()
{
	// Flash the built-in LED
	for (int i = 0; i < 5; i++)
	{ // Flash 5 times
		digitalWrite(ledPin, HIGH);
		delay(100); // LED on for 200ms
		digitalWrite(ledPin, LOW);
		delay(100); // LED off for 200ms
	}

	// Trigger server control (e.g., relay)
	digitalWrite(serverControlPin, HIGH); // Activate relay
	digitalWrite(ledPin, HIGH); 		  // keep lighting up 
	delay(8000);						  // Hold for 1 second
	digitalWrite(ledPin, LOW); 		      // turn off LED light
	digitalWrite(serverControlPin, LOW);  // Deactivate

	server.send(200, "text/plain", "Server stop command sent! LED flashed.");
}

void setup()
{
	Serial.begin(115200);

	// Initialize pins
	pinMode(serverControlPin, OUTPUT);
	digitalWrite(serverControlPin, LOW);
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, LOW);

	// Connect to Wi-Fi
	WiFi.begin(ssid, password);
	Serial.print("Connecting to Wi-Fi");
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println("\nConnected to Wi-Fi");
	Serial.print("IP Address: ");
	Serial.println(WiFi.localIP());

	// Print MAC Address
	Serial.print("MAC Address: ");
	Serial.println(WiFi.macAddress());

	// Set up web server routes - start
	server.on("/", handle_root);
	// Set up web server routes - start
	server.on("/start", handle_start);
	// Set up web server routes - stop
	server.on("/stop", handle_stop);

	// Start the server
	server.begin();
	Serial.println("Web server started");
}

void loop()
{
	server.handleClient(); // Handle incoming client requests
}
