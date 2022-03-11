#ifndef JSON_H
#define JSON_H

#include <Printable.h>

#include <iterator>
#include <vector>

#include "Arduino.h"

class Json;
class JsonArray;

class Element : public Printable {
   public:
    enum Type : uint8_t {
        jsonObject,
        jsonArray,
        jsonNumber,
        jsonString,
        jsonBoolean,
        jsonNull
    };

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
    explicit operator char *() const;
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
    char *c_str() const;

    Type getType() const;
    String getTypeName() const;
    size_t size() const;
    size_t lastIndex() const;

    void remove(const size_t &index, const size_t &count = 1);
    void remove(const String &key);
    void clear();

    bool contains(const Element &e) const;
    bool isNull() const;
    bool isNotNull() const;
    bool isEmpty() const;
    bool isNotEmpty() const;

    bool isObject() const;
    bool isArray() const;
    bool isString() const;
    bool isNumber() const;
    bool isBool() const;

    size_t printTo(Print &p) const;

    std::vector<Element>::iterator begin();
    std::vector<Element>::iterator end();

    template <typename T>
    T as() { return *this; }

    template <typename T>
    const T as() const { return *this; }

   private:
    Type type;
    mutable String *value;
    mutable Json *object;
    mutable JsonArray *array;
    void release();
    int compareTo(const Element &e) const;
};

class JsonArray : public Printable {
   public:
    JsonArray();
    JsonArray(const String &str);
    JsonArray(const JsonArray &other);
    JsonArray(std::vector<int> &arr);
    explicit JsonArray(const char *str);

    JsonArray &push(const Element &value);
    JsonArray &push();

    Element &operator[](const uint16_t &index);
    const Element &operator[](const uint16_t &index) const;

    Element &getElement(const uint16_t &index);
    const Element &getElement(const uint16_t &index) const;

    bool operator==(const JsonArray &other) const;
    bool operator!=(const JsonArray &other) const;

    Element::Type getType(const uint16_t &index) const;
    String getTypeName(const uint16_t &index) const;
    String toString() const;
    size_t size() const;
    size_t lastIndex() const;

    bool contains(const Element &e) const;
    bool isEmpty() const;
    bool isNotEmpty() const;

    void remove(const size_t &index, const size_t &count = 1);
    void clear();

    size_t printTo(Print &p) const;

    std::vector<Element>::iterator begin();
    std::vector<Element>::iterator end();

   private:
    std::vector<Element> v;
};

class Json : public Printable {
   public:
    struct Pair {
        String key;
        Element value;

        Pair(const String &key, const Element &value);

        bool operator==(const Pair &other) const;
        bool operator!=(const Pair &other) const;
    };

    Json();
    Json(const String &str);
    Json(const Json &other);
    explicit Json(const char *str);

    Json &add(const String &name, const Element &value);
    Json &add(const String &name);

    Element &operator[](const String &name);
    Element &operator[](const uint16_t &index);
    const Element &operator[](const String &name) const;
    const Element &operator[](const uint16_t &index) const;

    Element &getElement(const String &name);
    Element &getElement(const uint16_t &index);
    const Element &getElement(const String &name) const;
    const Element &getElement(const uint16_t &index) const;

    bool operator==(const Json &other) const;
    bool operator!=(const Json &other) const;

    Element::Type getType(const String &name) const;
    Element::Type getType(const uint16_t &index) const;
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

    size_t printTo(Print &p) const;

    std::vector<Pair>::iterator begin();
    std::vector<Pair>::iterator end();

   private:
    std::vector<Pair> doc;
};

class JSUtil {
   public:
    enum LogLevel {
        Error,
        Info,
        Warning
    };

    static void parse(const char *str, Json *doc, JsonArray *v);
    static bool isFloat(const String &str);
    static String typeToString(const Element::Type &type);
    static void prettyPrint(HardwareSerial &serial, Json &doc, const uint8_t &spaceTab = 2);
    static void prettyPrint(HardwareSerial &serial, JsonArray &v, const uint8_t &spaceTab = 2);
    static String removeInsignificantZeros(const String &str);
    static String toString(const float &value);
    static String toString(const double &value);
    static void remove(String &str, const int &len, const bool &fromEnd = false);
    static void attachDebugger(HardwareSerial &hwSerial);
    static void detachDebugger();
    static void log(const LogLevel &level, const String &tag, const String &message);

   private:
    static HardwareSerial *serial;
    static void prettyPrint(HardwareSerial &serial, Json *doc, JsonArray *v, const uint8_t &spaceTab, const uint8_t &space);
};

#endif
