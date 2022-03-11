#include "Json.h"

static Element _emptyElement;

/**--- Element ---**/

Element::Element()
    : value(new String("null")), type(jsonNull), object(NULL), array(NULL) {}

Element::~Element() {
    release();
}

Element::Element(const String &value, const Type &type)
    : value(new String(value)), type(type), object(NULL), array(NULL) {}

Element::Element(const Element &e)
    : value(NULL), type(e.type), object(NULL), array(NULL) {
    type = e.type;
    switch (type) {
        case jsonObject:
            if (e.object) {
                object = new Json(*e.object);
            } else if (e.value) {
                value = new String(*e.value);
            } else {
                object = new Json();
            }
            break;
        case jsonArray:
            if (e.array) {
                array = new JsonArray(*e.array);
            } else if (e.value) {
                value = new String(*e.value);
            } else {
                array = new JsonArray();
            }
            break;
        default:
            value = new String(*e.value);
            break;
    }
}

Element::Element(const int8_t &value)
    : value(new String(value)), type(jsonNumber), object(NULL), array(NULL) {}

Element::Element(const int16_t &value)
    : value(new String(value)), type(jsonNumber), object(NULL), array(NULL) {}

Element::Element(const int32_t &value)
    : value(new String(value)), type(jsonNumber), object(NULL), array(NULL) {}

Element::Element(const uint8_t &value)
    : value(new String(value)), type(jsonNumber), object(NULL), array(NULL) {}

Element::Element(const uint16_t &value)
    : value(new String(value)), type(jsonNumber), object(NULL), array(NULL) {}

Element::Element(const uint32_t &value)
    : value(new String(value)), type(jsonNumber), object(NULL), array(NULL) {}

Element::Element(const float &value)
    : value(new String(JSUtil::toString(value))), type(jsonNumber), object(NULL), array(NULL) {}

Element::Element(const double &value)
    : value(new String(JSUtil::toString(value))), type(jsonNumber), object(NULL), array(NULL) {}

Element::Element(const String &value)
    : value(new String(value)), type(jsonString), object(NULL), array(NULL) {}

Element::Element(const char *value)
    : value(new String(value)), type(jsonString), object(NULL), array(NULL) {}

Element::Element(const bool &value)
    : value(new String(value ? "true" : "false")), type(jsonBoolean), object(NULL), array(NULL) {}

Element::Element(const Json &value)
    : value(NULL), type(jsonObject), object(new Json(value)), array(NULL) {}

Element::Element(const JsonArray &value)
    : value(NULL), type(jsonArray), object(NULL), array(new JsonArray(value)) {}

Element::operator int8_t() const {
    if (!value) return 0;
    return round((*value).toDouble());
}

Element::operator int16_t() const {
    if (!value) return 0;
    return round((*value).toDouble());
}

Element::operator int32_t() const {
    if (!value) return 0;
    return round((*value).toDouble());
}

Element::operator uint8_t() const {
    if (!value) return 0;
    return round((*value).toDouble());
}

Element::operator uint16_t() const {
    if (!value) return 0;
    return round((*value).toDouble());
}

Element::operator uint32_t() const {
    if (!value) return 0;
    return round((*value).toDouble());
}

Element::operator float() const {
    if (!value) return 0;
    return (*value).toFloat();
}

Element::operator double() const {
    if (!value) return 0;
    return (*value).toDouble();
}

Element::operator bool() const {
    switch (type) {
        case jsonObject:
            return value != NULL || object != NULL;
        case jsonArray:
            return value != NULL || array != NULL;
        case jsonString:
            return value != NULL && !value->isEmpty();
        case jsonNumber:
            return as<double>() != 0;
        case jsonBoolean:
            return value != NULL && *value == "true";
        case jsonNull:
            return value != NULL && *value == "null";
        default:
            return false;
    }
}

Element::operator String() const {
    switch (type) {
        case jsonObject:
            if (value) {
                return *value;
            } else if (object) {
                return object->toString();
            } else {
                return "";
            }
        case jsonArray:
            if (value) {
                return *value;
            } else if (array) {
                return array->toString();
            } else {
                return "";
            }
        default:
            return *value;
    }
}

Element::operator char *() const {
    switch (type) {
        case jsonObject:
            if (value) {
                return const_cast<char *>(value->c_str());
            } else if (object) {
                value = new String(object->toString());
                return const_cast<char *>(value->c_str());
            } else {
                return NULL;
            }
        case jsonArray:
            if (value) {
                return const_cast<char *>(value->c_str());
            } else if (array) {
                value = new String(array->toString());
                return const_cast<char *>(value->c_str());
            } else {
                return NULL;
            }
        default:
            if (value) {
                return const_cast<char *>(value->c_str());
            } else {
                return NULL;
            }
    }
}

Element::operator Json &() const {
    static Json emptyJson;
    if (!object) {
        if (value && type == jsonObject) {
            object = new Json(*value);
            delete value;
            value = NULL;
        } else {
            emptyJson.clear();
            return emptyJson;
        }
    }
    return *object;
}

Element::operator JsonArray &() const {
    static JsonArray emptyJsonArray;
    if (!array) {
        if (value && type == jsonArray) {
            array = new JsonArray(*value);
            delete value;
            value = NULL;
        } else {
            emptyJsonArray.clear();
            return emptyJsonArray;
        }
    }
    return *array;
}

Element &Element::operator=(const Element &e) {
    if (this == &e) return *this;
    release();
    type = e.getType();
    switch (type) {
        case jsonObject:
            if (e.object) {
                object = new Json(*e.object);
            } else if (e.value) {
                value = new String(*e.value);
            } else {
                object = new Json();
            }
            break;
        case jsonArray:
            if (e.array) {
                array = new JsonArray(*e.array);
            } else if (e.value) {
                value = new String(*e.value);
            } else {
                array = new JsonArray();
            }
            break;
        default:
            value = new String(*e.value);
            break;
    }
    return *this;
}

Element &Element::operator[](const int8_t &index) {
    return operator[]((uint32_t)index);
}

Element &Element::operator[](const int16_t &index) {
    return operator[]((uint32_t)index);
}

Element &Element::operator[](const int32_t &index) {
    return operator[]((uint32_t)index);
}

Element &Element::operator[](const uint8_t &index) {
    return operator[]((uint32_t)index);
}

Element &Element::operator[](const uint16_t &index) {
    return operator[]((uint32_t)index);
}

Element &Element::operator[](const uint32_t &index) {
    if (type != jsonArray) {
        _emptyElement = Element();
        return _emptyElement;
    }
    return as<JsonArray &>()[index];
}

Element &Element::operator[](const char *key) {
    return operator[](String(key));
}

Element &Element::operator[](const String &key) {
    if (type != jsonObject) {
        _emptyElement = Element();
        return _emptyElement;
    }
    return as<Json &>()[key];
}

