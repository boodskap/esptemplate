#ifndef _BOODSKAP_GATEWAY_H_
#define _BOODSKAP_GATEWAY_H_

#include <ArduinoJson.h>
#include "DummyPrinter.h"
#include "MessageListener.h"
#include "Config.h"

char *uint64_to_string(uint64_t input);

class BoodskapGateway
{

public:
  BoodskapGateway() : _printer(&VoidPrinter), _listener(NULL), _port(1883)
  {
    
    setDomainKey("");
    setApiKey("");
    setDeviceId("");
    setDeviceModel("-");
    setFwVersion("-");
  }

  void setPrinter(Print &printer);

  void print(const char *msg);

  void println(const char *msg);

  void setServer(const char* ip, uint16_t port);

  const char* ip() { return _ip; }

  uint16_t port() { return _port; }

  void setDomainKey(const char *domainKey);

  const char* domainKey(){ return _domainKey; }

  void setApiKey(const char *apiKey);

  const char* apiKey() { return _apiKey; }

  void setDeviceId(const char *deviceId);

  const char* deviceId() { return _deviceId; }

  void setDeviceModel(const char *deviceModel);

  const char* deviceModel() { return _deviceModel; }

  void setFwVersion(const char* fwVersion);

  const char* fwVersion() { return _fwVersion; }

  void setListener(MessageListener* listener) { this->_listener = listener;}

  MessageListener* listener() { return _listener; }

  Print* printer() { return _printer; }

  virtual void loop() = 0;

  virtual bool connected() = 0;

  /**
     * 
     * Send's a regular JSON Message to the platform
     * mid -> Defined Message ID
     * data -> Json document
     */
   virtual bool send(uint64_t mid, JsonDocument &data) = 0;

  /**
     * 
     * Send's a binary Message to the platform
     */
   virtual bool send(const char *type, const char *data) = 0;

  /**
     * 
     * Invokes a named rule in the platform
     * Returns a session id, this can be used to verfy if the rule has been trigered successfully in the callback
     */
   virtual const char *trigger(const char *rule, JsonDocument &args) = 0;

  /**
     * 
     * Inserts a record into the storage using auto generated unique ID
     * rid -> Defined Record ID
     * data -> Json document
     * Return's the auto generated Record's unique identifier
     */
   virtual const char *insert(uint64_t rid, JsonDocument &data) = 0;

  /**
     * 
     * Inserts a record into the storage
     * rid -> Defined Record ID
     * id -> Record's unique identifier
     * data -> Json document
     * Return's the Record's unique identifier
     */
   virtual const char *insert(uint64_t rid, const char *id, JsonDocument &data) = 0;

   void processMessage(byte* data, size_t len);

protected:
  Print *_printer;
  MessageListener* _listener;
  uint16_t _port;
  char _ip[IP_ADDR_SIZE];
  char _domainKey[DOMAIN_KEY_SIZE];
  char _apiKey[API_KEY_SIZE];
  char _deviceId[DEVICE_ID_SIZE];
  char _deviceModel[DEVICE_MODEL_SIZE];
  char _fwVersion[FW_VERSION_SIZE];
};

#endif // _BOODSKAP_GATEWAY_H_