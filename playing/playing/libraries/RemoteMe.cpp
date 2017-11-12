// 
// 
// 

const char* host = "192.168.0.30";
const int port = 8082;

#include "RemoteMe.h"
#include <HardwareSerial.h>


RemoteMe::RemoteMe(char * token, uint16_t deviceId ) {
	this->token = std::move(token);
	this->deviceId = deviceId;
	
}



void RemoteMe::webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
	static bool zm = true;

	switch (type) {
	case WStype_DISCONNECTED:
		Serial.print("disConnected web socket");
		RemoteMe::getInstance("", 0).conencted = false;
		break;
	case WStype_CONNECTED:
		RemoteMe::getInstance("", 0).conencted = true;
		RemoteMe::getInstance("", 0).webSocket->setReconnectInterval(20000);

		Serial.print("websocket conencted");
		break;
	case WStype_TEXT:

		// send message to server
		// webSocket.sendTXT("message here");
		break;
	case WStype_BIN:
		Serial.print("got data");
		if (zm) {
			digitalWrite(LED, HIGH);
			zm = 0;
		}
		else {
			digitalWrite(LED, LOW);
			zm = 1;
		}

		break;
	}

}

void RemoteMe::sendExamplePost() {
	
	//secure
	//WiFiClient  tcp = new WiFiClientSecure();

	char asdds[20];
	asdds[0] = 23;
	asdds[1] =42;
	asdds[2] = 3;
	asdds[10] = 0;
	HttpClient* httpClient;
	if (httpClient == nullptr) {
		WiFiClient wifiClient;

		httpClient = new HttpClient(wifiClient, host, port);

	}
	

	Serial.print("trying send");
	

	httpClient->beginRequest();
	Serial.print("1");
	httpClient->post("/api/rest/v1/sender/sendMessage2/");
	Serial.print("2");
	httpClient->sendHeader("Content-Type", "text/plain");
	Serial.print("3");
	httpClient->sendHeader("Content-Length", 20);
	Serial.print("4");
	httpClient->sendHeader("token", "~1_ySKpyx+'G23");
	httpClient->beginBody();
	httpClient->print(asdds);


	Serial.println("endRequest send");
	httpClient->endRequest();
	Serial.println("32");

	// read the status code and body of the response
	int statusCode = httpClient->responseStatusCode();

	Serial.println(statusCode);

}

void RemoteMe::setupTwoWayCommunication() {
	if (webSocket != nullptr) {
		webSocket->disconnect();
	}
	webSocket = new WebSocketsClient();
	webSocket->begin("192.168.0.30", 8082, "/api/ws/v1/34", "as");
	webSocket->setAuthorization("token", token); // HTTP Basic Authorization

	webSocket->setReconnectInterval(500);
	
	
	

	webSocket->onEvent(RemoteMe::webSocketEvent);
	webSocket->sendPing();

}

void RemoteMe::stupidPrint() {
	Serial.print("parr");
}


uint16_t RemoteMe::putString(uint8_t* data, uint16_t pos, String string ) {
	const uint8_t* dataString = reinterpret_cast<const uint8_t*>(&string[0]);
	pos = putArray(data, pos, dataString, string.length());
	data[pos] = 0;
	return pos + 1;

}
uint16_t RemoteMe::putArray(uint8_t* data, uint16_t pos, const void* arrayT, uint16_t length) {

	memcpy(&data[pos], arrayT, length);

	return pos + length;
}

uint16_t RemoteMe::putByte(uint8_t* data, uint16_t pos, uint8_t number) {
	data[pos] = number;

	return pos + 1;
}

uint16_t RemoteMe::putShort(uint8_t* data, uint16_t pos, uint16_t number ) {
	return putBigEndian(data, pos, &number, sizeof(uint16_t));
}

uint16_t RemoteMe::putLong(uint8_t* data, uint16_t pos, uint64_t number) {
	return putBigEndian(data, pos, &number, sizeof(uint64_t));
}


uint8_t* RemoteMe::getReverseBytes(void* start, uint16_t size) {
	uint8_t* ret = new uint8_t[size];

	for (uint8_t i = 0; i < size; i++) {
		ret[i] =((uint8_t*) start)[size - i - 1];
	
	}


	return ret;
}

uint16_t RemoteMe::putBigEndian(uint8_t* data, uint16_t pos, void* value, uint16_t size) {
	return putArray(data, pos, getReverseBytes(value, size), size);
}

uint16_t RemoteMe::putDouble(uint8_t* data, uint16_t pos, double value) {
	return putBigEndian(data,pos,& value,sizeof(double));
}

void RemoteMe::sendUserMessage(WSUserMessageSettings renevalWhenFailType, uint16_t receiverDeviceId, uint16_t messageId, const uint8_t *payload, uint16_t length) {
	sendUserMessage(renevalWhenFailType, receiverDeviceId, deviceId, messageId, payload, length);
}