const Element &Element::operator[](const int8_t &index) const {
    return operator[]((uint32_t)index);
}

const Element &Element::operator[](const int16_t &index) const {
    return operator[]((uint32_t)index);
}

const Element &Element::operator[](const int32_t &index) const {
    return operator[]((uint32_t)index);
}

const Element &Element::operator[](const uint8_t &index) const {
    return operator[]((uint32_t)index);
}

const Element &Element::operator[](const uint16_t &index) const {
    return operator[]((uint32_t)index);
}

const Element &Element::operator[](const uint32_t &index) const {
    if (type != jsonArray) {
        _emptyElement = Element();
        return _emptyElement;
    }
    return as<JsonArray &>()[index];
}

const Element &Element::operator[](const char *key) const {
    return operator[](String(key));
}

const Element &Element::operator[](const String &key) const {
    if (type != jsonObject) {
        _emptyElement = Element();
        return _emptyElement;
    }
    return as<Json &>()[key];
}

bool Element::operator==(const Element &e) const {
    switch (type) {
        case jsonObject:
            if (e.type != jsonObject) {
                return false;
            }
            if ((value || object) && (e.value || e.object)) {
                return as<Json &>() == e.as<Json &>();
            } else {
                return false;
            }
        case jsonArray:
            if (e.type != jsonArray) {
                return false;
            }
            if ((value || array) && (e.value || e.array)) {
                return as<JsonArray &>() == e.as<JsonArray &>();
            } else {
                return false;
            }
        case jsonString:
            return value->equals(*e.value);
        case jsonNumber:
            if (e.type == jsonNumber) {
                return as<double>() == e.as<double>();
            }
            return false;
        case jsonBoolean:
            return as<bool>() == e.as<bool>();
        case jsonNull:
            return e.type == jsonNull;
        default:
            return false;
    }
}

bool Element::operator==(const int8_t &e) const {
    if (type == jsonNumber) {
        return as<int8_t>() == e;
    }
    return false;
}

bool Element::operator==(const int16_t &e) const {
    if (type == jsonNumber) {
        return as<int16_t>() == e;
    }
    return false;
}

bool Element::operator==(const int32_t &e) const {
    if (type == jsonNumber) {
        return as<int32_t>() == e;
    }
    return false;
}

bool Element::operator==(const uint8_t &e) const {
    if (type == jsonNumber) {
        return as<uint8_t>() == e;
    }
    return false;
}

bool Element::operator==(const uint16_t &e) const {
    if (type == jsonNumber) {
        return as<uint16_t>() == e;
    }
    return false;
}

bool Element::operator==(const uint32_t &e) const {
    if (type == jsonNumber) {
        return as<uint32_t>() == e;
    }
    return false;
}

bool Element::operator==(const float &e) const {
    if (type == jsonNumber) {
        return as<float>() == e;
    }
    return false;
}

bool Element::operator==(const double &e) const {
    if (type == jsonNumber) {
        return as<double>() == e;
    }
    return false;
}

bool Element::operator==(const bool &e) const {
    return as<bool>() == e;
}

bool Element::operator==(const char *e) const {
    if (type == jsonString) {
        return as<String>() == e;
    }
    return false;
}

bool Element::operator==(const String &e) const {
    if (type == jsonString) {
        return as<String>() == e;
    }
    return false;
}

bool Element::operator!=(const Element &e) const {
    return !operator==(e);
}

bool Element::operator!=(const int8_t &e) const {
    return !operator==(e);
}

bool Element::operator!=(const int16_t &e) const {
    return !operator==(e);
}

bool Element::operator!=(const int32_t &e) const {
    return !operator==(e);
}

bool Element::operator!=(const uint8_t &e) const {
    return !operator==(e);
}

bool Element::operator!=(const uint16_t &e) const {
    return !operator==(e);
}

bool Element::operator!=(const uint32_t &e) const {
    return !operator==(e);
}

bool Element::operator!=(const float &e) const {
    return !operator==(e);
}

bool Element::operator!=(const double &e) const {
    return !operator==(e);
}

bool Element::operator!=(const bool &e) const {
    return !operator==(e);
}

bool Element::operator!=(const char *e) const {
    return !operator==(e);
}

bool Element::operator!=(const String &e) const {
    return !operator==(e);
}

bool Element::operator<(const Element &e) const {
    switch (type) {
        case jsonObject:
        case jsonArray:
        case jsonNull:
            return false;
    }
    switch (e.type) {
        case jsonObject:
        case jsonArray:
        case jsonNull:
            return false;
    }
    return compareTo(e) < 0;
}

bool Element::operator<(const int8_t &e) const {
    return compareTo(e) < 0;
}

bool Element::operator<(const int16_t &e) const {
    return compareTo(e) < 0;
}

bool Element::operator<(const int32_t &e) const {
    return compareTo(e) < 0;
}

bool Element::operator<(const uint8_t &e) const {
    return compareTo(e) < 0;
}

bool Element::operator<(const uint16_t &e) const {
    return compareTo(e) < 0;
}

bool Element::operator<(const uint32_t &e) const {
    return compareTo(e) < 0;
}

bool Element::operator<(const float &e) const {
    return compareTo(e) < 0;
}

bool Element::operator<(const double &e) const {
    return compareTo(e) < 0;
}

bool Element::operator<(const bool &e) const {
    return compareTo(e) < 0;
}

bool Element::operator<(const char *e) const {
    return compareTo(e) < 0;
}

bool Element::operator<(const String &e) const {
    return compareTo(e) < 0;
}

bool Element::operator>(const Element &e) const {
    return compareTo(e) > 0;
}

bool Element::operator>(const int8_t &e) const {
    return compareTo(e) > 0;
}

bool Element::operator>(const int16_t &e) const {
    return compareTo(e) > 0;
}

bool Element::operator>(const int32_t &e) const {
    return compareTo(e) > 0;
}

bool Element::operator>(const uint8_t &e) const {
    return compareTo(e) > 0;
}

bool Element::operator>(const uint16_t &e) const {
    return compareTo(e) > 0;
}

bool Element::operator>(const uint32_t &e) const {
    return compareTo(e) > 0;
}

bool Element::operator>(const float &e) const {
    return compareTo(e) > 0;
}

bool Element::operator>(const double &e) const {
    return compareTo(e) > 0;
}

bool Element::operator>(const bool &e) const {
    return compareTo(e) > 0;
}

bool Element::operator>(const char *e) const {
    return compareTo(e) > 0;
}

bool Element::operator>(const String &e) const {
    return compareTo(e) > 0;
}

bool Element::operator<=(const Element &e) const {
    return compareTo(e) <= 0;
}

