#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define PI 3.141592653589793238
#define EARTH_RADIUS 6378.137e3
#define GRAVITY 9.80665

double distance(double lng, double lng2, double lat, double lat2) {
    double deltaLat = lat2 - lat;
    double deltaLong = lng2 - lng;
    double hv = pow(sin(deltaLat/2), 2) + cos(lat) * cos(lat2) * pow(sin(deltaLong/2), 2);
    double c = 2 * atan2(sqrt(hv), sqrt(1-hv));
    return EARTH_RADIUS * c;
}

double bearing(double lng, double lng2, double lat, double lat2) {
    double x = cos(lat * PI/180) * sin(lat2 * PI/180) - sin(lat * PI/180) * cos(lat2 * PI/180) * cos(lng2 * PI/180 - lng * PI/180);
    double y = sin(lng2 * PI/180 - lng * PI/180) * cos(lat2 * PI/180);
    double theta = atan2(y,x);
    return fmod((theta * 180 / PI + 360), 360); // Returns value in degrees
}
/* To fly one direction at a time (x,y) just fly along latitude or longitude
only to treat as Manhattan distance

To see how it works:
===========================
1. Take difference between Aircraft & Destination latitudes
2. Take difference between aircraft & Destination longitudes
3. Fly along latitude direction until difference is close to 0
4. Fly along longitude direction until difference is close to 0
5. Result should be close to destination
*/
double haversine(double a, double b) {
    double hav = pow(sin((b - a)/2),2);
    return hav;
}

double rollAngle(double toRad, int turnAngle) { // Assuming the RC aircraft goes at fixed average speed of 90kph or about 56mph
    // Finding bank angle required to complete rate turn of 30° per second at speed of 90kph (in radians)
    // This will essentially find the roll angle (in radians)
    double bankAngle = atan((turnAngle * toRad * 25*(463/900))/GRAVITY);
    return bankAngle;
}
    // Assuming average rudder angle capability range of -23.5° to 23.5° for yaw control
double yawAngle(double delta_Long) {
	if (delta_Long > 0) { // Move East
		// Signal output to change rudder angle to 23.5° relative to 0 degree idle (to turn right)
	} else {
		// Signal output to change rudder angle to -23.5° relative to 0 degree idle (to turn left)
	}
}

void setup() {
//    Serial.begin(9600); // Baud Rate
}

int main() { // Change to void loop
    const double to_Rad = PI / 180; // radian conversion
    const int turnRate = 30; // degrees
    const double aircraftSpeed = 90; // kph
    // assign float integer for lat-long
    // still need to figure out howto assign raw GPS data to these variables
//================ User Inputs & Data Feed =================//
    double DestLong, DestLat;
    double RCLong, RCLat; // RC's current longitude/latitude position
    double bear; // Clockwise angle to go from point A to destination relative from North
    
    // For Testing purposes Format: (Lat, Long) in Linux C with -lm
    // Coordinates from top left to bottom right in alumni park
    DestLat = 45.382931535169504;
    DestLong = -75.69466778891062;
    RCLat = 45.38332526682105;
    RCLong = -75.69554218904666;
//================= Constant Data Updates ====================//

	// Convert degrees to radians
	DestLat *= (PI/180);
	DestLong *= (PI/180);
	RCLat *= (PI/180);
	RCLong *= (PI/180);

    double deltaLat = DestLat - RCLat;
    double deltaLong = fabs(DestLong- RCLong);

    // Haversine bearing between two coordinates
    double hav = haversine(DestLat, RCLat) + cos(RCLat) * cos(DestLat) * haversine(DestLong, RCLong);

    // Finding angular distance
    double angDist = 2 * atan(sqrt(hav)/sqrt(1-hav));
    // Find distance from current position to destination
    double distance = EARTH_RADIUS * angDist;

    // Find bearing angle to the destination relative to North clockwise
    bear = bearing(RCLong, DestLong, RCLat, DestLat);
    
    // To determine turning relationship between roll & yaw for turning
    // Rate turn of 30° per second or 360° every 12s
    double turn = rollAngle(to_Rad, turnRate);
    if (fabs(RCLong - DestLong) > 0 || fabs(RCLat - DestLat) > 0) {
        
        // Roll aircraft to bankAngle
        // Yaw aircraft to full tilt
    }
    printf("Direct distance between Aircraft and Destination: %.3fm\n", distance);
    printf("Bearing from Aircraft relative to North: %.3f°\n", bear);
    
    // Finds x-distance equivalent of travelling along Longitude line (East-West)
    double x = distance * sin(bear * PI/180);
    // Finds y-distance equivalent of travelling along Latitude line (North-South)
    double y = distance * cos(bear * PI/180);
    
    if (deltaLat > 0) {
    	printf("Move North for %0.8fm\n", fabs(y));
    } else {
    	printf("Move South %0.8fm\n", fabs(y));
    }
    
    if (deltaLong > 0) {
    	printf("Move East for %0.8fm\n", fabs(x));
    	yawAngle(deltaLong);
    } else {
    	printf("Move West for %0.8fm\n", fabs(x));
    	yawAngle(deltaLong);
    }
    return 0;
}

