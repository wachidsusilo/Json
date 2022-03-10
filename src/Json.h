#ifndef JSON_H
#define JSON_H

#include <iterator>
#include <map>
#include <vector>

#include "Arduino.h"

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
        jsonNull
    };

    struct Element {
       private:
        Type type;
        mutable String *value;
        mutable Json *object;
        mutable JsonArray *array;
        void release();
        int compareTo(const Element &e) const;

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
        Element(const bool &);
        Element(const Json &);
        Element(const JsonArray &);

        operator int8_t() const;
        operator int16_t() const;
        operator int32_t() const;
        operator uint8_t() const;
        operator uint16_t() const;
        operator uint32_t() const;
        operator float() const;
        operator double() const;
        operator bool() const;
        operator String() const;
        explicit operator char*() const;
        operator Json &() const;
        operator JsonArray &() const;

        Element &operator=(const Element &);

        Element &operator[](const int8_t &index);
        Element &operator[](const int16_t &index);
        Element &operator[](const int32_t &index);
        Element &operator[](const uint8_t &index);
        Element &operator[](const uint16_t &index);
        Element &operator[](const uint32_t &index);
        Element &operator[](const String &key);
        Element &operator[](const char *key);

        const Element &operator[](const int8_t &index) const;
        const Element &operator[](const int16_t &index) const;
        const Element &operator[](const int32_t &index) const;
        const Element &operator[](const uint8_t &index) const;
        const Element &operator[](const uint16_t &index) const;
        const Element &operator[](const uint32_t &index) const;
        const Element &operator[](const String &key) const;
        const Element &operator[](const char *key) const;

        bool operator==(const Element &e) const;
        bool operator==(const int8_t &e) const;
        bool operator==(const int16_t &e) const;
        bool operator==(const int32_t &e) const;
        bool operator==(const uint8_t &e) const;
        bool operator==(const uint16_t &e) const;
        bool operator==(const uint32_t &e) const;
        bool operator==(const float &e) const;
        bool operator==(const double &e) const;
        bool operator==(const bool &e) const;
        bool operator==(const char *e) const;
        bool operator==(const String &e) const;

        bool operator!=(const Element &e) const;
        bool operator!=(const int8_t &e) const;
        bool operator!=(const int16_t &e) const;
        bool operator!=(const int32_t &e) const;
        bool operator!=(const uint8_t &e) const;
        bool operator!=(const uint16_t &e) const;
        bool operator!=(const uint32_t &e) const;
        bool operator!=(const float &e) const;
        bool operator!=(const double &e) const;
        bool operator!=(const bool &e) const;
        bool operator!=(const char *e) const;
        bool operator!=(const String &e) const;

        bool operator<(const Element &e) const;
        bool operator<(const int8_t &e) const;
        bool operator<(const int16_t &e) const;
        bool operator<(const int32_t &e) const;
        bool operator<(const uint8_t &e) const;
        bool operator<(const uint16_t &e) const;
        bool operator<(const uint32_t &e) const;
        bool operator<(const float &e) const;
        bool operator<(const double &e) const;
        bool operator<(const bool &e) const;
        bool operator<(const char *e) const;
        bool operator<(const String &e) const;

        bool operator>(const Element &e) const;
        bool operator>(const int8_t &e) const;
        bool operator>(const int16_t &e) const;
        bool operator>(const int32_t &e) const;
        bool operator>(const uint8_t &e) const;
        bool operator>(const uint16_t &e) const;
        bool operator>(const uint32_t &e) const;
        bool operator>(const float &e) const;
        bool operator>(const double &e) const;
        bool operator>(const bool &e) const;
        bool operator>(const char *e) const;
        bool operator>(const String &e) const;

        bool operator<=(const Element &e) const;
        bool operator<=(const int8_t &e) const;
        bool operator<=(const int16_t &e) const;
        bool operator<=(const int32_t &e) const;
        bool operator<=(const uint8_t &e) const;
        bool operator<=(const uint16_t &e) const;
        bool operator<=(const uint32_t &e) const;
        bool operator<=(const float &e) const;
        bool operator<=(const double &e) const;
        bool operator<=(const bool &e) const;
        bool operator<=(const char *e) const;
        bool operator<=(const String &e) const;

        bool operator>=(const Element &e) const;
        bool operator>=(const int8_t &e) const;
        bool operator>=(const int16_t &e) const;
        bool operator>=(const int32_t &e) const;
        bool operator>=(const uint8_t &e) const;
        bool operator>=(const uint16_t &e) const;
        bool operator>=(const uint32_t &e) const;
        bool operator>=(const float &e) const;
        bool operator>=(const double &e) const;
        bool operator>=(const bool &e) const;
        bool operator>=(const char *e) const;
        bool operator>=(const String &e) const;

        Element operator+(const Element &e) const;
        Element operator+(const int8_t &e) const;
        Element operator+(const int16_t &e) const;
        Element operator+(const int32_t &e) const;
        Element operator+(const uint8_t &e) const;
        Element operator+(const uint16_t &e) const;
        Element operator+(const uint32_t &e) const;
        Element operator+(const float &e) const;
        Element operator+(const double &e) const;
        Element operator+(const bool &e) const;
        Element operator+(const char *e) const;
        Element operator+(const String &e) const;

        Element operator-(const Element &e) const;
        Element operator-(const int8_t &e) const;
        Element operator-(const int16_t &e) const;
        Element operator-(const int32_t &e) const;
        Element operator-(const uint8_t &e) const;
        Element operator-(const uint16_t &e) const;
        Element operator-(const uint32_t &e) const;
        Element operator-(const float &e) const;
        Element operator-(const double &e) const;

        Element operator*(const Element &e) const;
        Element operator*(const int8_t &e) const;
        Element operator*(const int16_t &e) const;
        Element operator*(const int32_t &e) const;
        Element operator*(const uint8_t &e) const;
        Element operator*(const uint16_t &e) const;
        Element operator*(const uint32_t &e) const;
        Element operator*(const float &e) const;
        Element operator*(const double &e) const;

        Element operator/(const Element &e) const;
        Element operator/(const int8_t &e) const;
        Element operator/(const int16_t &e) const;
        Element operator/(const int32_t &e) const;
        Element operator/(const uint8_t &e) const;
        Element operator/(const uint16_t &e) const;
        Element operator/(const uint32_t &e) const;
        Element operator/(const float &e) const;
        Element operator/(const double &e) const;

        Element operator%(const Element &e) const;
        Element operator%(const int8_t &e) const;
        Element operator%(const int16_t &e) const;
        Element operator%(const int32_t &e) const;
        Element operator%(const uint8_t &e) const;
        Element operator%(const uint16_t &e) const;
        Element operator%(const uint32_t &e) const;
        Element operator%(const float &e) const;
        Element operator%(const double &e) const;

        Element &operator+=(const Element &e);
        Element &operator+=(const int8_t &e);
        Element &operator+=(const int16_t &e);
        Element &operator+=(const int32_t &e);
        Element &operator+=(const uint8_t &e);
        Element &operator+=(const uint16_t &e);
        Element &operator+=(const uint32_t &e);
        Element &operator+=(const float &e);
        Element &operator+=(const double &e);
        Element &operator+=(const bool &e);
        Element &operator+=(const char *e);
        Element &operator+=(const String &e);

        Element &operator-=(const Element &e);
        Element &operator-=(const int8_t &e);
        Element &operator-=(const int16_t &e);
        Element &operator-=(const int32_t &e);
        Element &operator-=(const uint8_t &e);
        Element &operator-=(const uint16_t &e);
        Element &operator-=(const uint32_t &e);
        Element &operator-=(const float &e);
        Element &operator-=(const double &e);

        Element &operator*=(const Element &e);
        Element &operator*=(const int8_t &e);
        Element &operator*=(const int16_t &e);
        Element &operator*=(const int32_t &e);
        Element &operator*=(const uint8_t &e);
        Element &operator*=(const uint16_t &e);
        Element &operator*=(const uint32_t &e);
        Element &operator*=(const float &e);
        Element &operator*=(const double &e);

        Element &operator/=(const Element &e);
        Element &operator/=(const int8_t &e);
        Element &operator/=(const int16_t &e);
        Element &operator/=(const int32_t &e);
        Element &operator/=(const uint8_t &e);
        Element &operator/=(const uint16_t &e);
        Element &operator/=(const uint32_t &e);
        Element &operator/=(const float &e);
        Element &operator/=(const double &e);

        Element &operator%=(const Element &e);
        Element &operator%=(const int8_t &e);
        Element &operator%=(const int16_t &e);
        Element &operator%=(const int32_t &e);
        Element &operator%=(const uint8_t &e);
        Element &operator%=(const uint16_t &e);
        Element &operator%=(const uint32_t &e);
        Element &operator%=(const float &e);
        Element &operator%=(const double &e);

        Element &operator++();
        Element operator++(int);
        Element &operator--();
        Element operator--(int);

        int32_t toInt() const;
        uint32_t toUInt() const;
        float toFloat() const;
        double toDouble() const;
        bool toBool() const;
        String toString() const;
        Json &toJson() const;
        JsonArray &toArray() const;
        char* c_str() const;

        template <typename T>
        T as() { return *this; }

        template <typename T>
        const T as() const { return *this; }

        Type getType() const;
        String getTypeName() const;
        size_t size() const;
        size_t lastIndex() const;

        void remove(const size_t &index, const size_t &count = 1);
        void remove(const String &key);
        void clear();

        bool contains(const JSType::Element &e) const;
        bool isNull() const;
        bool isEmpty() const;
        bool isNotEmpty() const;
    };
};