bool Element::operator<=(const int8_t &e) const {
    return compareTo(e) <= 0;
}

bool Element::operator<=(const int16_t &e) const {
    return compareTo(e) <= 0;
}

bool Element::operator<=(const int32_t &e) const {
    return compareTo(e) <= 0;
}

bool Element::operator<=(const uint8_t &e) const {
    return compareTo(e) <= 0;
}

bool Element::operator<=(const uint16_t &e) const {
    return compareTo(e) <= 0;
}

bool Element::operator<=(const uint32_t &e) const {
    return compareTo(e) <= 0;
}

bool Element::operator<=(const float &e) const {
    return compareTo(e) <= 0;
}

bool Element::operator<=(const double &e) const {
    return compareTo(e) <= 0;
}

bool Element::operator<=(const bool &e) const {
    return compareTo(e) <= 0;
}

bool Element::operator<=(const char *e) const {
    return compareTo(e) <= 0;
}

bool Element::operator<=(const String &e) const {
    return compareTo(e) <= 0;
}

bool Element::operator>=(const Element &e) const {
    return compareTo(e) >= 0;
}

bool Element::operator>=(const int8_t &e) const {
    return compareTo(e) >= 0;
}

bool Element::operator>=(const int16_t &e) const {
    return compareTo(e) >= 0;
}

bool Element::operator>=(const int32_t &e) const {
    return compareTo(e) >= 0;
}

bool Element::operator>=(const uint8_t &e) const {
    return compareTo(e) >= 0;
}

bool Element::operator>=(const uint16_t &e) const {
    return compareTo(e) >= 0;
}

bool Element::operator>=(const uint32_t &e) const {
    return compareTo(e) >= 0;
}

bool Element::operator>=(const float &e) const {
    return compareTo(e) >= 0;
}

bool Element::operator>=(const double &e) const {
    return compareTo(e) >= 0;
}

bool Element::operator>=(const bool &e) const {
    return compareTo(e) >= 0;
}

bool Element::operator>=(const char *e) const {
    return compareTo(e) >= 0;
}

bool Element::operator>=(const String &e) const {
    return compareTo(e) >= 0;
}

Element Element::operator+(const Element &e) const {
    if (value == NULL || e.value == NULL) return *this;
    switch (type) {
        case jsonNumber: {
            switch (e.type) {
                case jsonNumber:
                    return as<double>() + e.as<double>();
                case jsonString:
                    return as<String>() + e.as<String>();
                default:
                    return *this;
            }
        }
        case jsonString: {
            switch (e.type) {
                case jsonNumber:
                case jsonString:
                    return as<String>() + e.as<String>();
                case jsonBoolean:
                    return as<String>() + String(e.as<bool>() ? "true" : "false");
                default:
                    return *this;
            }
        }
        case jsonBoolean: {
            switch (e.type) {
                case jsonString:
                    return String(as<bool>() ? "true" : "false") + e.as<String>();
                default:
                    return *this;
            }
        }
        default: {
            return *this;
        }
    }
}

Element Element::operator+(const int8_t &e) const {
    return operator+((double)e);
}

Element Element::operator+(const int16_t &e) const {
    return operator+((double)e);
}

Element Element::operator+(const int32_t &e) const {
    return operator+((double)e);
}

Element Element::operator+(const uint8_t &e) const {
    return operator+((double)e);
}

Element Element::operator+(const uint16_t &e) const {
    return operator+((double)e);
}

Element Element::operator+(const uint32_t &e) const {
    return operator+((double)e);
}

Element Element::operator+(const float &e) const {
    return operator+((double)e);
}

Element Element::operator+(const double &e) const {
    if (type == jsonNumber) {
        return as<double>() + e;
    }
    return *this;
}

Element Element::operator+(const bool &e) const {
    return operator+(String(e ? "true" : "false"));
}

Element Element::operator+(const char *e) const {
    return operator+(String(e));
}

Element Element::operator+(const String &e) const {
    if (type == jsonString) {
        return as<String>() + e;
    }
    return *this;
}

Element Element::operator-(const Element &e) const {
    if (value == NULL || e.value == NULL) return *this;
    if (type == jsonNumber && e.type == jsonNumber) {
        return as<double>() - e.as<double>();
    }
    return *this;
}

Element Element::operator-(const int8_t &e) const {
    return operator-((double)e);
}

Element Element::operator-(const int16_t &e) const {
    return operator-((double)e);
}

Element Element::operator-(const int32_t &e) const {
    return operator-((double)e);
}

Element Element::operator-(const uint8_t &e) const {
    return operator-((double)e);
}

Element Element::operator-(const uint16_t &e) const {
    return operator-((double)e);
}

Element Element::operator-(const uint32_t &e) const {
    return operator-((double)e);
}

Element Element::operator-(const float &e) const {
    return operator-((double)e);
}

Element Element::operator-(const double &e) const {
    if (type == jsonNumber) {
        return as<double>() - e;
    }
    return *this;
}

Element Element::operator*(const Element &e) const {
    if (value == NULL || e.value == NULL) return *this;
    if (type == jsonNumber && e.type == jsonNumber) {
        return as<double>() * e.as<double>();
    }
    return *this;
}

Element Element::operator*(const int8_t &e) const {
    return operator*((double)e);
}

Element Element::operator*(const int16_t &e) const {
    return operator*((double)e);
}

Element Element::operator*(const int32_t &e) const {
    return operator*((double)e);
}

Element Element::operator*(const uint8_t &e) const {
    return operator*((double)e);
}

Element Element::operator*(const uint16_t &e) const {
    return operator*((double)e);
}

Element Element::operator*(const uint32_t &e) const {
    return operator*((double)e);
}

Element Element::operator*(const float &e) const {
    return operator*((double)e);
}

Element Element::operator*(const double &e) const {
    if (type == jsonNumber) {
        return as<double>() * e;
    }
    return *this;
}

Element Element::operator/(const Element &e) const {
    if (value == NULL || e.value == NULL) return *this;
    if (type == jsonNumber && e.type == jsonNumber) {
        return as<double>() / e.as<double>();
    }
    return *this;
}

Element Element::operator/(const int8_t &e) const {
    return operator/((double)e);
}

Element Element::operator/(const int16_t &e) const {
    return operator/((double)e);
}

Element Element::operator/(const int32_t &e) const {
    return operator/((double)e);
}

Element Element::operator/(const uint8_t &e) const {
    return operator/((double)e);
}

Element Element::operator/(const uint16_t &e) const {
    return operator/((double)e);
}

Element Element::operator/(const uint32_t &e) const {
    return operator/((double)e);
}

Element Element::operator/(const float &e) const {
    return operator/((double)e);
}

Element Element::operator/(const double &e) const {
    if (type == jsonNumber) {
        return as<double>() / e;
    }
    return *this;
}

