/**
 * SmartLightManager.h *
 *  Created on: Apr 22, 2021
 *      Author: nqnghia285
 */

#ifndef SMARTLIGHTMANAGER_H_
#define SMARTLIGHTMANAGER_H_

#include <map>
#include <ArduinoJson.h>

// #define DEBUG(...) Serial.printf(__VA_ARGS__);
#define DEBUG(...)

class SmartLightManager
{
private:
    std::map<int, int> _lightPins;

public:
    SmartLightManager(void);
    virtual ~SmartLightManager(void);

    void mapPin(int lightId, int pin);
    void mapPin(const char *arrLightPins);
    void mapPin(String arrLightPins);
    void removePin(int lightId);
    void removeAllPin(void);
    void setPinMode(void);
    int getStatus(int lightId);
    bool turnOn(int lightId);
    bool turnOff(int lightId);
    bool controlLight(int lightId, bool status);
    bool controlLight(const char *ctrlString);
    bool controlLight(String ctrlString);
    bool mixControl(const char *ctrlString);
    bool mixControl(String ctrlString);
};

#endif /* SMARTLIGHTMANAGER_H_ */