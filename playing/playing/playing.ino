
#include <ArduinoHttpClient.h>
#include "libraries/Remoteme.h"
#include "libraries/RemoteMeDataStructures.h"
#include <ESP8266WiFi.h>

#include <ESP8266WiFiMulti.h>


#include "secret.h"

#define INPUT_PIN   A0
#define LED     D5   

ESP8266WiFiMulti WiFiMulti;
RemoteMe& remoteMe= RemoteMe::getInstance("~1_ySKpyx+'G23",123);






void setup() {
	digitalWrite(INPUT_PIN, INPUT);
	pinMode(LED, OUTPUT);
	Serial.begin(9600);
	Serial.print("started");
	while (!Serial) {
	}
	WiFiMulti.addAP(ssid, password);
	while (WiFiMulti.run() != WL_CONNECTED) {
		delay(100);
		Serial.print(".");
	}
	Serial.print("Connected to wifi");
	Serial.print("Connected, IP address: ");
	Serial.println(WiFi.localIP());


	remoteMe.setupTwoWayCommunication();
	
	remoteMe.waitForConnect();
	
	Serial.print("setup end");
}




void loop() {
	double static value = 0.32;
	remoteMe.loop();
	//remoteMe.sendAddDataMessage(12345, NO_ROUND, 12345678901l, value);
											
//	remoteMe.sendUserMessage(NO_RENEWAL,123,32,"maciek sajdak");

	//remoteMe.sendRegisterChildDeviceMessage(345,5678,"maciek sajdak");

	remoteMe.sendLogMessage(" maciek  %d asdad %d sdfsdf", 1, 23);

	value += 1.23;

	Serial.println(sizeof(double));

	//remoteMe.sendExamplePost();
	//delay(500);	
	Serial.print(".");

	delay(500);
	digitalWrite(LED, HIGH);
	delay(500);
	
	digitalWrite(LED, LOW);
	

}