Element Element::operator%(const Element &e) const {
    if (value == NULL || e.value == NULL) return *this;
    if (type == jsonNumber && e.type == jsonNumber) {
        return fmod(as<double>(), e.as<double>());
    }
    return *this;
}

Element Element::operator%(const int8_t &e) const {
    return operator%((double)e);
}

Element Element::operator%(const int16_t &e) const {
    return operator%((double)e);
}

Element Element::operator%(const int32_t &e) const {
    return operator%((double)e);
}

Element Element::operator%(const uint8_t &e) const {
    return operator%((double)e);
}

Element Element::operator%(const uint16_t &e) const {
    return operator%((double)e);
}

Element Element::operator%(const uint32_t &e) const {
    return operator%((double)e);
}

Element Element::operator%(const float &e) const {
    return operator%((double)e);
}

Element Element::operator%(const double &e) const {
    if (type == jsonNumber) {
        return fmod(as<double>(), e);
    }
    return *this;
}

Element &Element::operator+=(const Element &e) {
    if (value == NULL || e.value == NULL) return *this;
    switch (type) {
        case jsonNumber: {
            if (e.type == jsonNumber) {
                *value = JSUtil::toString(as<double>() + e.as<double>());
            }
            break;
        }
        case jsonString: {
            switch (e.type) {
                case jsonNumber:
                case jsonString:
                    *value += *e.value;
                    break;
                case jsonBoolean:
                    *value += e.as<bool>() ? "true" : "false";
                    break;
            }
            break;
        }
    }
    return *this;
}

Element &Element::operator+=(const int8_t &e) {
    return operator+=((double)e);
}

Element &Element::operator+=(const int16_t &e) {
    return operator+=((double)e);
}

Element &Element::operator+=(const int32_t &e) {
    return operator+=((double)e);
}

Element &Element::operator+=(const uint8_t &e) {
    return operator+=((double)e);
}

Element &Element::operator+=(const uint16_t &e) {
    return operator+=((double)e);
}

Element &Element::operator+=(const uint32_t &e) {
    return operator+=((double)e);
}

Element &Element::operator+=(const float &e) {
    return operator+=((double)e);
}

Element &Element::operator+=(const double &e) {
    if (type == jsonNumber) {
        *value = JSUtil::toString(as<double>() + e);
    } else if (type == jsonString) {
        *value += JSUtil::toString(e);
    }
    return *this;
}

Element &Element::operator+=(const bool &e) {
    return operator+=(String(e ? "true" : "false"));
}

Element &Element::operator+=(const char *e) {
    return operator+=(String(e));
}

Element &Element::operator+=(const String &e) {
    if (type == jsonString) {
        *value += e;
    }
    return *this;
}

Element &Element::operator-=(const Element &e) {
    if (value == NULL || e.value == NULL) return *this;
    if (type == jsonNumber && e.type == jsonNumber) {
        *value = JSUtil::toString(as<double>() - e.as<double>());
    }
    return *this;
}

Element &Element::operator-=(const int8_t &e) {
    return operator-=((double)e);
}

Element &Element::operator-=(const int16_t &e) {
    return operator-=((double)e);
}

Element &Element::operator-=(const int32_t &e) {
    return operator-=((double)e);
}

Element &Element::operator-=(const uint8_t &e) {
    return operator-=((double)e);
}

Element &Element::operator-=(const uint16_t &e) {
    return operator-=((double)e);
}

Element &Element::operator-=(const uint32_t &e) {
    return operator-=((double)e);
}

Element &Element::operator-=(const float &e) {
    return operator-=((double)e);
}

Element &Element::operator-=(const double &e) {
    if (type == jsonNumber) {
        *value = JSUtil::toString(as<double>() - e);
    }
    return *this;
}

Element &Element::operator*=(const Element &e) {
    if (value == NULL || e.value == NULL) return *this;
    if (type == jsonNumber && e.type == jsonNumber) {
        *value = JSUtil::toString(as<double>() * e.as<double>());
    }
    return *this;
}

Element &Element::operator*=(const int8_t &e) {
    return operator*=((double)e);
}

Element &Element::operator*=(const int16_t &e) {
    return operator*=((double)e);
}

Element &Element::operator*=(const int32_t &e) {
    return operator*=((double)e);
}

Element &Element::operator*=(const uint8_t &e) {
    return operator*=((double)e);
}

Element &Element::operator*=(const uint16_t &e) {
    return operator*=((double)e);
}

Element &Element::operator*=(const uint32_t &e) {
    return operator*=((double)e);
}

Element &Element::operator*=(const float &e) {
    return operator*=((double)e);
}

Element &Element::operator*=(const double &e) {
    if (type == jsonNumber) {
        *value = JSUtil::toString(as<double>() * e);
    }
    return *this;
}

Element &Element::operator/=(const Element &e) {
    if (value == NULL || e.value == NULL) return *this;
    if (type == jsonNumber && e.type == jsonNumber) {
        *value = JSUtil::toString(as<double>() / e.as<double>());
    }
    return *this;
}

Element &Element::operator/=(const int8_t &e) {
    return operator/=((double)e);
}

Element &Element::operator/=(const int16_t &e) {
    return operator/=((double)e);
}

Element &Element::operator/=(const int32_t &e) {
    return operator/=((double)e);
}

Element &Element::operator/=(const uint8_t &e) {
    return operator/=((double)e);
}

Element &Element::operator/=(const uint16_t &e) {
    return operator/=((double)e);
}

Element &Element::operator/=(const uint32_t &e) {
    return operator/=((double)e);
}

Element &Element::operator/=(const float &e) {
    return operator/=((double)e);
}

Element &Element::operator/=(const double &e) {
    if (type == jsonNumber) {
        *value = JSUtil::toString(as<double>() / e);
    }
    return *this;
}

Element &Element::operator%=(const Element &e) {
    if (value == NULL || e.value == NULL) return *this;
    if (type == jsonNumber) {
        *value = JSUtil::toString(fmod(as<double>(), e.as<double>()));
    }
    return *this;
}

Element &Element::operator%=(const int8_t &e) {
    return operator%=((double)e);
}

Element &Element::operator%=(const int16_t &e) {
    return operator%=((double)e);
}

Element &Element::operator%=(const int32_t &e) {
    return operator%=((double)e);
}

Element &Element::operator%=(const uint8_t &e) {
    return operator%=((double)e);
}

Element &Element::operator%=(const uint16_t &e) {
    return operator%=((double)e);
}

Element &Element::operator%=(const uint32_t &e) {
    return operator%=((double)e);
}

Element &Element::operator%=(const float &e) {
    return operator%=((double)e);
}

Element &Element::operator%=(const double &e) {
    if (type == jsonNumber) {
        *value = JSUtil::toString(fmod(as<double>(), e));
    }
    return *this;
}