class JsonUtil {
   public:
    enum class LogLevel {
        Error,
        Info,
        Warning
    };

    void parse(const char *str, Json *doc, JsonArray *v);
    bool isFloat(const String &str);
    String typeToString(const JSType::Type &type);
    void prettyPrint(HardwareSerial &serial, Json &doc, const uint8_t &spaceTab = 2);
    void prettyPrint(HardwareSerial &serial, JsonArray &v, const uint8_t &spaceTab = 2);
    String removeInsignificantZeros(const String &str);
    String toString(const float &value);
    String toString(const double &value);
    void remove(String &str, const int &len, const bool &fromEnd = false);
    void attachDebugger(HardwareSerial &serial);
    void detachDebugger();
    void log(const LogLevel &level, const String &tag, const String &message);

   private:
    HardwareSerial *serial;
    void prettyPrint(HardwareSerial &serial, Json *doc, JsonArray *v, const uint8_t &spaceTab, const uint8_t &space);
};

extern JsonUtil JSUtil;

class JsonArray {
   public:
    JsonArray();
    JsonArray(const String &str);
    JsonArray(const JsonArray &other);
    JsonArray(std::vector<int> &arr);
    explicit JsonArray(const char *str);

    JsonArray &push(const JSType::Element &value);
    JsonArray &push();

