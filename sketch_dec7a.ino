// TinyGPS - Version: Latest 

#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define gpsRX 3
#define gpsTX 4
#define RUDDER_PIN 9

float lat, lon;
float distance, course;

float delta_lat, delta_lon;
// Destination
const float dest_lat = 45.382931535169504, dest_lon = -75.69466778891062;

SoftwareSerial gpsSerial(gpsRX, gpsTX); // RX, TX
TinyGPS gps;
Servo testServo;

void turnMech(float delta_Long) {
	if (delta_Long > 0) { // Move East
		// Signal output to change rudder angle to 23.5° relative to 0 degree idle (to turn right)
      for (int k = 0; k < 90; k++)
        testServo.write(k); // Have to figure out how to control the servo to the rudder
	} else {
		// Signal output to change rudder angle to -23.5° relative to 0 degree idle (to turn left)
        testServo.write(155); // Have to figure out how to control the servo to the rudder
	}
}

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  pinMode(gpsRX, INPUT); // Nano-GPS RX
  pinMode(gpsTX, OUTPUT); // Nano-GPS TX
  testServo.attach(RUDDER_PIN);
}

void loop() {
  while (gpsSerial.available()) {
    if (gps.encode(gpsSerial.read())) { // Takes a bit of time to establish GPS-satellite connection
      // Gives Latitude, Longitude of GPS Module
      gps.f_get_position(&lat, &lon);
      String latitude = String(lat, 6);
      String longitude = String(lon, 6);
      Serial.println("(Lat, Long): " + latitude + ", " + longitude);

      // Gives distance & course between GPS (relative to North) & destination
      distance = gps.distance_between(lat, lon, dest_lat, dest_lon);
      course = gps.course_to(lat, lon, dest_lat, dest_lon);
      Serial.println("Distance & Course to destination: " + String(distance, 6) + "m at " + String(course, 6) + "°"); // Close with error of about 2-4 meters

      // Get deltas
      delta_lat = dest_lat - lat;
      delta_lon = dest_lon - lon;
      turnMech(delta_lon);
      delay(100);
    }
  }
  while (gpsSerial.available() < 0) {
    Serial.print("gpsSerial is not available");   
  }
}