Element &Element::operator++() {
    if (type == jsonNumber) {
        *value = JSUtil::toString(as<double>() + 1);
    }
    return *this;
}

Element Element::operator++(int) {
    Element e = *this;
    if (type == jsonNumber) {
        *value = JSUtil::toString(as<double>() + 1);
    }
    return e;
}

Element &Element::operator--() {
    if (type == jsonNumber) {
        *value = JSUtil::toString(as<double>() - 1);
    }
    return *this;
}

Element Element::operator--(int) {
    Element e = *this;
    if (type == jsonNumber) {
        *value = JSUtil::toString(as<double>() - 1);
    }
    return e;
}

int32_t Element::toInt() const {
    return *this;
}

uint32_t Element::toUInt() const {
    return *this;
}

float Element::toFloat() const {
    return *this;
}

double Element::toDouble() const {
    return *this;
}

bool Element::toBool() const {
    return *this;
}

String Element::toString() const {
    return *this;
}

Json &Element::toJson() const {
    return *this;
}

JsonArray &Element::toArray() const {
    return *this;
}

char *Element::c_str() const {
    return (char *)*this;
}

Element::Type Element::getType() const {
    return type;
}

String Element::getTypeName() const {
    return JSUtil::typeToString(type);
}

size_t Element::size() const {
    switch (type) {
        case jsonObject:
            if (object != NULL || value != NULL) {
                return as<Json &>().size();
            } else {
                return 0;
            }
        case jsonArray:
            if (array != NULL || value != NULL) {
                return as<JsonArray &>().size();
            } else {
                return 0;
            }
        case jsonString:
            if (value) {
                return value->length();
            } else {
                return 0;
            }
        default:
            return 0;
    }
}

size_t Element::lastIndex() const {
    size_t len = size();
    return len == 0 ? 0 : len - 1;
}

void Element::remove(const size_t &index, const size_t &count) {
    if (type == jsonString) {
        if (value) {
            value->remove(index, count);
        }
    } else if (type == jsonArray) {
        as<JsonArray &>().remove(index, count);
    }
}

void Element::remove(const String &key) {
    if (type == jsonObject) {
        as<Json &>().remove(key);
    }
}

void Element::clear() {
    switch (type) {
        case jsonObject:
            as<Json &>().clear();
            break;
        case jsonArray:
            as<JsonArray &>().clear();
            break;
        case jsonString:
            if (value) {
                value->clear();
            }
            break;
        case jsonNumber:
            if (value) {
                *value = "0";
            }
            break;
        case jsonBoolean:
            if (value) {
                *value = "false";
            }
            break;
    }
}

bool Element::contains(const Element &e) const {
    switch (type) {
        case jsonObject:
            if (value != NULL || object != NULL) {
                return as<Json &>().contains(e);
            } else {
                return false;
            }
        case jsonArray:
            if (value != NULL || array != NULL) {
                return as<JsonArray &>().contains(e);
            } else {
                return false;
            }
        case jsonString:
            if (value) {
                return value->indexOf(e.toString()) != -1;
            } else {
                return false;
            }
        default:
            return false;
    }
}

bool Element::isNull() const {
    return type == jsonNull;
}

bool Element::isNotNull() const {
    return type != jsonNull;
}

bool Element::isEmpty() const {
    switch (type) {
        case jsonObject:
            if (value || object) {
                return as<Json &>().isEmpty();
            } else {
                return true;
            }
        case jsonArray:
            if (value || array) {
                return as<JsonArray &>().isEmpty();
            } else {
                return true;
            }
        case jsonString:
            return value == NULL || value->isEmpty();
        case jsonNumber:
        case jsonBoolean:
            return value == NULL;
        default:
            return true;
    }
}

bool Element::isNotEmpty() const {
    return !isEmpty();
}

bool Element::isObject() const {
    return type == jsonObject;
}

bool Element::isArray() const {
    return type == jsonArray;
}

bool Element::isString() const {
    return type == jsonString;
}

bool Element::isNumber() const {
    return type == jsonNumber;
}

std::vector<Element>::iterator Element::begin() {
    if (type == jsonArray) {
        return as<JsonArray &>().begin();
    }
    return std::vector<Element>::iterator();
}

std::vector<Element>::iterator Element::end() {
    if (type == jsonArray) {
        return as<JsonArray &>().end();
    }
    return std::vector<Element>::iterator();
}

size_t Element::printTo(Print &p) const {
    switch (type) {
        case jsonObject:
            if (value) {
                return p.print(*value);
            } else if (object) {
                return p.print(object->toString());
            } else {
                return 0;
            }
        case jsonArray:
            if (value) {
                return p.print(*value);
            } else if (array) {
                return p.print(array->toString());
            } else {
                return 0;
            }
        default:
            if (value) {
                return p.print(*value);
            } else {
                return 0;
            }
    }
}

void Element::release() {
    if (value) delete value;
    if (object) delete object;
    if (array) delete array;
    value = NULL;
    object = NULL;
    array = NULL;
}

int Element::compareTo(const Element &e) const {
    switch (type) {
        case jsonString:
            if (value->equals(*e.value)) {
                return 0;
            } else {
                return value->compareTo(*e.value) > 0 ? 1 : -1;
            }
        case jsonNumber:
            if (e.type == jsonNumber) {
                if (as<double>() == e.as<double>()) {
                    return 0;
                } else {
                    return as<double>() > e.as<double>() ? 1 : -1;
                }
            }
            return -1;
        case jsonBoolean:
            if (as<bool>() == e.as<bool>()) {
                return 0;
            } else {
                return as<bool>() ? 1 : -1;
            }
        default:
            if (e.type == jsonBoolean) {
                if (as<bool>() == e.as<bool>()) {
                    return 0;
                } else {
                    return as<bool>() ? 1 : -1;
                }
            }
            return -1;
    }
}

/*--- Json Array ---*/

JsonArray::JsonArray() : v(std::vector<Element>()) {}

JsonArray::JsonArray(const char *str) : v(std::vector<Element>()) {
    JSUtil::parse(str, NULL, this);
}

JsonArray::JsonArray(const String &str) : v(std::vector<Element>()) {
    JSUtil::parse(str.c_str(), NULL, this);
}

JsonArray::JsonArray(const JsonArray &other) : v(other.v) {}

JsonArray::JsonArray(std::vector<int> &arr) : v(std::vector<Element>()) {
    for (int i : arr) push(i);
}

JsonArray &JsonArray::push(const Element &value) {
    v.push_back(value);
    return *this;
}

JsonArray &JsonArray::push() {
    push(Element());
    return *this;
}

Element &JsonArray::operator[](const uint16_t &index) {
    if (index >= v.size()) {
        _emptyElement = Element();
        return _emptyElement;
    }
    return v.at(index);
}

