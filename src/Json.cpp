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
            if (e.object)
                object = new Json(e.object->toString());
            else if (e.value)
                object = new Json(*e.value);
            else
                object = new Json();
            break;
        case jsonArray:
            if (e.array)
                array = new JsonArray(e.array->toString());
            else if (e.value)
                array = new JsonArray(*e.value);
            else
                array = new JsonArray();
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
    : value(NULL), type(jsonObject), object(new Json(value.toString())), array(NULL) {}

JSType::Element::Element(const JsonArray &value)
    : value(NULL), type(jsonArray), object(NULL), array(new JsonArray(value.toString())) {}

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
    if (!value) return false;
    return (*value).equals("true");
}

JSType::Element::operator void *() const {
    return NULL;
}

JSType::Element::operator String() const {
    switch (type) {
        case jsonObject: {
            if (object)
                return object->toString();
            else if (value)
                return *value;
            else
                return "";
        }
        case jsonArray: {
            if (array)
                return array->toString();
            else if (value)
                return *value;
            else
                return "";
        }
        default:
            return *value;
    }
}

JSType::Element::operator Json &() {
    if (!object) {
        if (value && type == jsonObject) {
            object = new Json(*value);
            delete value;
            value = NULL;
        } else {
            object = new Json();
        }
    }
    return *object;
}

