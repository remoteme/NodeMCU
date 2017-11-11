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
	uint16_t putArray(uint8_t* data, uint16_t pos, const uint8_t* number, uint16_t length);

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

	void sendUserMessage(WSUserMessageSettings renevalWhenFailType, uint16_t receiverDeviceId, uint16_t messageId, const uint8_t * payload, uint16_t length);
	void sendUserMessage(WSUserMessageSettings renevalWhenFailType, uint16_t receiverDeviceId, uint16_t senderDeviceId, uint16_t messageId, const uint8_t *payload, uint16_t length);
	void sendUserMessage(WSUserMessageSettings renevalWhenFailType, uint16_t receiverDeviceId, uint16_t messageId, String message);
	void sendUserMessage(WSUserMessageSettings renevalWhenFailType, uint16_t receiverDeviceId, uint16_t senderDeviceId, uint16_t messageId, String message);


};


#endif

