#ifndef JSON_H
#define JSON_H

#include "Arduino.h"
#include "FlashString.h"
#include "Log.h"
#include "iterator"
#include "map"
#include "vector"

class Json;
class JsonArray;

class JSType {
   public:
    enum Type : uint8_t {
        jsonObject,
        jsonArray,
        jsonInteger,
        jsonFloat,
        jsonString,
        jsonBoolean,
        jsonNull,
        undefined
    };

    struct Element {
       private:
        String *value;
        Type type;

       public:
        Element();
        Element(const String &value, const Type &type);
        Element(const Element &e);
        ~Element();
        Type getType() const;
        String getValue() const;
        bool operator==(const Element &e);
    };

    struct Value {
        String value;
        operator int8_t();
        operator int16_t();
        operator int32_t();
        operator uint8_t();
        operator uint16_t();
        operator uint32_t();
        operator float();
        operator double();
        operator String();
        operator void *();
        operator bool();
        operator Json();
        operator JsonArray();
    };
};

class JsonUtil {
   public:
    void parse(const char *str, Json *doc, JsonArray *v);
    bool isFloat(const String &str);
    String typeToString(const JSType::Type &type);
    void prettyPrint(HardwareSerial &serial, Json &doc, const uint8_t &spaceTab = 2);
    void prettyPrint(HardwareSerial &serial, JsonArray &v, const uint8_t &spaceTab = 2);

   private:
    void prettyPrint(HardwareSerial &serial, Json *doc, JsonArray *v, const uint8_t &spaceTab, const uint8_t &space);
};

extern JsonUtil JSUtil;

class JsonArray {
   public:
    JsonArray();
    JsonArray(const char *str);
    JsonArray(const String &str);
    JsonArray(std::vector<int> &arr);

    JsonArray &push(const String &value, const JSType::Type &type);
    JsonArray &push(Json &value);
    JsonArray &push(JsonArray &value);
    JsonArray &push(const char *value);
    JsonArray &push(const String &value);
    JsonArray &push(const int8_t &value);
    JsonArray &push(const int16_t &value);
    JsonArray &push(const int32_t &value);
    JsonArray &push(const uint8_t &value);
    JsonArray &push(const uint16_t &value);
    JsonArray &push(const uint32_t &value);
    JsonArray &push(const bool &value);
    JsonArray &push(const void *value);
    JsonArray &push(const float &value, const uint8_t &precision = 2);
    JsonArray &push(const double &value, const uint8_t &precision = 2);

    JSType::Value getValue(const uint16_t &index);
    JSType::Value operator[](const uint16_t &index);

    JSType::Element getElement(const uint16_t &index);
    JSType::Type getType(const uint16_t &index);
    String getTypeString(const uint16_t &index);
    String toString();

    size_t size();
    bool contains(const JSType::Element &e);
    void erase(const uint16_t &index);
    std::vector<JSType::Element>::iterator begin();
    std::vector<JSType::Element>::iterator end();

   private:
    std::vector<JSType::Element> v;
};

class Json {
   public:
    Json();
    Json(const char *str);
    Json(const String &str);

    Json &add(const String &name, const String &value, const JSType::Type &type);
    Json &add(const String &name, Json &value);
    Json &add(const String &name, JsonArray &value);
    Json &add(const String &name, const char *value);
    Json &add(const String &name, const String &value);
    Json &add(const String &name, const int8_t &value);
    Json &add(const String &name, const int16_t &value);
    Json &add(const String &name, const int32_t &value);
    Json &add(const String &name, const uint8_t &value);
    Json &add(const String &name, const uint16_t &value);
    Json &add(const String &name, const uint32_t &value);
    Json &add(const String &name, const bool &value);
    Json &add(const String &name, const void *value);
    Json &add(const String &name, const float &value, const uint8_t &precision = 2);
    Json &add(const String &name, const double &value, const uint8_t &precision = 2);

    JSType::Value getValue(const String &name);
    JSType::Value getValue(const uint16_t &index);
    JSType::Value operator[](const String &name);
    JSType::Value operator[](const uint16_t &index);

    JSType::Element getElement(const String &name);
    JSType::Element getElement(const uint16_t &index);

    JSType::Type getType(const String &name);
    JSType::Type getType(const uint16_t &index);
    String getTypeString(const String &name);
    String getTypeString(const uint16_t &index);
    String getKey(const uint16_t &index);
    
    int16_t getIndex(const String &name);
    String toString();

    size_t size() const;
    bool contains(String name) const;
    void erase(const String &name);
    std::map<String, JSType::Element>::iterator begin();
    std::map<String, JSType::Element>::iterator end();

    static void attachDebugger(HardwareSerial &serial);
    static void detachDebugger();

   private:
    std::map<String, JSType::Element> doc;
    std::map<uint16_t, String> indexList;
    uint16_t counter;
    void arrangeIndex(uint16_t index);
};

#endif