void RemoteMe::sendUserMessage(WSUserMessageSettings renevalWhenFailType, uint16_t receiverDeviceId, uint16_t messageId, String message) {
	this->sendUserMessage(renevalWhenFailType, receiverDeviceId, deviceId, messageId, message);
}

void RemoteMe::sendUserMessage(WSUserMessageSettings renevalWhenFailType, uint16_t receiverDeviceId, uint16_t senderDeviceId, uint16_t messageId, String message) {

	const uint8_t* data = reinterpret_cast<const uint8_t*>(&message[0]);
	sendUserMessage(renevalWhenFailType, receiverDeviceId, senderDeviceId, messageId, data, message.length());
}
void RemoteMe::sendUserMessage(WSUserMessageSettings renevalWhenFailType, uint16_t receiverDeviceId, uint16_t senderDeviceId, uint16_t messageId, const uint8_t *payload, uint16_t length) {
	uint16_t size = 7 + length;
	uint8_t* data = new uint8_t[size+4];
	

	uint8_t index = 0;
	

	index = putShort(data, index, USER_MESSAGE);
	index = putShort(data, index, size);

	index = putByte(data, index, renevalWhenFailType);
	index = putShort(data, index, receiverDeviceId);
	index = putShort(data, index, senderDeviceId);
	index = putShort(data, index, messageId);
	

	index = putArray(data, index, payload,length);

	
	send(data, size+4);

}

void RemoteMe::sendAddDataMessage(uint16_t seriesId, AddDataMessageSetting settings, uint64_t time, double value)
{
	uint16_t size = 11+sizeof(double) ;
	uint8_t* data = new uint8_t[size+4];


	uint8_t index = 0;


	index = putShort(data, index, ADD_DATA);
	index = putShort(data, index, size);

	index = putShort(data, index, seriesId);
	index = putByte(data, index, settings);
	index = putLong(data, index, time);
	index = putDouble(data, index, value);




	send(data, size+4);
}


void RemoteMe::send(uint8_t * payload, size_t length) {
	
	webSocket->sendBIN(payload, length);
}


void  RemoteMe::sendRegisterDeviceMessage(uint16_t deviceId, String deviceName, DeviceType deviceType) {
	uint16_t size = 4 + deviceName.length();//4 short byte + 1 for 0 for string
	uint8_t* data = new uint8_t[size + 4];


	uint8_t index = 0;


	index = putShort(data, index, REGISTER_DEVICE);
	index = putShort(data, index, size);

	index = putShort(data, index, deviceId);
	index = putString(data, index, deviceName);
	index = putByte(data, index, deviceType);
	




	send(data, size + 4);

}

void RemoteMe::sendRegisterDeviceMessage(String deviceName ) {
	sendRegisterDeviceMessage(deviceId, deviceName, NETWORK);
}

void RemoteMe::sendRegisterDeviceMessage() {
	sendRegisterDeviceMessage("");
}


void RemoteMe::sendRegisterChildDeviceMessage(uint16_t parentDeviceId, uint16_t deviceId, String deviceName) {
	uint16_t size = 5 + deviceName.length();//4 short  + 1 for 0 for string
	uint8_t* data = new uint8_t[size + 4];


	uint8_t index = 0;


	index = putShort(data, index, REGISTER_CHILD_DEVICE);
	index = putShort(data, index, size);

	index = putShort(data, index, parentDeviceId);
	index = putShort(data, index, deviceId);
	index = putString(data, index, deviceName);



	send(data, size + 4);
}
void RemoteMe::sendRegisterChildDeviceMessage(uint16_t deviceId, String deviceName) {
	sendRegisterChildDeviceMessage(this->deviceId,deviceId, "");
}
void RemoteMe::sendRegisterChildDeviceMessage(uint16_t deviceId) {
	sendRegisterChildDeviceMessage(deviceId, "");
}




void RemoteMe::sendLogMessage(String str) {

}


void RemoteMe::waitForConnect() {
	Serial.println("waiting for conenction");
	while (!conencted) {
		webSocket->loop();
		delay(100);
		
	}
	Serial.println(millis());

}

void RemoteMe::loop() {

		static unsigned long firstTimeFail = 0;
		static unsigned long lastTimePing = millis();
		static bool restarted = true;

		webSocket->loop();
		

		if (lastTimePing+60000<millis() || firstTimeFail!=0) {
			lastTimePing = millis();

			if (webSocket->sendPing()) {
				Serial.println("ping sent OK");
				firstTimeFail = 0;
			} else {
				if (firstTimeFail == 0) {//happen first time after connection established
					firstTimeFail = millis();
					restarted = false;
				}

				if (firstTimeFail + (restarted ? 1800 : 60) * 1000 < millis()) {// if it was already restarted we are allow websocket to do job but until half hour we restart again
					firstTimeFail = millis();
					restarted = true;
					setupTwoWayCommunication();//shoud hapopen 60s after first fail and then after halfhour
				}
				else {
					Serial.print("x");
				}
			}
		
		
	}
}