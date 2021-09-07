#include "BoodskapGateway.h"


void BoodskapGateway::processMessage(byte* payload, size_t len){

    if(listener() == NULL){
        println("MessageListener not set, no point in processing...");
        return;
    }

    if (payload[0] == '{' && payload[len - 1] == '}')
    {

        DynamicJsonDocument doc(MAX_PACKET_SIZE);
        deserializeJson(doc, String((char*)payload));
        JsonObject root = doc.as<JsonObject>();

        bool invalid = root.isNull();

        invalid = invalid || (!root.containsKey("header"));
        invalid = (invalid || !root.containsKey("data"));

        if (invalid)
        {
            println("Not a JSON message");
            listener()->onData(len, payload);
            return;
        }

        JsonObject header = root["header"];
        JsonObject message = root["data"];

        bool shouldAck = header.containsKey("corrid");
        uint32_t messageId = header["mid"].as<uint32_t>();
        int acked = listener()->onMessage(messageId, header, message);

        if(shouldAck){
            uint64_t corrId = header["corrid"].as<uint64_t>();

            StaticJsonDocument<80> ack;
            JsonObject reply = ack.to<JsonObject>();
            reply["corrid"] = corrId;
            reply["acked"] = acked;

            send(2, ack);
        }
    }else{
        listener()->onData(len, payload);
    }
}

void BoodskapGateway::setPrinter(Print &printer)
{
    this->_printer = &printer;
}

void BoodskapGateway::print(const char *msg)
{
    DEBUG_PRINT(_printer, msg);
}

void BoodskapGateway::println(const char *msg)
{
    DEBUG_PRINTLN(_printer, msg);
}

void BoodskapGateway::setServer(const char *ip, uint16_t port)
{
    strcpy(this->_ip, ip);
    this->_port = port;
}

void BoodskapGateway::setDomainKey(const char *domainKey)
{
    strcpy(this->_domainKey, domainKey);
}

void BoodskapGateway::setApiKey(const char *apiKey)
{
    strcpy(this->_apiKey, apiKey);
}

void BoodskapGateway::setDeviceId(const char *deviceId)
{
    strcpy(this->_deviceId, deviceId);
}

void BoodskapGateway::setDeviceModel(const char *deviceModel)
{
    strcpy(this->_deviceModel, deviceModel);
}

void BoodskapGateway::setFwVersion(const char *fwVersion)
{
    strcpy(this->_fwVersion, fwVersion);
}

char *uint64_to_string(uint64_t input)
{
    static char result[21] = "";
    // Clear result from any leftover digits from previous function call.
    memset(&result[0], 0, sizeof(result));
    // temp is used as a temporary result storage to prevent sprintf bugs.
    char temp[21] = "";
    char c;
    uint8_t base = 10;

    while (input) 
    {
        int num = input % base;
        input /= base;
        c = '0' + num;

        sprintf(temp, "%c%s", c, result);
        strcpy(result, temp);
    } 
    return result;
}