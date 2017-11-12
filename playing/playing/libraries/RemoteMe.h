// RemoteMe.h

#include <ArduinoHttpClient.h>
#include <WebSocketsClient.h>
#include <Hash.h>

#include "RemoteMeDataStructures.h"


#ifndef _REMOTEME_h
#define _REMOTEME_h

#define LED     D5   




class RemoteMe
{
	const char * token;
	uint16_t deviceId;
	bool conencted;

	WebSocketsClient* webSocket = nullptr;
	

private:
	RemoteMe(char * token, uint16_t deviceId );
	
	uint16_t putShort(uint8_t* data, uint16_t pos, uint16_t number);
	uint16_t putByte(uint8_t* data, uint16_t pos, uint8_t number);
	uint16_t putArray(uint8_t* data, uint16_t pos, const void* number, uint16_t length);
	uint16_t putString(uint8_t * data, uint16_t pos, String string);
	uint16_t putLong(uint8_t * data, uint16_t pos, uint64_t number);
	uint16_t putDouble(uint8_t * data, uint16_t pos, double value);

	uint8_t* getReverseBytes(void* start, uint16_t size);
	uint16_t putBigEndian(uint8_t * data, uint16_t pos, void * value, uint16_t size);

	
protected:
	void send(uint8_t * payload, size_t length);
	
public:
	static RemoteMe& getInstance(char * token, int deviceId )
	{
		static RemoteMe    instance(token, deviceId); // Guaranteed to be destroyed.
							  // Instantiated on first use.
		return instance;
	}


	RemoteMe(RemoteMe const&) = delete;
	void operator=(RemoteMe const&) = delete;

	static void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);
	void setupTwoWayCommunication();
	void stupidPrint();
	void loop();

	void sendExamplePost();
	void waitForConnect();

	void sendAddDataMessage(uint16_t seriesId, AddDataMessageSetting settings, uint64_t time, double value);

	void sendUserMessage(WSUserMessageSettings renevalWhenFailType, uint16_t receiverDeviceId, uint16_t messageId, const uint8_t * payload, uint16_t length);
	void sendUserMessage(WSUserMessageSettings renevalWhenFailType, uint16_t receiverDeviceId, uint16_t senderDeviceId, uint16_t messageId, const uint8_t *payload, uint16_t length);
	void sendUserMessage(WSUserMessageSettings renevalWhenFailType, uint16_t receiverDeviceId, uint16_t messageId, String message);
	void sendUserMessage(WSUserMessageSettings renevalWhenFailType, uint16_t receiverDeviceId, uint16_t senderDeviceId, uint16_t messageId, String message);

	void sendRegisterDeviceMessage(uint16_t deviceId, String deviceName, DeviceType deviceType);
	void sendRegisterDeviceMessage( String deviceName );
	void sendRegisterDeviceMessage();

	void sendRegisterChildDeviceMessage(uint16_t parentDeviceId, uint16_t deviceId, String deviceName );
	void sendRegisterChildDeviceMessage(uint16_t deviceId, String deviceName);
	void sendRegisterChildDeviceMessage(uint16_t deviceId);
	void sendLogMessage(String str);



	template<typename... Args> void sendLogMessage(const char* fmt, Args... args) {
		char* buf = new char[100];
		int cx=	snprintf(buf,100, fmt, args...);
		if (cx > 100) {
			buf = new char[cx];
			snprintf(buf, cx, fmt, args...);
		}
		Serial.println(buf);
	}

	


	
};


#endif

