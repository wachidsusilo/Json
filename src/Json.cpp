#include "Json.h"

static JSType::Element _emptyElement;

/**--- Json Type ---**/
JSType::Element::Element()
    : value(new String("null")), type(jsonNull), object(NULL), array(NULL) {}

JSType::Element::~Element() {
    release();
}

JSType::Element::Element(const String &value, const Type &type)
    : value(new String(value)), type(type), object(NULL), array(NULL) {}

JSType::Element::Element(const Element &e)
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

JSType::Element::Element(const int8_t &value)
    : value(new String(value)), type(jsonInteger), object(NULL), array(NULL) {}

JSType::Element::Element(const int16_t &value)
    : value(new String(value)), type(jsonInteger), object(NULL), array(NULL) {}

JSType::Element::Element(const int32_t &value)
    : value(new String(value)), type(jsonInteger), object(NULL), array(NULL) {}

JSType::Element::Element(const uint8_t &value)
    : value(new String(value)), type(jsonInteger), object(NULL), array(NULL) {}

JSType::Element::Element(const uint16_t &value)
    : value(new String(value)), type(jsonInteger), object(NULL), array(NULL) {}

JSType::Element::Element(const uint32_t &value)
    : value(new String(value)), type(jsonInteger), object(NULL), array(NULL) {}

JSType::Element::Element(const float &value)
    : value(new String(JSUtil.toString(value))), type(jsonFloat), object(NULL), array(NULL) {}

JSType::Element::Element(const double &value)
    : value(new String(JSUtil.toString(value))), type(jsonFloat), object(NULL), array(NULL) {}

JSType::Element::Element(const String &value)
    : value(new String(value)), type(jsonString), object(NULL), array(NULL) {}

JSType::Element::Element(const char *value)
    : value(new String(value)), type(jsonString), object(NULL), array(NULL) {}

JSType::Element::Element(const bool &value)
    : value(new String(value ? "true" : "false")), type(jsonBoolean), object(NULL), array(NULL) {}

JSType::Element::Element(const Json &value)
    : value(NULL), type(jsonObject), object(new Json(value)), array(NULL) {}

JSType::Element::Element(const JsonArray &value)
    : value(NULL), type(jsonArray), object(NULL), array(new JsonArray(value)) {}

JSType::Element::operator int8_t() const {
    if (!value) return 0;
    return round((*value).toDouble());
}

JSType::Element::operator int16_t() const {
    if (!value) return 0;
    return round((*value).toDouble());
}

JSType::Element::operator int32_t() const {
    if (!value) return 0;
    return round((*value).toDouble());
}

JSType::Element::operator uint8_t() const {
    if (!value) return 0;
    return round((*value).toDouble());
}

JSType::Element::operator uint16_t() const {
    if (!value) return 0;
    return round((*value).toDouble());
}

JSType::Element::operator uint32_t() const {
    if (!value) return 0;
    return round((*value).toDouble());
}

JSType::Element::operator float() const {
    if (!value) return 0;
    return (*value).toFloat();
}

JSType::Element::operator double() const {
    if (!value) return 0;
    return (*value).toDouble();
}

JSType::Element::operator bool() const {
    switch (type) {
        case jsonObject:
            return value != NULL || object != NULL;
        case jsonArray:
            return value != NULL || array != NULL;
        case jsonString:
            return value != NULL && !value->isEmpty();
        case jsonInteger:
            return as<int>() != 0;
        case jsonFloat:
            return as<double>() != 0.0;
        case jsonBoolean:
            return value != NULL && *value == "true";
        case jsonNull:
            return value != NULL && *value == "null";
        default:
            return false;
    }
}

