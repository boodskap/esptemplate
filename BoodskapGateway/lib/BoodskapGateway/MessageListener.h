#ifndef _MESSAGE_LISTENER_H_
#define _MESSAGE_LISTENER_H_

#include <ArduinoJson.h>

class MessageListener
{

public:

    virtual int onMessage(uint64_t mid, JsonObject& header, JsonObject& data) = 0;

    virtual void onData(size_t len, byte* data) = 0;

};


#endif //_MESSAGE_LISTENER_H_