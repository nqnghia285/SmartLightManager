/*
 * SmartLightManager.cpp
 *
 *  Created on: Apr 22, 2021
 *      Author: nqnghia285
 */
#include <SmartLightManager.h>

SmartLightManager::SmartLightManager() {}
SmartLightManager::~SmartLightManager() {}

/**
 * @brief Match lightId (key) with pin (value)
 * 
 * @param lightId int
 * @param pin int
 */
void SmartLightManager::mapPin(int lightId, int pin)
{
    _lightPins[lightId] = pin;
}

/**
 * @brief Match lightId (key) with pin (value), param is a string follow format: "[[lightId,pin],[lightId,pin],...]"
 * 
 * @param arrLightPins const char *
 */
void SmartLightManager::mapPin(const char *arrLightPins)
{
    DynamicJsonDocument params(512);
    DeserializationError err = deserializeJson(params, arrLightPins);

    if (err == DeserializationError::Ok)
    {
        // Get a reference to the root array
        JsonArray arr = params.as<JsonArray>();
        for (JsonArray::iterator it = arr.begin(); it != arr.end(); ++it)
        {
            mapPin(it->getElement(0), it->getElement(1));
            // Serial.println("LightId: " + String(_lightPins.at(it->getElement(0))));
            // Serial.println("Pin: " + String(_lightPins.at(it->getElement(0))));
        }
    }
    else
    {
        DEBUG("[DEBUG] arrLightPins invalid!", arrLightPins);
    }

    // Clear store
    params.clear();
}

/**
 * @brief Match lightId (key) with pin (value), param is a string follow format: "[[lightId,pin],[lightId,pin],...]"
 * 
 * @param arrLightPins String
 */
void SmartLightManager::mapPin(String arrLightPins)
{
    mapPin(arrLightPins.c_str());
}

/**
 * @brief Set pin mode
 * 
 */
void SmartLightManager::setPinMode()
{
    for (auto it = _lightPins.begin(); it != _lightPins.end(); ++it)
    {
        pinMode(it->second, OUTPUT);
        // Serial.println("Key: " + String(it->first));
        // Serial.println("Pin: " + String(it->second));
    }
}

int SmartLightManager::getStatus(int lightId)
{
    auto l = _lightPins.find(lightId);
    if (l != _lightPins.end())
    {
        return digitalRead(l->second);
    }
    else
    {
        DEBUG("[DEBUG] lightId %s not found, can not be removed!", lightId);
        return LOW;
    }
}

/**
 * @brief Remove the pin in _lightPins
 * 
 * @param lightId int
 */
void SmartLightManager::removePin(int lightId)
{
    auto l = _lightPins.find(lightId);
    if (l != _lightPins.end())
    {
        _lightPins.erase(l);
    }
    else
    {
        DEBUG("[DEBUG] lightId %s not found, can not be removed!", lightId);
    }
}

/**
 * @brief Remove all pins in _lightPins
 * 
 */
void SmartLightManager::removeAllPin()
{
    _lightPins.clear();
}

/**
 * @brief Turn on a light with lightId
 * 
 * @param lightId int
 * @return bool
 */
bool SmartLightManager::turnOn(int lightId)
{
    const auto it = _lightPins.find(lightId);
    if (it != _lightPins.end())
    {
        digitalWrite(it->second, HIGH);
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Turn off a light with lightId
 * 
 * @param lightId int
 * @return bool 
 */
bool SmartLightManager::turnOff(int lightId)
{
    const auto it = _lightPins.find(lightId);
    if (it != _lightPins.end())
    {
        digitalWrite(it->second, LOW);
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Control a light
 * 
 * @param lightId int
 * @param status bool
 * @return bool
 */
bool SmartLightManager::controlLight(int lightId, bool status)
{
    if (status)
    {
        return turnOn(lightId);
    }
    else
    {
        return turnOff(lightId);
    }
}

/**
 * @brief Control a light, param is a string follow format: "[lightId,status]"
 * 
 * @param ctrlString const char *
 * @return bool
 */
bool SmartLightManager::controlLight(const char *ctrlString)
{
    DynamicJsonDocument params(512);
    DeserializationError err = deserializeJson(params, ctrlString);
    bool isSuccess = false;

    if (err == DeserializationError::Ok)
    {
        // Get a reference to the root array
        JsonArray arr = params.as<JsonArray>();
        isSuccess = controlLight(arr.getElement(0), arr.getElement(1));
    }
    else
    {
        DEBUG("[DEBUG] ctrlString invalid!", ctrlString);
    }

    // Clear store
    params.clear();

    return isSuccess;
}

bool SmartLightManager::controlLight(String ctrlString)
{
    return controlLight(ctrlString.c_str());
}

/**
 * @brief Control multiple lights at the same time, param is a string follow format: "[[lightId,status],[lightId,status],...]"
 * 
 * @param ctrlString const char *
 * @return bool
 */
bool SmartLightManager::mixControl(const char *ctrlString)
{
    DynamicJsonDocument params(512);
    DeserializationError err = deserializeJson(params, ctrlString);
    bool isSuccess = true;

    if (err == DeserializationError::Ok)
    {
        // Get a reference to the root array
        JsonArray arr = params.as<JsonArray>();
        for (JsonArray::iterator it = arr.begin(); it != arr.end(); ++it)
        {
            isSuccess = controlLight(it->getElement(0), it->getElement(1)) ? isSuccess : false;
        }
    }
    else
    {
        DEBUG("[DEBUG] ctrlString invalid!", ctrlString);
        isSuccess = false;
    }

    // Clear store
    params.clear();

    return isSuccess;
}

/**
 * @brief Control multiple lights at the same time, param is a string follow format: "[[lightId,status],[lightId,status],...]"
 * 
 * @param ctrlString String
 * @return bool
 */
bool SmartLightManager::mixControl(String ctrlString)
{
    return mixControl(ctrlString.c_str());
}