JSType::Element::operator String() const {
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

JSType::Element::operator char *() const {
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

JSType::Element::operator Json &() const {
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

JSType::Element::operator JsonArray &() const {
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

JSType::Element &JSType::Element::operator=(const Element &e) {
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

JSType::Element &JSType::Element::operator[](const int8_t &index) {
    return operator[]((uint32_t)index);
}

JSType::Element &JSType::Element::operator[](const int16_t &index) {
    return operator[]((uint32_t)index);
}

JSType::Element &JSType::Element::operator[](const int32_t &index) {
    return operator[]((uint32_t)index);
}

JSType::Element &JSType::Element::operator[](const uint8_t &index) {
    return operator[]((uint32_t)index);
}

JSType::Element &JSType::Element::operator[](const uint16_t &index) {
    return operator[]((uint32_t)index);
}

JSType::Element &JSType::Element::operator[](const uint32_t &index) {
    if (type != jsonArray) {
        _emptyElement = Element();
        return _emptyElement;
    }
    return as<JsonArray &>()[index];
}

JSType::Element &JSType::Element::operator[](const char *key) {
    return operator[](String(key));
}

JSType::Element &JSType::Element::operator[](const String &key) {
    if (type != jsonObject) {
        _emptyElement = Element();
        return _emptyElement;
    }
    return as<Json &>()[key];
}

const JSType::Element &JSType::Element::operator[](const int8_t &index) const {
    return operator[]((uint32_t)index);
}

const JSType::Element &JSType::Element::operator[](const int16_t &index) const {
    return operator[]((uint32_t)index);
}

const JSType::Element &JSType::Element::operator[](const int32_t &index) const {
    return operator[]((uint32_t)index);
}

const JSType::Element &JSType::Element::operator[](const uint8_t &index) const {
    return operator[]((uint32_t)index);
}

const JSType::Element &JSType::Element::operator[](const uint16_t &index) const {
    return operator[]((uint32_t)index);
}

const JSType::Element &JSType::Element::operator[](const uint32_t &index) const {
    if (type != jsonArray) {
        _emptyElement = Element();
        return _emptyElement;
    }
    return as<JsonArray &>()[index];
}

const JSType::Element &JSType::Element::operator[](const char *key) const {
    return operator[](String(key));
}

const JSType::Element &JSType::Element::operator[](const String &key) const {
    if (type != jsonObject) {
        _emptyElement = Element();
        return _emptyElement;
    }
    return as<Json &>()[key];
}

bool JSType::Element::operator==(const Element &e) const {
    switch (type) {
        case jsonObject:
            if (e.type != jsonObject) {
                return false;
            }
            if (value) {
                if (e.object) {
                    return Json(*value) == *e.object;
                } else if (e.value) {
                    return Json(*value) == Json(*e.value);
                } else {
                    return false;
                }
            } else if (object) {
                if (e.object) {
                    return *object == *e.object;
                } else if (e.value) {
                    return *object == Json(*e.value);
                } else {
                    return false;
                }
            } else {
                return false;
            }
        case jsonArray:
            if (e.type != jsonArray) {
                return false;
            }
            if (value) {
                if (e.array) {
                    return JsonArray(*value) == *e.array;
                } else if (e.value) {
                    return JsonArray(*value) == JsonArray(*e.value);
                } else {
                    return false;
                }
            } else if (array) {
                if (e.array) {
                    return *array == *e.array;
                } else if (e.value) {
                    return *array == JsonArray(*e.value);
                } else {
                    return false;
                }
            } else {
                return false;
            }
        case jsonString:
            return value->equals(*e.value);
        case jsonInteger:
        case jsonFloat:
            if (e.type == jsonInteger || e.type == jsonFloat) {
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

bool JSType::Element::operator==(const int8_t &e) const {
    if (type == jsonInteger || type == jsonFloat) {
        return as<int8_t>() == e;
    }
    return false;
}

bool JSType::Element::operator==(const int16_t &e) const {
    if (type == jsonInteger || type == jsonFloat) {
        return as<int16_t>() == e;
    }
    return false;
}

bool JSType::Element::operator==(const int32_t &e) const {
    if (type == jsonInteger || type == jsonFloat) {
        return as<int32_t>() == e;
    }
    return false;
}

bool JSType::Element::operator==(const uint8_t &e) const {
    if (type == jsonInteger || type == jsonFloat) {
        return as<uint8_t>() == e;
    }
    return false;
}

bool JSType::Element::operator==(const uint16_t &e) const {
    if (type == jsonInteger || type == jsonFloat) {
        return as<uint16_t>() == e;
    }
    return false;
}

bool JSType::Element::operator==(const uint32_t &e) const {
    if (type == jsonInteger || type == jsonFloat) {
        return as<uint32_t>() == e;
    }
    return false;
}

bool JSType::Element::operator==(const float &e) const {
    if (type == jsonInteger || type == jsonFloat) {
        return as<float>() == e;
    }
    return false;
}

bool JSType::Element::operator==(const double &e) const {
    if (type == jsonInteger || type == jsonFloat) {
        return as<double>() == e;
    }
    return false;
}

bool JSType::Element::operator==(const bool &e) const {
    return as<bool>() == e;
}

bool JSType::Element::operator==(const char *e) const {
    if (type == jsonString) {
        return as<String>() == e;
    }
    return false;
}

bool JSType::Element::operator==(const String &e) const {
    if (type == jsonString) {
        return as<String>() == e;
    }
    return false;
}

bool JSType::Element::operator!=(const Element &e) const {
    return !operator==(e);
}

bool JSType::Element::operator!=(const int8_t &e) const {
    return !operator==(e);
}

bool JSType::Element::operator!=(const int16_t &e) const {
    return !operator==(e);
}

bool JSType::Element::operator!=(const int32_t &e) const {
    return !operator==(e);
}

bool JSType::Element::operator!=(const uint8_t &e) const {
    return !operator==(e);
}

bool JSType::Element::operator!=(const uint16_t &e) const {
    return !operator==(e);
}

bool JSType::Element::operator!=(const uint32_t &e) const {
    return !operator==(e);
}

bool JSType::Element::operator!=(const float &e) const {
    return !operator==(e);
}

bool JSType::Element::operator!=(const double &e) const {
    return !operator==(e);
}

bool JSType::Element::operator!=(const bool &e) const {
    return !operator==(e);
}

bool JSType::Element::operator!=(const char *e) const {
    return !operator==(e);
}

bool JSType::Element::operator!=(const String &e) const {
    return !operator==(e);
}

bool JSType::Element::operator<(const Element &e) const {
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

bool JSType::Element::operator<(const int8_t &e) const {
    return compareTo(e) < 0;
}

bool JSType::Element::operator<(const int16_t &e) const {
    return compareTo(e) < 0;
}

bool JSType::Element::operator<(const int32_t &e) const {
    return compareTo(e) < 0;
}

bool JSType::Element::operator<(const uint8_t &e) const {
    return compareTo(e) < 0;
}

bool JSType::Element::operator<(const uint16_t &e) const {
    return compareTo(e) < 0;
}

bool JSType::Element::operator<(const uint32_t &e) const {
    return compareTo(e) < 0;
}

bool JSType::Element::operator<(const float &e) const {
    return compareTo(e) < 0;
}

bool JSType::Element::operator<(const double &e) const {
    return compareTo(e) < 0;
}

bool JSType::Element::operator<(const bool &e) const {
    return compareTo(e) < 0;
}

bool JSType::Element::operator<(const char *e) const {
    return compareTo(e) < 0;
}

bool JSType::Element::operator<(const String &e) const {
    return compareTo(e) < 0;
}

bool JSType::Element::operator>(const Element &e) const {
    return compareTo(e) > 0;
}

bool JSType::Element::operator>(const int8_t &e) const {
    return compareTo(e) > 0;
}

bool JSType::Element::operator>(const int16_t &e) const {
    return compareTo(e) > 0;
}

bool JSType::Element::operator>(const int32_t &e) const {
    return compareTo(e) > 0;
}

bool JSType::Element::operator>(const uint8_t &e) const {
    return compareTo(e) > 0;
}

bool JSType::Element::operator>(const uint16_t &e) const {
    return compareTo(e) > 0;
}

bool JSType::Element::operator>(const uint32_t &e) const {
    return compareTo(e) > 0;
}

bool JSType::Element::operator>(const float &e) const {
    return compareTo(e) > 0;
}

bool JSType::Element::operator>(const double &e) const {
    return compareTo(e) > 0;
}

bool JSType::Element::operator>(const bool &e) const {
    return compareTo(e) > 0;
}

bool JSType::Element::operator>(const char *e) const {
    return compareTo(e) > 0;
}

bool JSType::Element::operator>(const String &e) const {
    return compareTo(e) > 0;
}

bool JSType::Element::operator<=(const Element &e) const {
    return compareTo(e) <= 0;
}

bool JSType::Element::operator<=(const int8_t &e) const {
    return compareTo(e) <= 0;
}

bool JSType::Element::operator<=(const int16_t &e) const {
    return compareTo(e) <= 0;
}

bool JSType::Element::operator<=(const int32_t &e) const {
    return compareTo(e) <= 0;
}

bool JSType::Element::operator<=(const uint8_t &e) const {
    return compareTo(e) <= 0;
}

bool JSType::Element::operator<=(const uint16_t &e) const {
    return compareTo(e) <= 0;
}

bool JSType::Element::operator<=(const uint32_t &e) const {
    return compareTo(e) <= 0;
}

bool JSType::Element::operator<=(const float &e) const {
    return compareTo(e) <= 0;
}

bool JSType::Element::operator<=(const double &e) const {
    return compareTo(e) <= 0;
}

bool JSType::Element::operator<=(const bool &e) const {
    return compareTo(e) <= 0;
}

bool JSType::Element::operator<=(const char *e) const {
    return compareTo(e) <= 0;
}

bool JSType::Element::operator<=(const String &e) const {
    return compareTo(e) <= 0;
}

bool JSType::Element::operator>=(const Element &e) const {
    return compareTo(e) >= 0;
}

bool JSType::Element::operator>=(const int8_t &e) const {
    return compareTo(e) >= 0;
}

bool JSType::Element::operator>=(const int16_t &e) const {
    return compareTo(e) >= 0;
}

bool JSType::Element::operator>=(const int32_t &e) const {
    return compareTo(e) >= 0;
}

bool JSType::Element::operator>=(const uint8_t &e) const {
    return compareTo(e) >= 0;
}

bool JSType::Element::operator>=(const uint16_t &e) const {
    return compareTo(e) >= 0;
}

bool JSType::Element::operator>=(const uint32_t &e) const {
    return compareTo(e) >= 0;
}

bool JSType::Element::operator>=(const float &e) const {
    return compareTo(e) >= 0;
}

bool JSType::Element::operator>=(const double &e) const {
    return compareTo(e) >= 0;
}

bool JSType::Element::operator>=(const bool &e) const {
    return compareTo(e) >= 0;
}

bool JSType::Element::operator>=(const char *e) const {
    return compareTo(e) >= 0;
}

bool JSType::Element::operator>=(const String &e) const {
    return compareTo(e) >= 0;
}

JSType::Element JSType::Element::operator+(const Element &e) const {
    if (value == NULL || e.value == NULL) return *this;
    switch (type) {
        case jsonInteger:
        case jsonFloat: {
            switch (e.type) {
                case jsonInteger:
                case jsonFloat:
                    return as<double>() + e.as<double>();
                case jsonString:
                    return as<String>() + e.as<String>();
                default:
                    return *this;
            }
        }
        case jsonString: {
            switch (e.type) {
                case jsonInteger:
                case jsonFloat:
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

JSType::Element JSType::Element::operator+(const int8_t &e) const {
    return operator+((double)e);
}

JSType::Element JSType::Element::operator+(const int16_t &e) const {
    return operator+((double)e);
}

JSType::Element JSType::Element::operator+(const int32_t &e) const {
    return operator+((double)e);
}

JSType::Element JSType::Element::operator+(const uint8_t &e) const {
    return operator+((double)e);
}

JSType::Element JSType::Element::operator+(const uint16_t &e) const {
    return operator+((double)e);
}

JSType::Element JSType::Element::operator+(const uint32_t &e) const {
    return operator+((double)e);
}

JSType::Element JSType::Element::operator+(const float &e) const {
    return operator+((double)e);
}

JSType::Element JSType::Element::operator+(const double &e) const {
    if (type == jsonInteger || type == jsonFloat) {
        return as<double>() + e;
    }
    return *this;
}

JSType::Element JSType::Element::operator+(const bool &e) const {
    return operator+(String(e ? "true" : "false"));
}

JSType::Element JSType::Element::operator+(const char *e) const {
    return operator+(String(e));
}

JSType::Element JSType::Element::operator+(const String &e) const {
    if (type == jsonString) {
        return as<String>() + e;
    }
    return *this;
}

JSType::Element JSType::Element::operator-(const Element &e) const {
    if (value == NULL || e.value == NULL) return *this;
    if ((type == jsonInteger || type == jsonFloat) && (e.type == jsonInteger || e.type == jsonFloat)) {
        return as<double>() - e.as<double>();
    }
    return *this;
}

JSType::Element JSType::Element::operator-(const int8_t &e) const {
    return operator-((double)e);
}

JSType::Element JSType::Element::operator-(const int16_t &e) const {
    return operator-((double)e);
}

JSType::Element JSType::Element::operator-(const int32_t &e) const {
    return operator-((double)e);
}

JSType::Element JSType::Element::operator-(const uint8_t &e) const {
    return operator-((double)e);
}

JSType::Element JSType::Element::operator-(const uint16_t &e) const {
    return operator-((double)e);
}

JSType::Element JSType::Element::operator-(const uint32_t &e) const {
    return operator-((double)e);
}

JSType::Element JSType::Element::operator-(const float &e) const {
    return operator-((double)e);
}

JSType::Element JSType::Element::operator-(const double &e) const {
    if (type == jsonInteger || type == jsonFloat) {
        return as<double>() - e;
    }
    return *this;
}

JSType::Element JSType::Element::operator*(const Element &e) const {
    if (value == NULL || e.value == NULL) return *this;
    if ((type == jsonInteger || type == jsonFloat) && (e.type == jsonInteger || e.type == jsonFloat)) {
        return as<double>() * e.as<double>();
    }
    return *this;
}

JSType::Element JSType::Element::operator*(const int8_t &e) const {
    return operator*((double)e);
}

JSType::Element JSType::Element::operator*(const int16_t &e) const {
    return operator*((double)e);
}

JSType::Element JSType::Element::operator*(const int32_t &e) const {
    return operator*((double)e);
}

JSType::Element JSType::Element::operator*(const uint8_t &e) const {
    return operator*((double)e);
}

JSType::Element JSType::Element::operator*(const uint16_t &e) const {
    return operator*((double)e);
}

JSType::Element JSType::Element::operator*(const uint32_t &e) const {
    return operator*((double)e);
}

JSType::Element JSType::Element::operator*(const float &e) const {
    return operator*((double)e);
}

JSType::Element JSType::Element::operator*(const double &e) const {
    if (type == jsonInteger || type == jsonFloat) {
        return as<double>() * e;
    }
    return *this;
}

JSType::Element JSType::Element::operator/(const Element &e) const {
    if (value == NULL || e.value == NULL) return *this;
    if ((type == jsonInteger || type == jsonFloat) && (e.type == jsonInteger || e.type == jsonFloat)) {
        return as<double>() / e.as<double>();
    }
    return *this;
}

JSType::Element JSType::Element::operator/(const int8_t &e) const {
    return operator/((double)e);
}

JSType::Element JSType::Element::operator/(const int16_t &e) const {
    return operator/((double)e);
}

JSType::Element JSType::Element::operator/(const int32_t &e) const {
    return operator/((double)e);
}

JSType::Element JSType::Element::operator/(const uint8_t &e) const {
    return operator/((double)e);
}

JSType::Element JSType::Element::operator/(const uint16_t &e) const {
    return operator/((double)e);
}

JSType::Element JSType::Element::operator/(const uint32_t &e) const {
    return operator/((double)e);
}

JSType::Element JSType::Element::operator/(const float &e) const {
    return operator/((double)e);
}

JSType::Element JSType::Element::operator/(const double &e) const {
    if (type == jsonInteger || type == jsonFloat) {
        return as<double>() / e;
    }
    return *this;
}

JSType::Element JSType::Element::operator%(const Element &e) const {
    if (value == NULL || e.value == NULL) return *this;
    switch (type) {
        case jsonInteger:
            if (e.type == jsonInteger) {
                return as<int32_t>() % e.as<int32_t>();
            } else if (e.type == jsonFloat) {
                return fmod(as<double>(), e.as<double>());
            }
        case jsonFloat:
            if (e.type == jsonInteger || e.type == jsonFloat) {
                return fmod(as<double>(), e.as<double>());
            }
    }
    return *this;
}

JSType::Element JSType::Element::operator%(const int8_t &e) const {
    return operator%((int32_t)e);
}

JSType::Element JSType::Element::operator%(const int16_t &e) const {
    return operator%((int32_t)e);
}

JSType::Element JSType::Element::operator%(const int32_t &e) const {
    switch (type) {
        case jsonInteger:
            return as<int32_t>() % e;
        case jsonFloat:
            return fmod(as<double>(), (double)e);
    }
    return *this;
}

JSType::Element JSType::Element::operator%(const uint8_t &e) const {
    return operator%((uint32_t)e);
}

JSType::Element JSType::Element::operator%(const uint16_t &e) const {
    return operator%((uint32_t)e);
}

JSType::Element JSType::Element::operator%(const uint32_t &e) const {
    switch (type) {
        case jsonInteger:
            return as<int32_t>() % e;
        case jsonFloat:
            return fmod(as<double>(), (double)e);
    }
    return *this;
}

JSType::Element JSType::Element::operator%(const float &e) const {
    return operator%((double)e);
}

JSType::Element JSType::Element::operator%(const double &e) const {
    if (type == jsonInteger || type == jsonFloat) {
        return fmod(as<double>(), e);
    }
    return *this;
}

JSType::Element &JSType::Element::operator+=(const Element &e) {
    if (value == NULL || e.value == NULL) return *this;
    switch (type) {
        case jsonInteger:
        case jsonFloat: {
            if (e.type == jsonInteger || e.type == jsonFloat) {
                *value = JSUtil.toString(as<double>() + e.as<double>());
                if (type == jsonInteger && fmod(as<double>(), 1) != 0.0) {
                    type = jsonFloat;
                }
            }
            break;
        }
        case jsonString: {
            switch (e.type) {
                case jsonInteger:
                case jsonFloat:
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

JSType::Element &JSType::Element::operator+=(const int8_t &e) {
    return operator+=((double)e);
}

JSType::Element &JSType::Element::operator+=(const int16_t &e) {
    return operator+=((double)e);
}

JSType::Element &JSType::Element::operator+=(const int32_t &e) {
    return operator+=((double)e);
}

JSType::Element &JSType::Element::operator+=(const uint8_t &e) {
    return operator+=((double)e);
}

JSType::Element &JSType::Element::operator+=(const uint16_t &e) {
    return operator+=((double)e);
}

JSType::Element &JSType::Element::operator+=(const uint32_t &e) {
    return operator+=((double)e);
}

JSType::Element &JSType::Element::operator+=(const float &e) {
    return operator+=((double)e);
}

JSType::Element &JSType::Element::operator+=(const double &e) {
    if (type == jsonInteger || type == jsonFloat) {
        *value = JSUtil.toString(as<double>() + e);
        if (type == jsonInteger && fmod(as<double>(), 1) != 0.0) {
            type = jsonFloat;
        }
    } else if (type == jsonString) {
        *value += JSUtil.toString(e);
    }
    return *this;
}

JSType::Element &JSType::Element::operator+=(const bool &e) {
    return operator+=(String(e ? "true" : "false"));
}

JSType::Element &JSType::Element::operator+=(const char *e) {
    return operator+=(String(e));
}

JSType::Element &JSType::Element::operator+=(const String &e) {
    if (type == jsonString) {
        *value += e;
    }
    return *this;
}

JSType::Element &JSType::Element::operator-=(const Element &e) {
    if (value == NULL || e.value == NULL) return *this;
    if ((type == jsonInteger || type == jsonFloat) && (e.type == jsonInteger || e.type == jsonFloat)) {
        *value = JSUtil.toString(as<double>() - e.as<double>());
        if (type == jsonInteger && fmod(as<double>(), 1) != 0.0) {
            type = jsonFloat;
        }
    }
    return *this;
}

JSType::Element &JSType::Element::operator-=(const int8_t &e) {
    return operator-=((double)e);
}

JSType::Element &JSType::Element::operator-=(const int16_t &e) {
    return operator-=((double)e);
}

JSType::Element &JSType::Element::operator-=(const int32_t &e) {
    return operator-=((double)e);
}

JSType::Element &JSType::Element::operator-=(const uint8_t &e) {
    return operator-=((double)e);
}

JSType::Element &JSType::Element::operator-=(const uint16_t &e) {
    return operator-=((double)e);
}

JSType::Element &JSType::Element::operator-=(const uint32_t &e) {
    return operator-=((double)e);
}

JSType::Element &JSType::Element::operator-=(const float &e) {
    return operator-=((double)e);
}

JSType::Element &JSType::Element::operator-=(const double &e) {
    if (type == jsonInteger || type == jsonFloat) {
        *value = JSUtil.toString(as<double>() - e);
        if (type == jsonInteger && fmod(as<double>(), 1) != 0.0) {
            type = jsonFloat;
        }
    }
    return *this;
}

JSType::Element &JSType::Element::operator*=(const Element &e) {
    if (value == NULL || e.value == NULL) return *this;
    if ((type == jsonInteger || type == jsonFloat) && (e.type == jsonInteger || e.type == jsonFloat)) {
        *value = JSUtil.toString(as<double>() * e.as<double>());
        if (type == jsonInteger && fmod(as<double>(), 1) != 0.0) {
            type = jsonFloat;
        }
    }
    return *this;
}

JSType::Element &JSType::Element::operator*=(const int8_t &e) {
    return operator*=((double)e);
}

JSType::Element &JSType::Element::operator*=(const int16_t &e) {
    return operator*=((double)e);
}

JSType::Element &JSType::Element::operator*=(const int32_t &e) {
    return operator*=((double)e);
}

JSType::Element &JSType::Element::operator*=(const uint8_t &e) {
    return operator*=((double)e);
}

JSType::Element &JSType::Element::operator*=(const uint16_t &e) {
    return operator*=((double)e);
}

JSType::Element &JSType::Element::operator*=(const uint32_t &e) {
    return operator*=((double)e);
}

JSType::Element &JSType::Element::operator*=(const float &e) {
    return operator*=((double)e);
}

JSType::Element &JSType::Element::operator*=(const double &e) {
    if (type == jsonInteger || type == jsonFloat) {
        *value = JSUtil.toString(as<double>() * e);
        if (type == jsonInteger && fmod(as<double>(), 1) != 0.0) {
            type = jsonFloat;
        }
    }
    return *this;
}

JSType::Element &JSType::Element::operator/=(const Element &e) {
    if (value == NULL || e.value == NULL) return *this;
    if ((type == jsonInteger || type == jsonFloat) && (e.type == jsonInteger || e.type == jsonFloat)) {
        *value = JSUtil.toString(as<double>() / e.as<double>());
        if (type == jsonInteger && fmod(as<double>(), 1) != 0.0) {
            type = jsonFloat;
        }
    }
    return *this;
}

JSType::Element &JSType::Element::operator/=(const int8_t &e) {
    return operator/=((double)e);
}

JSType::Element &JSType::Element::operator/=(const int16_t &e) {
    return operator/=((double)e);
}

JSType::Element &JSType::Element::operator/=(const int32_t &e) {
    return operator/=((double)e);
}

JSType::Element &JSType::Element::operator/=(const uint8_t &e) {
    return operator/=((double)e);
}

JSType::Element &JSType::Element::operator/=(const uint16_t &e) {
    return operator/=((double)e);
}

JSType::Element &JSType::Element::operator/=(const uint32_t &e) {
    return operator/=((double)e);
}

JSType::Element &JSType::Element::operator/=(const float &e) {
    return operator/=((double)e);
}

JSType::Element &JSType::Element::operator/=(const double &e) {
    if (type == jsonInteger || type == jsonFloat) {
        *value = JSUtil.toString(as<double>() / e);
        if (type == jsonInteger && fmod(as<double>(), 1) != 0.0) {
            type = jsonFloat;
        }
    }
    return *this;
}

JSType::Element &JSType::Element::operator%=(const Element &e) {
    if (value == NULL || e.value == NULL) return *this;
    switch (type) {
        case jsonInteger:
            if (e.type == jsonInteger) {
                *value = String(as<int32_t>() % e.as<int32_t>());
            } else if (e.type == jsonFloat) {
                *value = JSUtil.toString(fmod(as<double>(), e.as<double>()));
                type = jsonFloat;
            }
            break;
        case jsonFloat:
            if (e.type == jsonInteger || e.type == jsonFloat) {
                *value = JSUtil.toString(fmod(as<double>(), e.as<double>()));
            }
            break;
    }
    return *this;
}

JSType::Element &JSType::Element::operator%=(const int8_t &e) {
    return operator%=((int32_t)e);
}

JSType::Element &JSType::Element::operator%=(const int16_t &e) {
    return operator%=((int32_t)e);
}

JSType::Element &JSType::Element::operator%=(const int32_t &e) {
    switch (type) {
        case jsonInteger:
            *value = String(as<int32_t>() % e);
            break;
        case jsonFloat:
            *value = JSUtil.toString(fmod(as<double>(), (double)e));
            break;
    }
    return *this;
}

JSType::Element &JSType::Element::operator%=(const uint8_t &e) {
    return operator%=((uint32_t)e);
}

JSType::Element &JSType::Element::operator%=(const uint16_t &e) {
    return operator%=((uint32_t)e);
}

JSType::Element &JSType::Element::operator%=(const uint32_t &e) {
    switch (type) {
        case jsonInteger:
            *value = String(as<int32_t>() % e);
            break;
        case jsonFloat:
            *value = JSUtil.toString(fmod(as<double>(), (double)e));
            break;
    }
    return *this;
}

JSType::Element &JSType::Element::operator%=(const float &e) {
    return operator%=((double)e);
}

JSType::Element &JSType::Element::operator%=(const double &e) {
    if (type == jsonInteger || type == jsonFloat) {
        *value = JSUtil.toString(fmod(as<double>(), e));
        type = jsonFloat;
    }
    return *this;
}

JSType::Element &JSType::Element::operator++() {
    if (type == jsonInteger || type == jsonFloat) {
        *value = JSUtil.toString(as<double>() + 1);
    }
    return *this;
}

JSType::Element JSType::Element::operator++(int) {
    Element e = *this;
    if (type == jsonInteger || type == jsonFloat) {
        *value = JSUtil.toString(as<double>() + 1);
    }
    return e;
}

JSType::Element &JSType::Element::operator--() {
    if (type == jsonInteger || type == jsonFloat) {
        *value = JSUtil.toString(as<double>() - 1);
    }
    return *this;
}

JSType::Element JSType::Element::operator--(int) {
    Element e = *this;
    if (type == jsonInteger || type == jsonFloat) {
        *value = JSUtil.toString(as<double>() - 1);
    }
    return e;
}

int32_t JSType::Element::toInt() const {
    return *this;
}

uint32_t JSType::Element::toUInt() const {
    return *this;
}

float JSType::Element::toFloat() const {
    return *this;
}

double JSType::Element::toDouble() const {
    return *this;
}

bool JSType::Element::toBool() const {
    return *this;
}

String JSType::Element::toString() const {
    return *this;
}

Json &JSType::Element::toJson() const {
    return *this;
}

JsonArray &JSType::Element::toArray() const {
    return *this;
}

char *JSType::Element::c_str() const {
    return (char *)*this;
}

JSType::Type JSType::Element::getType() const {
    return type;
}

String JSType::Element::getTypeName() const {
    return JSUtil.typeToString(type);
}

size_t JSType::Element::size() const {
    switch (type) {
        case jsonObject:
            if (object) {
                return object->size();
            } else if (value) {
                return Json(*value).size();
            } else {
                return 0;
            }
        case jsonArray:
            if (array) {
                return array->size();
            } else if (value) {
                return JsonArray(*value).size();
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

size_t JSType::Element::lastIndex() const {
    size_t len = size();
    return len == 0 ? 0 : len - 1;
}

void JSType::Element::remove(const size_t &index, const size_t &count) {
    if (type == jsonString) {
        if (value) {
            value->remove(index, count);
        }
    } else if (type == jsonArray) {
        as<JsonArray &>().remove(index, count);
    }
}

void JSType::Element::remove(const String &key) {
    if (type == jsonObject) {
        as<Json &>().remove(key);
    }
}

void JSType::Element::clear() {
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
        case jsonInteger:
        case jsonFloat:
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

bool JSType::Element::contains(const Element &e) const {
    switch (type) {
        case jsonObject:
            if (value) {
                return Json(*value).contains(e);
            } else if (object) {
                return object->contains(e);
            } else {
                return false;
            }
        case jsonArray:
            if (value) {
                return JsonArray(*value).contains(e);
            } else if (array) {
                return array->contains(e);
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

bool JSType::Element::isNull() const {
    return type == jsonNull;
}

bool JSType::Element::isEmpty() const {
    switch (type) {
        case jsonObject:
            if (value) {
                return value->isEmpty();
            } else if (object) {
                return object->isEmpty();
            } else {
                return true;
            }
        case jsonArray:
            if (value) {
                return value->isEmpty();
            } else if (array) {
                return array->isEmpty();
            } else {
                return true;
            }
        case jsonString:
            return value == NULL || value->isEmpty();
        case jsonInteger:
        case jsonFloat:
        case jsonBoolean:
            return value == NULL;
        default:
            return true;
    }
}

bool JSType::Element::isNotEmpty() const {
    return !isEmpty();
}

void JSType::Element::release() {
    if (value) delete value;
    if (object) delete object;
    if (array) delete array;
    value = NULL;
    object = NULL;
    array = NULL;
}

int JSType::Element::compareTo(const Element &e) const {
    switch (type) {
        case jsonString:
            if (value->equals(*e.value)) {
                return 0;
            } else {
                return value->compareTo(*e.value) > 0 ? 1 : -1;
            }
        case jsonInteger:
        case jsonFloat:
            if (e.type == jsonInteger || e.type == jsonFloat) {
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

/**--- Json Utility ---**/
void JsonUtil::parse(const char *str, Json *doc, JsonArray *v) {
    if (!str) return;
    if (!doc && !v) return;

    String s(str);
    s.trim();

    if (s.startsWith("{") && s.endsWith("}")) {
        if (!doc) {
            log(JsonUtil::LogLevel::Error, F("Json"), F("Object container not set"));
            return;
        } else {
            if (s.length() == 2) {
                log(JsonUtil::LogLevel::Warning, F("Json"), F("string contains empty object"));
                return;
            }
        }
    } else if (s.startsWith("[") && s.endsWith("]")) {
        if (!v) {
            log(JsonUtil::LogLevel::Error, F("Json"), F("Array container not set"));
            return;
        } else {
            if (s.length() == 2) {
                log(JsonUtil::LogLevel::Warning, F("Json"), F("string contains empty array"));
                return;
            }
        }
    } else {
        if (s.length() > 0) {
            if (!s.startsWith("{") || !s.startsWith("[")) {
                log(JsonUtil::LogLevel::Error, F("Json"), String(F("Expected '{' or '[' at the beginning of string, found '")) + s[0] + String(F("'")));
            } else {
                log(JsonUtil::LogLevel::Error, F("Json"), String(F("Expected '}' or ']' at the end of string, found '")) + s[s.length() - 1] + String(F("'")));
            }
        } else {
            log(JsonUtil::LogLevel::Error, F("Json"), F("String cannot be empty"));
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
                    log(JsonUtil::LogLevel::Error, F("Json"), String(F("Expected ',' after node-value, found '")) + s[0] + String(F("'")));
                } else {
                    log(JsonUtil::LogLevel::Error, F("Json"), String(F("Expected ',' after array element, found '")) + s[0] + String(F("'")));
                }
                return;
            }
            first = false;
        }

        String key = "";
        if (doc) {
            if (!s.startsWith("\"")) {
                log(JsonUtil::LogLevel::Error, F("Json"), String(F("Expected '\"' after ',', found '")) + s[0] + String(F("'")));
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
                log(JsonUtil::LogLevel::Error, F("Json"), F("Node-name must be enclosed with '\"'"));
                return;
            }
            if (!s.startsWith(":")) {
                log(JsonUtil::LogLevel::Error, F("Json"), String(F("Expected ':' before node-value, found '")) + s[0] + String(F("' (node-name: ")) + key + String(F(")")));
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
                        doc->add(key, JSType::Element(s.substring(1, i), JSType::jsonString));
                    } else {
                        v->push(JSType::Element(s.substring(1, i), JSType::jsonString));
                    }
                    remove(s, i + 1);
                    s.trim();
                    found = true;
                    break;
                }
            }
            if (!found) {
                if (doc) {
                    log(JsonUtil::LogLevel::Error, F("Json"), String(F("Node-value with type 'string' must be enclosed with '\"\"' (node-name: ")) + key + String(F(")")));
                } else {
                    log(JsonUtil::LogLevel::Error, F("Json"), String(F("Array element with type 'string' must be enclosed with '\"\"'")));
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
                        doc->add(key, JSType::Element(s.substring(0, i + 1), JSType::jsonObject));
                    } else {
                        v->push(JSType::Element(s.substring(0, i + 1), JSType::jsonObject));
                    }
                    remove(s, i + 1);
                    s.trim();
                    break;
                }
            }
            if (x != 0) {
                if (doc) {
                    log(JsonUtil::LogLevel::Error, F("Json"), String(F("Node-value with type 'object' must be enclosed with '{}' (node-name: ")) + key + String(F(")")));
                } else {
                    log(JsonUtil::LogLevel::Error, F("Json"), String(F("Array element with type 'object' must be enclosed with '{}'")));
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
                        doc->add(key, JSType::Element(s.substring(0, i + 1), JSType::jsonArray));
                    } else {
                        v->push(JSType::Element(s.substring(0, i + 1), JSType::jsonArray));
                    }
                    remove(s, i + 1);
                    s.trim();
                    break;
                }
            }
            if (x != 0) {
                if (doc) {
                    log(JsonUtil::LogLevel::Error, F("Json"), String(F("Node-value with type 'array' must be enclosed with '[]' (node-name: ")) + key + String(F(")")));
                } else {
                    log(JsonUtil::LogLevel::Error, F("Json"), String(F("Array element with type 'array' must be enclosed with '[]'")));
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
                    doc->add(key, JSType::Element(buf, buf.equals("null") ? JSType::jsonNull : JSType::jsonBoolean));
                } else {
                    v->push(JSType::Element(buf, buf.equals("null") ? JSType::jsonNull : JSType::jsonBoolean));
                }
            } else {
                if (buf.length() > 1) {
                    if (buf.startsWith("0") && !String(buf[1]).equals(".")) {
                        if (doc) {
                            log(JsonUtil::LogLevel::Error, F("Json"), String(F("Node-value with type 'number' cannot be started with '0' (node-name: ")) + key + String(F(")")));
                        } else {
                            log(JsonUtil::LogLevel::Error, F("Json"), String(F("Array element with type 'number' cannot be started with '0'")));
                        }
                        return;
                    } else if (!isdigit(buf[0]) && !String(buf[0]).equals("-")) {
                        if (doc) {
                            log(JsonUtil::LogLevel::Error, F("Json"), String(F("Node-value with type 'literal' can only contain 'true', 'false', 'null', or 'number' (node-name: ")) + key + String(F(")")));
                        } else {
                            log(JsonUtil::LogLevel::Error, F("Json"), String(F("Array element with type 'literal' can only contain 'true', 'false', 'null', or 'number'")));
                        }
                        return;
                    }
                } else {
                    if (buf.isEmpty() || !isdigit(buf[0])) {
                        if (doc) {
                            log(JsonUtil::LogLevel::Error, F("Json"), String(F("Node-value with type 'literal' can only contain 'true', 'false', 'null', or 'number' (node-name: ")) + key + String(F(")")));
                        } else {
                            log(JsonUtil::LogLevel::Error, F("Json"), String(F("Array element with type 'literal' can only contain 'true', 'false', 'null', or 'number'")));
                        }
                        return;
                    }
                }
                int d = 0, e = 0;
                for (int i = 0; i < buf.length(); i++) {
                    if (String(buf[i]).equals(".")) {
                        if (++d > 1 || e > 0) {
                            if (doc) {
                                log(JsonUtil::LogLevel::Error, F("Json"), String(F("Invalid number format (node-name: ")) + key + String(F(")")));
                            } else {
                                log(JsonUtil::LogLevel::Error, F("Json"), String(F("Invalid number format")));
                            }
                            return;
                        }
                    } else if (String(buf[i]).equals("e") || String(buf[i]).equals("E")) {
                        if (++e > 1) {
                            if (doc) {
                                log(JsonUtil::LogLevel::Error, F("Json"), String(F("Invalid number format (node-name: ")) + key + String(F(")")));
                            } else {
                                log(JsonUtil::LogLevel::Error, F("Json"), String(F("Invalid number format")));
                            }
                            return;
                        }
                    } else if (String(buf[i]).equals("-") || String(buf[i]).equals("+")) {
                        if (i != 0 && !(String(buf[i - 1]).equals("e") || String(buf[i - 1]).equals("E"))) {
                            if (doc) {
                                log(JsonUtil::LogLevel::Error, F("Json"), String(F("Invalid number format (node-name: ")) + key + String(F(")")));
                            } else {
                                log(JsonUtil::LogLevel::Error, F("Json"), String(F("Invalid number format")));
                            }
                            return;
                        }
                    } else if (!isdigit(buf[i])) {
                        if (doc) {
                            log(JsonUtil::LogLevel::Error, F("Json"), String(F("Invalid number format (node-name: ")) + key + String(F(")")));
                        } else {
                            log(JsonUtil::LogLevel::Error, F("Json"), String(F("Invalid number format")));
                        }
                        return;
                    }
                }
                if (doc) {
                    doc->add(key, JSType::Element(buf, isFloat(buf) ? JSType::jsonFloat : JSType::jsonInteger));
                } else {
                    v->push(JSType::Element(buf, isFloat(buf) ? JSType::jsonFloat : JSType::jsonInteger));
                }
            }
        }
    }
}

void JsonUtil::prettyPrint(HardwareSerial &serial, Json &doc, const uint8_t &spaceTab) {
    String s = doc.toString();
    prettyPrint(serial, &doc, NULL, spaceTab, 0);
}

void JsonUtil::prettyPrint(HardwareSerial &serial, JsonArray &v, const uint8_t &spaceTab) {
    prettyPrint(serial, NULL, &v, spaceTab, 0);
}

void JsonUtil::prettyPrint(HardwareSerial &serial, Json *doc, JsonArray *v, const uint8_t &spaceTab, const uint8_t &space) {
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
            case JSType::jsonObject: {
                if (doc) {
                    prettyPrint(serial, &doc->getElement(i).as<Json &>(), NULL, spaceTab, space + 1);
                } else {
                    prettyPrint(serial, &v->getElement(i).as<Json &>(), NULL, spaceTab, space + 1);
                }
                break;
            }
            case JSType::jsonArray: {
                if (doc) {
                    prettyPrint(serial, NULL, &doc->getElement(i).as<JsonArray &>(), spaceTab, space + 1);
                } else {
                    prettyPrint(serial, NULL, &v->getElement(i).as<JsonArray &>(), spaceTab, space + 1);
                }
                break;
            }
            case JSType::jsonString: {
                if (doc) {
                    serial.print("\"" + doc->getElement(i).as<String>() + "\"");
                } else {
                    serial.print("\"" + v->getElement(i).as<String>() + "\"");
                }
                break;
            }
            case JSType::jsonInteger:
            case JSType::jsonBoolean:
            case JSType::jsonFloat:
            case JSType::jsonNull: {
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

bool JsonUtil::isFloat(const String &str) {
    return fmod(str.toDouble(), 1) != 0;
}

String JsonUtil::typeToString(const JSType::Type &type) {
    switch (type) {
        case JSType::jsonObject:
            return "object";
        case JSType::jsonArray:
            return "array";
        case JSType::jsonString:
            return "string";
        case JSType::jsonInteger:
            return "integer";
        case JSType::jsonFloat:
            return "float";
        case JSType::jsonBoolean:
            return "boolean";
        case JSType::jsonNull:
            return "null";
        default:
            return "undefined";
    }
}

String JsonUtil::removeInsignificantZeros(const String &str) {
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

String JsonUtil::toString(const float &value) {
    return removeInsignificantZeros(String(value, 5));
}

String JsonUtil::toString(const double &value) {
    return removeInsignificantZeros(String(value, 11));
}

void JsonUtil::remove(String &str, const int &len, const bool &fromEnd) {
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

void JsonUtil::attachDebugger(HardwareSerial &serial) {
    this->serial = &serial;
}

void JsonUtil::detachDebugger() {
    this->serial = NULL;
}

void JsonUtil::log(const LogLevel &level, const String &tag, const String &message) {
    if (serial) {
        static String severity[] = {"[E]: ", "[I]: ", "[W]: "};
        serial->println(severity[static_cast<int>(level)] + tag + ": " + message);
    }
}

JsonUtil JSUtil;

/*--- Json Array ---*/
JsonArray::JsonArray() : v(std::vector<JSType::Element>()) {}

JsonArray::JsonArray(const char *str) : v(std::vector<JSType::Element>()) {
    JSUtil.parse(str, NULL, this);
}

JsonArray::JsonArray(const String &str) : v(std::vector<JSType::Element>()) {
    JSUtil.parse(str.c_str(), NULL, this);
}

JsonArray::JsonArray(const JsonArray &other) : v(other.v) {}

JsonArray::JsonArray(std::vector<int> &arr) : v(std::vector<JSType::Element>()) {
    for (int i : arr) push(i);
}

JsonArray &JsonArray::push(const JSType::Element &value) {
    v.push_back(value);
    return *this;
}

JsonArray &JsonArray::push() {
    push(JSType::Element());
    return *this;
}

JSType::Element &JsonArray::operator[](const uint16_t &index) {
    if (index >= v.size()) {
        _emptyElement = JSType::Element();
        return _emptyElement;
    }
    return v.at(index);
}

JSType::Element &JsonArray::getElement(const uint16_t &index) {
    if (index >= v.size()) {
        _emptyElement = JSType::Element();
        return _emptyElement;
    }
    return v[index];
}

const JSType::Element &JsonArray::operator[](const uint16_t &index) const {
    if (index >= v.size()) {
        _emptyElement = JSType::Element();
        return _emptyElement;
    }
    return v.at(index);
}

const JSType::Element &JsonArray::getElement(const uint16_t &index) const {
    if (index >= v.size()) {
        _emptyElement = JSType::Element();
        return _emptyElement;
    }
    return v.at(index);
}

bool JsonArray::operator==(const JsonArray &other) const {
    return v == other.v;
}

JSType::Type JsonArray::getType(const uint16_t &index) const {
    if (index >= v.size()) {
        return JSType::jsonNull;
    }
    return v[index].getType();
}

String JsonArray::getTypeName(const uint16_t &index) const {
    if (index >= v.size()) {
        return JSUtil.typeToString(JSType::jsonNull);
    }
    return JSUtil.typeToString(v[index].getType());
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
        str += (v[i].getType() == JSType::jsonString ? "\"" : "") + v[i].as<String>() + (v[i].getType() == JSType::jsonString ? "\"" : "");
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

bool JsonArray::contains(const JSType::Element &e) const {
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

std::vector<JSType::Element>::iterator JsonArray::begin() {
    return v.begin();
}

std::vector<JSType::Element>::iterator JsonArray::end() {
    return v.end();
}

/*--- Json ---*/
Json::Json()
    : doc(std::map<String, JSType::Element>()), indexList(std::map<uint16_t, String>()), counter(0) {}

Json::Json(const char *str)
    : doc(std::map<String, JSType::Element>()), indexList(std::map<uint16_t, String>()), counter(0) {
    JSUtil.parse(str, this, NULL);
}

Json::Json(const String &str)
    : doc(std::map<String, JSType::Element>()), indexList(std::map<uint16_t, String>()), counter(0) {
    JSUtil.parse(str.c_str(), this, NULL);
}

Json::Json(const Json &other)
    : doc(other.doc), indexList(other.indexList), counter(other.counter) {}

Json &Json::add(const String &name, const JSType::Element &value) {
    if (!contains(name)) {
        doc.insert(std::make_pair(name, value));
        indexList.insert(std::make_pair(counter++, name));
    } else {
        getElement(name) = value;
    }
    return *this;
}

Json &Json::add(const String &name) {
    add(name, JSType::Element());
    return *this;
}

JSType::Element &Json::operator[](const String &name) {
    return getElement(name);
}

JSType::Element &Json::operator[](const uint16_t &index) {
    return getElement(index);
}

JSType::Element &Json::getElement(const String &name) {
    if (!contains(name)) {
        add(name);
    }
    return doc.at(name);
}

JSType::Element &Json::getElement(const uint16_t &index) {
    if (index >= counter) {
        _emptyElement = JSType::Element();
        return _emptyElement;
    }
    return doc.at(indexList.at(index));
}

const JSType::Element &Json::operator[](const String &name) const {
    if (!contains(name)) {
        _emptyElement = JSType::Element();
        return _emptyElement;
    }
    return doc.at(name);
}

const JSType::Element &Json::operator[](const uint16_t &index) const {
    if (index >= counter) {
        _emptyElement = JSType::Element();
        return _emptyElement;
    }
    return doc.at(indexList.at(index));
}

const JSType::Element &Json::getElement(const String &name) const {
    if (!contains(name)) {
        _emptyElement = JSType::Element();
        return _emptyElement;
    }
    return doc.at(name);
}

const JSType::Element &Json::getElement(const uint16_t &index) const {
    if (index >= counter) {
        _emptyElement = JSType::Element();
        return _emptyElement;
    }
    return doc.at(indexList.at(index));
}

bool Json::operator==(const Json &other) const {
    return doc == other.doc;
}

JSType::Type Json::getType(const String &name) const {
    if (!contains(name)) {
        return JSType::jsonNull;
    }
    return doc.at(name).getType();
}

JSType::Type Json::getType(const uint16_t &index) const {
    if (index >= counter) {
        return JSType::jsonNull;
    }
    return doc.at(indexList.at(index)).getType();
}

String Json::getTypeName(const String &name) const {
    if (!contains(name)) {
        return JSUtil.typeToString(JSType::jsonNull);
    }
    return JSUtil.typeToString(doc.at(name).getType());
}

String Json::getTypeName(const uint16_t &index) const {
    if (index >= counter) {
        return JSUtil.typeToString(JSType::jsonNull);
    }
    return JSUtil.typeToString(doc.at(indexList.at(index)).getType());
}

String Json::getKey(const uint16_t &index) const {
    if (index >= counter) {
        return "";
    }
    return indexList.at(index);
}

int16_t Json::indexOf(const String &name) const {
    if (!contains(name)) {
        return -1;
    }
    for (int i = 0; i < indexList.size(); i++) {
        if (indexList.at(i).equals(name)) {
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
        result += (getElement(i).getType() == JSType::jsonString ? "\"" : "") + getElement(i).as<String>() + (getElement(i).getType() == JSType::jsonString ? "\"" : "");
    }
    result += "}";
    return result;
}

size_t Json::size() const {
    return doc.size();
}

bool Json::contains(const String &name) const {
    return doc.count(name) > 0;
}

bool Json::isEmpty() const {
    return doc.empty();
}

bool Json::isNotEmpty() const {
    return !doc.empty();
}

void Json::remove(const String &name) {
    if (contains(name)) {
        int i = indexOf(name);
        if (i >= 0) {
            doc.erase(name);
            indexList.erase(i);
            arrangeIndex(i);
        }
    }
}

void Json::clear() {
    doc.clear();
    indexList.clear();
    counter = 0;
}

std::map<String, JSType::Element>::iterator Json::begin() {
    return doc.begin();
}

std::map<String, JSType::Element>::iterator Json::end() {
    return doc.end();
}

void Json::arrangeIndex(uint16_t index) {
    counter = 0;
    std::map<uint16_t, String> buf;
    for (int i = 0; i < indexList.size() + 1; i++) {
        if (i != index) {
            buf.insert(std::make_pair(counter++, indexList[i]));
        }
    }
    indexList = buf;
}
