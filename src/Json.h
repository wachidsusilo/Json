#ifndef JSON_H
#define JSON_H

#include <Arduino.h>
#include <iterator>
#include <vector>
#include <map>

class Json;
class JsonArray;

class JSType {
  public:
    enum Type {
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
        String value;
        Type type;

      public:
        Element(): type(undefined) {}
        Element(const String &value, const Type &type): value(value), type(type) {}
        Type getType() const;
        String getValue() const;
        void setType(const Type &type);
        void setValue(const String &value);
        bool operator ==(const Element &e);
    };
    
    struct Value {
      String value;
      operator int();
      operator long();
      operator float();
      operator double();
      operator String();
      operator void*();
      operator bool();
      operator Json();
      operator JsonArray();
    };
};

class JsonUtil {
  public:
    Json parse(const String &str);
    JsonArray parseArray(const String &str);
    bool isFloat(const String &str);
    String typeToString(const JSType::Type &type);
    String prettify(const String &str, const uint8_t &space = 0);
    String prettifyArray(const String &str, const uint8_t &space = 0);
};

extern JsonUtil JSUtil;

class JsonArray {
  public:
    JsonArray();
    JsonArray(const String &str);

    JsonArray push(const JSType::Element &e);
    JsonArray push(const String &value, const JSType::Type &type);
    JsonArray push(Json value);
    JsonArray push(JsonArray value);
    JsonArray push(const char* value);
    JsonArray push(const String &value);
    JsonArray push(const int &value);
    JsonArray push(const long &value);
    JsonArray push(const bool &value);
    JsonArray push(const void* value);
    JsonArray push(const float &value, const uint8_t &precision);
    JsonArray push(const double &value, const uint8_t &precision);

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
    Json(const String &str);

    Json add(const String &key, const JSType::Element &e);
    Json add(const String &key, const String &value, const JSType::Type &type);
    Json add(const String &name, Json value);
    Json add(const String &name, JsonArray value);
    Json add(const String &name, const char* value);
    Json add(const String &name, const String &value);
    Json add(const String &name, const int &value);
    Json add(const String &name, const long &value);
    Json add(const String &name, const bool &value);
    Json add(const String &name, const void* value);
    Json add(const String &name, const float &value, const uint8_t &precision);
    Json add(const String &name, const double &value, const uint8_t &precision);

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
    String getKey(const uint16_t &index);;
    int16_t getIndex(const String &name);
    String toString();

    size_t size() const;
    bool contains(String name) const;
    void erase(const String &name);
    std::map<String, JSType::Element>::iterator begin();
    std::map<String, JSType::Element>::iterator end();

  private:
    std::map<String, JSType::Element> doc;
    std::map<uint16_t, String> indexList;
    uint16_t counter;
    void arrangeIndex(uint16_t index);
};

#endif