    JSType::Element &operator[](const uint16_t &index);
    JSType::Element &getElement(const uint16_t &index);

    const JSType::Element &operator[](const uint16_t &index) const;
    const JSType::Element &getElement(const uint16_t &index) const;

    bool operator==(const JsonArray &other) const;

    JSType::Type getType(const uint16_t &index) const;
    String getTypeName(const uint16_t &index) const;
    String toString() const;
    size_t size() const;
    size_t lastIndex() const;

    bool contains(const JSType::Element &e) const;
    bool isEmpty() const;
    bool isNotEmpty() const;

    void remove(const size_t &index, const size_t &count = 1);
    void clear();

    std::vector<JSType::Element>::iterator begin();
    std::vector<JSType::Element>::iterator end();

   private:
    std::vector<JSType::Element> v;
};

class Json {
   public:
    Json();
    Json(const String &str);
    Json(const Json &other);
    explicit Json(const char *str);

    Json &add(const String &name, const JSType::Element &value);
    Json &add(const String &name);

    JSType::Element &operator[](const String &name);
    JSType::Element &operator[](const uint16_t &index);
    JSType::Element &getElement(const String &name);
    JSType::Element &getElement(const uint16_t &index);

    const JSType::Element &operator[](const String &name) const;
    const JSType::Element &operator[](const uint16_t &index) const;
    const JSType::Element &getElement(const String &name) const;
    const JSType::Element &getElement(const uint16_t &index) const;

    bool operator==(const Json &other) const;

    JSType::Type getType(const String &name) const;
    JSType::Type getType(const uint16_t &index) const;
    String getTypeName(const String &name) const;
    String getTypeName(const uint16_t &index) const;
    String getKey(const uint16_t &index) const;

    int16_t indexOf(const String &name) const;
    String toString() const;
    size_t size() const;

    bool contains(const String &e) const;
    bool isEmpty() const;
    bool isNotEmpty() const;
    
    void remove(const String &name);
    void clear();

    std::map<String, JSType::Element>::iterator begin();
    std::map<String, JSType::Element>::iterator end();

   private:
    std::map<String, JSType::Element> doc;
    std::map<uint16_t, String> indexList;
    uint16_t counter;
    void arrangeIndex(uint16_t index);
};

#endif