Element &JsonArray::getElement(const uint16_t &index) {
    if (index >= v.size()) {
        _emptyElement = Element();
        return _emptyElement;
    }
    return v[index];
}

const Element &JsonArray::operator[](const uint16_t &index) const {
    if (index >= v.size()) {
        _emptyElement = Element();
        return _emptyElement;
    }
    return v.at(index);
}

const Element &JsonArray::getElement(const uint16_t &index) const {
    if (index >= v.size()) {
        _emptyElement = Element();
        return _emptyElement;
    }
    return v.at(index);
}

bool JsonArray::operator==(const JsonArray &other) const {
    return v == other.v;
}

bool JsonArray::operator!=(const JsonArray &other) const {
    return v != other.v;
}

Element::Type JsonArray::getType(const uint16_t &index) const {
    if (index >= v.size()) {
        return Element::jsonNull;
    }
    return v[index].getType();
}

String JsonArray::getTypeName(const uint16_t &index) const {
    if (index >= v.size()) {
        return JSUtil::typeToString(Element::jsonNull);
    }
    return JSUtil::typeToString(v[index].getType());
}

String JsonArray::toString() const {
    String str("[");
    bool isFirst = true;
    for (int i = 0; i < v.size(); i++) {
        if (isFirst) {
            isFirst = false;
        } else {
            str += ",";
        }
        str += (v[i].getType() == Element::jsonString ? "\"" : "") + v[i].as<String>() + (v[i].getType() == Element::jsonString ? "\"" : "");
    }
    str += "]";
    return str;
}

size_t JsonArray::size() const {
    return v.size();
}

size_t JsonArray::lastIndex() const {
    return v.size() == 0 ? 0 : v.size() - 1;
}

void JsonArray::remove(const size_t &index, const size_t &count) {
    if (index < v.size()) {
        v.erase(v.begin() + index, v.begin() + index + count);
    }
}

void JsonArray::clear() {
    v.clear();
}

bool JsonArray::contains(const Element &e) const {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == e) {
            return true;
        }
    }
    return false;
}

bool JsonArray::isEmpty() const {
    return v.empty();
}

bool JsonArray::isNotEmpty() const {
    return !v.empty();
}

size_t JsonArray::printTo(Print &p) const {
    return p.print(toString());
}

std::vector<Element>::iterator JsonArray::begin() {
    return v.begin();
}

std::vector<Element>::iterator JsonArray::end() {
    return v.end();
}

/*--- Json Pair ---*/

Json::Pair::Pair(const String &key, const Element &value)
    : key(key), value(value) {}

bool Json::Pair::operator==(const Pair &other) const {
    return key == other.key && value == other.value;
}

bool Json::Pair::operator!=(const Pair &other) const {
    return key != other.key || value != other.value;
}

/*--- Json ---*/

Json::Json()
    : doc(std::vector<Pair>()) {}

Json::Json(const char *str)
    : doc(std::vector<Pair>()) {
    JSUtil::parse(str, this, NULL);
}

Json::Json(const String &str)
    : doc(std::vector<Pair>()) {
    JSUtil::parse(str.c_str(), this, NULL);
}

Json::Json(const Json &other)
    : doc(other.doc) {}

Json &Json::add(const String &name, const Element &value) {
    if (!contains(name)) {
        doc.push_back(Pair(name, value));
    } else {
        getElement(name) = value;
    }
    return *this;
}

Json &Json::add(const String &name) {
    add(name, Element());
    return *this;
}

Element &Json::operator[](const String &name) {
    if (!contains(name)) {
        add(name);
        return doc[doc.size() - 1].value;
    } else {
        for (int i = 0; i < doc.size(); i++) {
            if (doc[i].key == name) {
                return doc[i].value;
            }
        }
    }
    _emptyElement = Element();
    return _emptyElement;
}

Element &Json::operator[](const uint16_t &index) {
    if (index >= doc.size()) {
        _emptyElement = Element();
        return _emptyElement;
    }
    return doc[index].value;
}

const Element &Json::operator[](const String &name) const {
    for (int i = 0; i < doc.size(); i++) {
        if (doc[i].key == name) {
            return doc[i].value;
        }
    }
    _emptyElement = Element();
    return _emptyElement;
}

const Element &Json::operator[](const uint16_t &index) const {
    if (index >= doc.size()) {
        _emptyElement = Element();
        return _emptyElement;
    }
    return doc[index].value;
}

Element &Json::getElement(const String &name) {
    return operator[](name);
}

Element &Json::getElement(const uint16_t &index) {
    return operator[](index);
}

const Element &Json::getElement(const String &name) const {
    return operator[](name);
}

const Element &Json::getElement(const uint16_t &index) const {
    return operator[](index);
}

bool Json::operator==(const Json &other) const {
    return doc == other.doc;
}

bool Json::operator!=(const Json &other) const {
    return doc != other.doc;
}

Element::Type Json::getType(const String &name) const {
    return operator[](name).getType();
}

Element::Type Json::getType(const uint16_t &index) const {
    return operator[](index).getType();
}

String Json::getTypeName(const String &name) const {
    if (!contains(name)) {
        return JSUtil::typeToString(Element::jsonNull);
    }
    return JSUtil::typeToString(operator[](name).getType());
}

String Json::getTypeName(const uint16_t &index) const {
    if (index >= doc.size()) {
        return JSUtil::typeToString(Element::jsonNull);
    }
    return JSUtil::typeToString(operator[](index).getType());
}

String Json::getKey(const uint16_t &index) const {
    if (index >= doc.size()) {
        return "";
    }
    return doc[index].key;
}

int16_t Json::indexOf(const String &name) const {
    for (int i = 0; i < doc.size(); i++) {
        if (doc[i].key == name) {
            return i;
        }
    }
    return -1;
}

String Json::toString() const {
    String result("{");
    bool isFirst = true;
    for (int i = 0; i < doc.size(); i++) {
        if (isFirst) {
            isFirst = false;
        } else {
            result += ",";
        }
        result += "\"" + getKey(i) + "\":";
        result += (getElement(i).getType() == Element::jsonString ? "\"" : "") + getElement(i).as<String>() + (getElement(i).getType() == Element::jsonString ? "\"" : "");
    }
    result += "}";
    return result;
}

size_t Json::size() const {
    return doc.size();
}

bool Json::contains(const String &name) const {
    for (int i = 0; i < doc.size(); i++) {
        if (doc[i].key == name) {
            return true;
        }
    }
    return false;
}

bool Json::isEmpty() const {
    return doc.empty();
}

bool Json::isNotEmpty() const {
    return !doc.empty();
}

void Json::remove(const String &name) {
    int i = indexOf(name);
    if (i >= 0) {
        doc.erase(doc.begin() + i);
    }
}

