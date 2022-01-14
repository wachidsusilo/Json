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
        ~Element();

        Element(const String &value, const Type &type);
        Element(const Element &e);
        Element(const int8_t &);
        Element(const int16_t &);
        Element(const int32_t &);
        Element(const uint8_t &);
        Element(const uint16_t &);
        Element(const uint32_t &);
        Element(const float &);
        Element(const double &);
        Element(const String &);
        Element(const char *);
        Element(const void *);
        Element(const bool &);
        Element(const Json &);
        Element(const JsonArray &);

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

        Element &operator=(const Element &);

        Type getType() const;
        String &getValue();
        const String &getValue() const;
        bool operator==(const Element &e) const;
    };
};

class JsonUtil {
   public:
    void parse(const char *str, Json *doc, JsonArray *v);
    bool isFloat(const String &str);
    String typeToString(const JSType::Type &type);
    void prettyPrint(HardwareSerial &serial, Json &doc, const uint8_t &spaceTab = 2);
    void prettyPrint(HardwareSerial &serial, JsonArray &v, const uint8_t &spaceTab = 2);
    String removeInsignificantZeros(const String &str);
    String toString(const float &value);
    String toString(const double &value);

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
    JsonArray &push(const JSType::Element &value);

    JSType::Element &operator[](const uint16_t &index);
    JSType::Element &getElement(const uint16_t &index);

    const JSType::Element &operator[](const uint16_t &index) const;
    const JSType::Element &getElement(const uint16_t &index) const;

    JSType::Type getType(const uint16_t &index) const;
    String getTypeString(const uint16_t &index) const;
    String toString() const;

    size_t size() const;
    bool contains(const JSType::Element &e) const;
    void erase(const uint16_t &index);
    void clear();
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
    Json &add(const String &name, const JSType::Element &value);

    JSType::Element &operator[](const String &name);
    JSType::Element &operator[](const uint16_t &index);
    JSType::Element &getElement(const String &name);
    JSType::Element &getElement(const uint16_t &index);

    const JSType::Element &operator[](const String &name) const;
    const JSType::Element &operator[](const uint16_t &index) const;
    const JSType::Element &getElement(const String &name) const;
    const JSType::Element &getElement(const uint16_t &index) const;

    JSType::Type getType(const String &name) const;
    JSType::Type getType(const uint16_t &index) const;
    String getTypeString(const String &name) const;
    String getTypeString(const uint16_t &index) const;
    String getKey(const uint16_t &index) const;

    int16_t getIndex(const String &name) const;
    String toString() const;

    size_t size() const;
    bool contains(String name) const;
    void erase(const String &name);
    void clear();
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