JSType::Element::operator JsonArray &() {
    if (!array) {
        if (value && type == jsonArray) {
            array = new JsonArray(*value);
            delete value;
            value = NULL;
        } else {
            array = new JsonArray();
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
            if (e.object)
                object = new Json(e.object->toString());
            else if (e.value)
                object = new Json(*e.value);
            else
                object = new Json();
            break;
        case jsonArray:
            if (e.array)
                array = new JsonArray(e.array->toString());
            else if (e.value)
                array = new JsonArray(*e.value);
            else
                array = new JsonArray();
            break;
        default:
            value = new String(*e.value);
            break;
    }
    return *this;
}

JSType::Type JSType::Element::getType() const {
    return type;
}

String JSType::Element::toString() {
    return *this;
}

bool JSType::Element::operator==(const Element &e) const {
    if (type != e.getType()) return false;
    switch (type) {
        case jsonObject:
            return object->toString().equals(e.object->toString());
        case jsonArray:
            return array->toString().equals(e.array->toString());
        default:
            return value->equals(*e.value);
    }
}

void JSType::Element::release() {
    if (value) delete value;
    if (object) delete object;
    if (array) delete array;
    value = NULL;
    object = NULL;
    array = NULL;
}

/**--- Json Utility ---**/
void JsonUtil::parse(const char *str, Json *doc, JsonArray *v) {
    FlashString *s = new FlashString(str);
    s->trim();

    if (s->startsWith("{") && s->endsWith("}")) {
        if (!doc) {
            fb_log.e(F("Json"), F("Object container not set"));
            return;
        } else {
            if (s->length() == 2) {
                fb_log.i(F("Json"), F("string contains empty object"));
                return;
            }
        }
    } else if (s->startsWith("[") && s->endsWith("]")) {
        if (!v) {
            fb_log.e(F("Json"), F("Array container not set"));
            return;
        } else {
            if (s->length() == 2) {
                fb_log.i(F("Json"), F("string contains empty array"));
                return;
            }
        }
    } else {
        if (s->length() > 0) {
            if (!s->startsWith("{") || !s->startsWith("[")) {
                fb_log.e(F("Json"), String(F("Expected '{' or '[' at the beginning of string, found '")) + s->get(0) + String(F("'")));
            } else {
                fb_log.e(F("Json"), String(F("Expected '}' or ']' at the end of string, found '")) + s->get(s->length() - 1) + String(F("'")));
            }
        } else {
            fb_log.e(F("Json"), F("String cannot be empty"));
        }
        return;
    }

    s->remove(1);
    s->remove(1, true);
    s->trim();

    bool first = true;
    while (s->length() > 0) {
        if (s->startsWith(",")) {
            s->remove(1);
            s->trim();
        } else {
            if (!first) {
                if (doc) {
                    fb_log.e(F("Json"), String(F("Expected ',' after node-value, found '")) + s->get(0) + String(F("'")));
                } else {
                    fb_log.e(F("Json"), String(F("Expected ',' after array element, found '")) + s->get(0) + String(F("'")));
                }
                return;
            }
            first = false;
        }

        String key = "";
        if (doc) {
            if (!s->startsWith("\"")) {
                fb_log.e(F("Json"), String(F("Expected '\"' after ',', found '")) + s->get(0) + String(F("'")));
                return;
            }
            bool found = false;
            for (int i = 1; i < s->length(); i++) {
                if (s->get(i).equals("\"") && !s->get(i - 1).equals("\\")) {
                    key = s->substring(1, i);
                    s->remove(i + 1);
                    s->trim();
                    found = true;
                    break;
                }
            }
            if (!found) {
                fb_log.e(F("Json"), F("Node-name must be enclosed with '\"'"));
                return;
            }
            if (!s->startsWith(":")) {
                fb_log.e(F("Json"), String(F("Expected ':' before node-value, found '")) + s->get(0) + String(F("' (node-name: ")) + key + String(F(")")));
                return;
            }
            s->remove(1);
            s->trim();
        }

        if (s->startsWith("\"")) {
            bool found = false;
            for (int i = 1; i < s->length(); i++) {
                if (s->get(i).equals("\"") && !s->get(i - 1).equals("\\")) {
                    if (doc) {
                        doc->add(key, s->substring(1, i), JSType::jsonString);
                    } else {
                        v->push(s->substring(1, i), JSType::jsonString);
                    }
                    s->remove(i + 1);
                    s->trim();
                    found = true;
                    break;
                }
            }
            if (!found) {
                if (doc) {
                    fb_log.e(F("Json"), String(F("Node-value with type 'string' must be enclosed with '\"\"' (node-name: ")) + key + String(F(")")));
                } else {
                    fb_log.e(F("Json"), String(F("Array element with type 'string' must be enclosed with '\"\"'")));
                }
                return;
            }
        } else if (s->startsWith("{")) {
            int x = 1;
            for (int i = 1; i < s->length(); i++) {
                if (s->get(i).equals("{")) {
                    x++;
                } else if (s->get(i).equals("}")) {
                    x--;
                }
                if (x == 0) {
                    if (doc) {
                        doc->add(key, s->substring(0, i + 1), JSType::jsonObject);
                    } else {
                        v->push(s->substring(0, i + 1), JSType::jsonObject);
                    }
                    s->remove(i + 1);
                    s->trim();
                    break;
                }
            }
            if (x != 0) {
                if (doc) {
                    fb_log.e(F("Json"), String(F("Node-value with type 'object' must be enclosed with '{}' (node-name: ")) + key + String(F(")")));
                } else {
                    fb_log.e(F("Json"), String(F("Array element with type 'object' must be enclosed with '{}'")));
                }
                return;
            }
        } else if (s->startsWith("[")) {
            int x = 1;
            for (int i = 1; i < s->length(); i++) {
                if (s->get(i).equals("[")) {
                    x++;
                } else if (s->get(i).equals("]")) {
                    x--;
                }
                if (x == 0) {
                    if (doc) {
                        doc->add(key, s->substring(0, i + 1), JSType::jsonArray);
                    } else {
                        v->push(s->substring(0, i + 1), JSType::jsonArray);
                    }
                    s->remove(i + 1);
                    s->trim();
                    break;
                }
            }
            if (x != 0) {
                if (doc) {
                    fb_log.e(F("Json"), String(F("Node-value with type 'array' must be enclosed with '[]' (node-name: ")) + key + String(F(")")));
                } else {
                    fb_log.e(F("Json"), String(F("Array element with type 'array' must be enclosed with '[]'")));
                }
                return;
            }
        } else {
            String buf = s->substring(0, s->indexOf(","));
            buf.trim();
            s->remove(s->indexOf(","));
            s->trim();
            if (buf.equals("true") || buf.equals("false") || buf.equals("null")) {
                if (doc) {
                    doc->add(key, buf, buf.equals("null") ? JSType::jsonNull : JSType::jsonBoolean);
                } else {
                    v->push(buf, buf.equals("null") ? JSType::jsonNull : JSType::jsonBoolean);
                }
            } else {
                if (buf.length() > 1) {
                    if (buf.startsWith("0") && !String(buf[1]).equals(".")) {
                        if (doc) {
                            fb_log.e(F("Json"), String(F("Node-value with type 'number' cannot be started with '0' (node-name: ")) + key + String(F(")")));
                        } else {
                            fb_log.e(F("Json"), String(F("Array element with type 'number' cannot be started with '0'")));
                        }
                        return;
                    } else if (!isdigit(buf[0]) && !String(buf[0]).equals("-")) {
                        if (doc) {
                            fb_log.e(F("Json"), String(F("Node-value with type 'literal' can only contain 'true', 'false', 'null', or 'number' (node-name: ")) + key + String(F(")")));
                        } else {
                            fb_log.e(F("Json"), String(F("Array element with type 'literal' can only contain 'true', 'false', 'null', or 'number'")));
                        }
                        return;
                    }
                } else {
                    if (buf.isEmpty() || !isdigit(buf[0])) {
                        if (doc) {
                            fb_log.e(F("Json"), String(F("Node-value with type 'literal' can only contain 'true', 'false', 'null', or 'number' (node-name: ")) + key + String(F(")")));
                        } else {
                            fb_log.e(F("Json"), String(F("Array element with type 'literal' can only contain 'true', 'false', 'null', or 'number'")));
                        }
                        return;
                    }
                }
                int d = 0, e = 0;
                for (int i = 0; i < buf.length(); i++) {
                    if (String(buf[i]).equals(".")) {
                        if (++d > 1 || e > 0) {
                            if (doc) {
                                fb_log.e(F("Json"), String(F("Invalid number format (node-name: ")) + key + String(F(")")));
                            } else {
                                fb_log.e(F("Json"), String(F("Invalid number format")));
                            }
                            return;
                        }
                    } else if (String(buf[i]).equals("e") || String(buf[i]).equals("E")) {
                        if (++e > 1) {
                            if (doc) {
                                fb_log.e(F("Json"), String(F("Invalid number format (node-name: ")) + key + String(F(")")));
                            } else {
                                fb_log.e(F("Json"), String(F("Invalid number format")));
                            }
                            return;
                        }
                    } else if (String(buf[i]).equals("-") || String(buf[i]).equals("+")) {
                        if (i != 0 && !(String(buf[i - 1]).equals("e") || String(buf[i - 1]).equals("E"))) {
                            if (doc) {
                                fb_log.e(F("Json"), String(F("Invalid number format (node-name: ")) + key + String(F(")")));
                            } else {
                                fb_log.e(F("Json"), String(F("Invalid number format")));
                            }
                            return;
                        }
                    } else if (!isdigit(buf[i])) {
                        if (doc) {
                            fb_log.e(F("Json"), String(F("Invalid number format (node-name: ")) + key + String(F(")")));
                        } else {
                            fb_log.e(F("Json"), String(F("Invalid number format")));
                        }
                        return;
                    }
                }
                if (doc) {
                    doc->add(key, buf, isFloat(buf) ? JSType::jsonFloat : JSType::jsonInteger);
                } else {
                    v->push(buf, isFloat(buf) ? JSType::jsonFloat : JSType::jsonInteger);
                }
            }
        }
    }
    if (s) delete s;
}

void JsonUtil::prettyPrint(HardwareSerial &serial, Json &doc, const uint8_t &spaceTab) {
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
                Json *json = NULL;
                if (doc) {
                    json = new Json(doc->getElement(i).as<String>());
                } else {
                    json = new Json(v->getElement(i).as<String>());
                }
                prettyPrint(serial, json, NULL, spaceTab, space + 1);
                if (json) delete json;
                break;
            }
            case JSType::jsonArray: {
                JsonArray *array = NULL;
                if (doc) {
                    array = new JsonArray(doc->getElement(i).as<String>());
                } else {
                    array = new JsonArray(v->getElement(i).as<String>());
                }
                prettyPrint(serial, NULL, array, spaceTab, space + 1);
                if (array) delete array;
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
    return removeInsignificantZeros(String(value, 7));
}

String JsonUtil::toString(const double &value) {
    return removeInsignificantZeros(String(value, 15));
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

JsonArray &JsonArray::push(const String &value, const JSType::Type &type) {
#ifdef ESP32
    v.emplace_back(value, type);
#else
    v.push_back(JSType::Element(value, type));
#endif
    return *this;
}

JsonArray &JsonArray::push(const JSType::Element &value) {
    push(value.as<String>(), value.getType());
    return *this;
}

JsonArray &JsonArray::push() {
    push("null", JSType::jsonNull);
    return *this;
}

JSType::Element &JsonArray::operator[](const uint16_t &index) {
    return v.at(index);
}

JSType::Element &JsonArray::getElement(const uint16_t &index) {
    return v[index];
}

const JSType::Element &JsonArray::operator[](const uint16_t &index) const {
    return v.at(index);
}

const JSType::Element &JsonArray::getElement(const uint16_t &index) const {
    return v[index];
}

JSType::Type JsonArray::getType(const uint16_t &index) const {
    return v[index].getType();
}

String JsonArray::getTypeString(const uint16_t &index) const {
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

void JsonArray::erase(const uint16_t &index) {
    if (index < v.size()) {
        v.erase(v.begin() + index);
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

Json &Json::add(const String &name, const String &value, const JSType::Type &type) {
#ifdef ESP32
    doc.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(value, type));
    indexList.emplace(counter++, name);
#else
    doc.insert(std::make_pair(name, JSType::Element(value, type)));
    indexList.insert(std::make_pair(counter++, name));
#endif
    return *this;
}

Json &Json::add(const String &name, const JSType::Element &value) {
    add(name, value.as<String>(), value.getType());
    return *this;
}

Json &Json::add(const String &name) {
    add(name, "null", JSType::jsonNull);
    return *this;
}

JSType::Element &Json::operator[](const String &name) {
    if (!contains(name)) {
        add(name);
    }
    return doc.at(name);
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
    return doc.at(indexList.at(index));
}

const JSType::Element &Json::operator[](const String &name) const {
    return doc.at(name);
}

const JSType::Element &Json::operator[](const uint16_t &index) const {
    return doc.at(indexList.at(index));
}

const JSType::Element &Json::getElement(const String &name) const {
    return doc.at(name);
}

const JSType::Element &Json::getElement(const uint16_t &index) const {
    return doc.at(indexList.at(index));
}

JSType::Type Json::getType(const String &name) const {
    return doc.at(name).getType();
}

JSType::Type Json::getType(const uint16_t &index) const {
    return doc.at(indexList.at(index)).getType();
}

String Json::getTypeString(const String &name) const {
    return JSUtil.typeToString(doc.at(name).getType());
}

String Json::getTypeString(const uint16_t &index) const {
    return JSUtil.typeToString(doc.at(indexList.at(index)).getType());
}

String Json::getKey(const uint16_t &index) const {
    return indexList.at(index);
}

int16_t Json::getIndex(const String &name) const {
    for (int i = 0; i < indexList.size(); i++) {
        if (indexList.at(i).equals(name)) {
            return i;
        }
    }
    return -1;
}

String Json::toString() const {
    String str("{");
    bool isFirst = true;
    for (int i = 0; i < doc.size(); i++) {
        if (isFirst) {
            isFirst = false;
        } else {
            str += ",";
        }
        str += "\"" + getKey(i) + "\":";
        str += (getElement(i).getType() == JSType::jsonString ? "\"" : "") + getElement(i).as<String>() + (getElement(i).getType() == JSType::jsonString ? "\"" : "");
    }
    str += "}";
    return str;
}

size_t Json::size() const {
    return doc.size();
}

bool Json::contains(String name) const {
    return doc.count(name) > 0;
}

void Json::erase(const String &name) {
    if (contains(name)) {
        int i = getIndex(name);
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
            buf.emplace(counter++, indexList[i]);
        }
    }
    indexList = buf;
}

void Json::attachDebugger(HardwareSerial &serial) {
    fb_log.attachSerial(serial);
}

void Json::detachDebugger() {
    fb_log.detachSerial();
}