void Json::clear() {
    doc.clear();
}

size_t Json::printTo(Print &p) const {
    return p.print(toString());
}

std::vector<Json::Pair>::iterator Json::begin() {
    return doc.begin();
}

std::vector<Json::Pair>::iterator Json::end() {
    return doc.end();
}

/**--- Json Utility ---**/

HardwareSerial *JSUtil::serial = NULL;

void JSUtil::parse(const char *str, Json *doc, JsonArray *v) {
    if (!str) return;
    if (!doc && !v) return;

    String s(str);
    s.trim();

    if (s.startsWith("{") && s.endsWith("}")) {
        if (!doc) {
            log(Error, F("Json"), F("Object container not set"));
            return;
        } else {
            if (s.length() == 2) {
                log(Warning, F("Json"), F("string contains empty object"));
                return;
            }
        }
    } else if (s.startsWith("[") && s.endsWith("]")) {
        if (!v) {
            log(Error, F("Json"), F("Array container not set"));
            return;
        } else {
            if (s.length() == 2) {
                log(Warning, F("Json"), F("string contains empty array"));
                return;
            }
        }
    } else {
        if (s.length() > 0) {
            if (!s.startsWith("{") || !s.startsWith("[")) {
                log(Error, F("Json"), String(F("Expected '{' or '[' at the beginning of string, found '")) + s[0] + String(F("'")));
            } else {
                log(Error, F("Json"), String(F("Expected '}' or ']' at the end of string, found '")) + s[s.length() - 1] + String(F("'")));
            }
        } else {
            log(Error, F("Json"), F("String cannot be empty"));
        }
        return;
    }

    remove(s, 1);
    remove(s, 1, true);
    s.trim();

    bool first = true;
    while (!s.isEmpty()) {
        if (s.startsWith(",")) {
            remove(s, 1);
            s.trim();
        } else {
            if (!first) {
                if (doc) {
                    log(Error, F("Json"), String(F("Expected ',' after node-value, found '")) + s[0] + String(F("'")));
                } else {
                    log(Error, F("Json"), String(F("Expected ',' after array element, found '")) + s[0] + String(F("'")));
                }
                return;
            }
            first = false;
        }

        String key = "";
        if (doc) {
            if (!s.startsWith("\"")) {
                log(Error, F("Json"), String(F("Expected '\"' after ',', found '")) + s[0] + String(F("'")));
                return;
            }
            bool found = false;
            for (int i = 1; i < s.length(); i++) {
                if (s[i] == '"' && s[i - 1] != '\\') {
                    key = s.substring(1, i);
                    remove(s, i + 1);
                    s.trim();
                    found = true;
                    break;
                }
            }
            if (!found) {
                log(Error, F("Json"), F("Node-name must be enclosed with '\"'"));
                return;
            }
            if (!s.startsWith(":")) {
                log(Error, F("Json"), String(F("Expected ':' before node-value, found '")) + s[0] + String(F("' (node-name: ")) + key + String(F(")")));
                return;
            }
            remove(s, 1);
            s.trim();
        }

        if (s.startsWith("\"")) {
            bool found = false;
            for (int i = 1; i < s.length(); i++) {
                if (s[i] == '"' && s[i - 1] != '\\') {
                    if (doc) {
                        doc->add(key, Element(s.substring(1, i), Element::jsonString));
                    } else {
                        v->push(Element(s.substring(1, i), Element::jsonString));
                    }
                    remove(s, i + 1);
                    s.trim();
                    found = true;
                    break;
                }
            }
            if (!found) {
                if (doc) {
                    log(Error, F("Json"), String(F("Node-value with type 'string' must be enclosed with '\"\"' (node-name: ")) + key + String(F(")")));
                } else {
                    log(Error, F("Json"), String(F("Array element with type 'string' must be enclosed with '\"\"'")));
                }
                return;
            }
        } else if (s.startsWith("{")) {
            int x = 1;
            for (int i = 1; i < s.length(); i++) {
                if (s[i] == '{') {
                    x++;
                } else if (s[i] == '}') {
                    x--;
                }
                if (x == 0) {
                    if (doc) {
                        doc->add(key, Element(s.substring(0, i + 1), Element::jsonObject));
                    } else {
                        v->push(Element(s.substring(0, i + 1), Element::jsonObject));
                    }
                    remove(s, i + 1);
                    s.trim();
                    break;
                }
            }
            if (x != 0) {
                if (doc) {
                    log(Error, F("Json"), String(F("Node-value with type 'object' must be enclosed with '{}' (node-name: ")) + key + String(F(")")));
                } else {
                    log(Error, F("Json"), String(F("Array element with type 'object' must be enclosed with '{}'")));
                }
                return;
            }
        } else if (s.startsWith("[")) {
            int x = 1;
            for (int i = 1; i < s.length(); i++) {
                if (s[i] == '[') {
                    x++;
                } else if (s[i] == ']') {
                    x--;
                }
                if (x == 0) {
                    if (doc) {
                        doc->add(key, Element(s.substring(0, i + 1), Element::jsonArray));
                    } else {
                        v->push(Element(s.substring(0, i + 1), Element::jsonArray));
                    }
                    remove(s, i + 1);
                    s.trim();
                    break;
                }
            }
            if (x != 0) {
                if (doc) {
                    log(Error, F("Json"), String(F("Node-value with type 'array' must be enclosed with '[]' (node-name: ")) + key + String(F(")")));
                } else {
                    log(Error, F("Json"), String(F("Array element with type 'array' must be enclosed with '[]'")));
                }
                return;
            }
        } else {
            String buf = s.substring(0, s.indexOf(","));
            buf.trim();
            remove(s, s.indexOf(","));
            s.trim();
            if (buf.equals("true") || buf.equals("false") || buf.equals("null")) {
                if (doc) {
                    doc->add(key, Element(buf, buf.equals("null") ? Element::jsonNull : Element::jsonBoolean));
                } else {
                    v->push(Element(buf, buf.equals("null") ? Element::jsonNull : Element::jsonBoolean));
                }
            } else {
                if (buf.length() > 1) {
                    if (buf.startsWith("0") && !String(buf[1]).equals(".")) {
                        if (doc) {
                            log(Error, F("Json"), String(F("Node-value with type 'number' cannot be started with '0' (node-name: ")) + key + String(F(")")));
                        } else {
                            log(Error, F("Json"), String(F("Array element with type 'number' cannot be started with '0'")));
                        }
                        return;
                    } else if (!isdigit(buf[0]) && !String(buf[0]).equals("-")) {
                        if (doc) {
                            log(Error, F("Json"), String(F("Node-value with type 'literal' can only contain 'true', 'false', 'null', or 'number' (node-name: ")) + key + String(F(")")));
                        } else {
                            log(Error, F("Json"), String(F("Array element with type 'literal' can only contain 'true', 'false', 'null', or 'number'")));
                        }
                        return;
                    }
                } else {
                    if (buf.isEmpty() || !isdigit(buf[0])) {
                        if (doc) {
                            log(Error, F("Json"), String(F("Node-value with type 'literal' can only contain 'true', 'false', 'null', or 'number' (node-name: ")) + key + String(F(")")));
                        } else {
                            log(Error, F("Json"), String(F("Array element with type 'literal' can only contain 'true', 'false', 'null', or 'number'")));
                        }
                        return;
                    }
                }
                int d = 0, e = 0;
                for (int i = 0; i < buf.length(); i++) {
                    if (String(buf[i]).equals(".")) {
                        if (++d > 1 || e > 0) {
                            if (doc) {
                                log(Error, F("Json"), String(F("Invalid number format (node-name: ")) + key + String(F(")")));
                            } else {
                                log(Error, F("Json"), String(F("Invalid number format")));
                            }
                            return;
                        }
                    } else if (String(buf[i]).equals("e") || String(buf[i]).equals("E")) {
                        if (++e > 1) {
                            if (doc) {
                                log(Error, F("Json"), String(F("Invalid number format (node-name: ")) + key + String(F(")")));
                            } else {
                                log(Error, F("Json"), String(F("Invalid number format")));
                            }
                            return;
                        }
                    } else if (String(buf[i]).equals("-") || String(buf[i]).equals("+")) {
                        if (i != 0 && !(String(buf[i - 1]).equals("e") || String(buf[i - 1]).equals("E"))) {
                            if (doc) {
                                log(Error, F("Json"), String(F("Invalid number format (node-name: ")) + key + String(F(")")));
                            } else {
                                log(Error, F("Json"), String(F("Invalid number format")));
                            }
                            return;
                        }
                    } else if (!isdigit(buf[i])) {
                        if (doc) {
                            log(Error, F("Json"), String(F("Invalid number format (node-name: ")) + key + String(F(")")));
                        } else {
                            log(Error, F("Json"), String(F("Invalid number format")));
                        }
                        return;
                    }
                }
                if (doc) {
                    doc->add(key, Element(buf, Element::jsonNumber));
                } else {
                    v->push(Element(buf, Element::jsonNumber));
                }
            }
        }
    }
}

void JSUtil::prettyPrint(HardwareSerial &serial, Json &doc, const uint8_t &spaceTab) {
    String s = doc.toString();
    prettyPrint(serial, &doc, NULL, spaceTab, 0);
}

void JSUtil::prettyPrint(HardwareSerial &serial, JsonArray &v, const uint8_t &spaceTab) {
    prettyPrint(serial, NULL, &v, spaceTab, 0);
}

void JSUtil::prettyPrint(HardwareSerial &serial, Json *doc, JsonArray *v, const uint8_t &spaceTab, const uint8_t &space) {
    if (!doc && !v) return;
    String sTab = "";
    for (int i = 0; i < spaceTab; i++) {
        sTab += " ";
    }

    String indent = "";
    for (int i = 0; i <= space; i++) {
        indent += sTab;
    }

    String inBot = "";
    for (int i = 0; i < space; i++) {
        inBot += sTab;
    }

    serial.print(doc ? "{\n" : "[\n");
    bool isFirst = true;

    for (int i = 0; i < (doc ? doc->size() : v->size()); i++) {
        if (isFirst) {
            isFirst = false;
        } else {
            serial.print(",\n");
        }
        serial.print(indent + (doc ? "\"" + doc->getKey(i) + "\":" : ""));
        switch ((doc ? doc->getType(i) : v->getType(i))) {
            case Element::jsonObject: {
                if (doc) {
                    prettyPrint(serial, &doc->getElement(i).as<Json &>(), NULL, spaceTab, space + 1);
                } else {
                    prettyPrint(serial, &v->getElement(i).as<Json &>(), NULL, spaceTab, space + 1);
                }
                break;
            }
            case Element::jsonArray: {
                if (doc) {
                    prettyPrint(serial, NULL, &doc->getElement(i).as<JsonArray &>(), spaceTab, space + 1);
                } else {
                    prettyPrint(serial, NULL, &v->getElement(i).as<JsonArray &>(), spaceTab, space + 1);
                }
                break;
            }
            case Element::jsonString: {
                if (doc) {
                    serial.print("\"" + doc->getElement(i).as<String>() + "\"");
                } else {
                    serial.print("\"" + v->getElement(i).as<String>() + "\"");
                }
                break;
            }
            case Element::jsonBoolean:
            case Element::jsonNumber:
            case Element::jsonNull: {
                if (doc) {
                    serial.print(doc->getElement(i).as<String>());
                } else {
                    serial.print(v->getElement(i).as<String>());
                }
                break;
            }
        }
    }
    serial.print("\n" + inBot + (doc ? "}" : "]") + String(space == 0 ? "\n" : ""));
}

bool JSUtil::isFloat(const String &str) {
    return fmod(str.toDouble(), 1) != 0;
}

String JSUtil::typeToString(const Element::Type &type) {
    switch (type) {
        case Element::jsonObject:
            return "object";
        case Element::jsonArray:
            return "array";
        case Element::jsonString:
            return "string";
        case Element::jsonNumber:
            return "number";
        case Element::jsonBoolean:
            return "boolean";
        case Element::jsonNull:
            return "null";
        default:
            return "undefined";
    }
}

String JSUtil::removeInsignificantZeros(const String &str) {
    String s = str;
    if (s.indexOf(".") != -1) {
        while (s.endsWith("0")) {
            s.remove(s.length() - 1);
        }
        if (s.endsWith(".")) {
            s.remove(s.length() - 1);
        }
    }
    return s;
}

String JSUtil::toString(const float &value) {
    return removeInsignificantZeros(String(value, 5));
}

String JSUtil::toString(const double &value) {
    return removeInsignificantZeros(String(value, 11));
}

void JSUtil::remove(String &str, const int &len, const bool &fromEnd) {
    if (len < 0 || len >= str.length()) {
        str.clear();
    } else if (len < str.length()) {
        if (fromEnd) {
            str.remove(str.length() - len);
        } else {
            str.remove(0, len);
        }
    }
}

void JSUtil::attachDebugger(HardwareSerial &hwSerial) {
    serial = &hwSerial;
}

void JSUtil::detachDebugger() {
    serial = NULL;
}

void JSUtil::log(const LogLevel &level, const String &tag, const String &message) {
    if (serial) {
        static String severity[] = {"[E]: ", "[I]: ", "[W]: "};
        serial->println(severity[static_cast<int>(level)] + tag + ": " + message);
    }
}
