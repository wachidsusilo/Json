#include "Json.h"

/**--- Json Type ---**/
JSType::Element::Element()
    : value(NULL), type(undefined) {}

JSType::Element::Element(const String &value, const Type &type)
    : value(new String(value)), type(type) {}

JSType::Element::Element(const Element &e)
    : value(new String(e.getValue())), type(e.getType()) {}

JSType::Element::~Element() {
    if (value) delete value;
}

JSType::Type JSType::Element::getType() const {
    return type;
}

String JSType::Element::getValue() const {
    return *value;
}

bool JSType::Element::operator==(const Element &e) const {
    return value->equals(e.getValue()) && (type == e.getType());
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
            case JSType::jsonObject:
                if (doc) {
                    Json *json = new Json(doc->getElement(i).getValue());
                    prettyPrint(serial, json, NULL, spaceTab, space + 1);
                    if (json) delete json;
                } else {
                    Json *json = new Json(v->getElement(i).getValue());
                    prettyPrint(serial, json, NULL, spaceTab, space + 1);
                    if (json) delete json;
                }
                break;
            case JSType::jsonArray:
                if (doc) {
                    JsonArray *arr = new JsonArray(doc->getElement(i).getValue());
                    prettyPrint(serial, NULL, arr, spaceTab, space + 1);
                    if (arr) delete arr;
                } else {
                    JsonArray *arr = new JsonArray(v->getElement(i).getValue());
                    prettyPrint(serial, NULL, arr, spaceTab, space + 1);
                    if (arr) delete arr;
                }
                break;
            case JSType::jsonString:
                if (doc) {
                    serial.print("\"" + doc->getElement(i).getValue() + "\"");
                } else {
                    serial.print("\"" + v->getElement(i).getValue() + "\"");
                }
                break;
            case JSType::jsonInteger:
            case JSType::jsonBoolean:
            case JSType::jsonFloat:
            case JSType::jsonNull:
                if (doc) {
                    serial.print(doc->getElement(i).getValue());
                } else {
                    serial.print(v->getElement(i).getValue());
                }
                break;
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

JsonUtil JSUtil;

/*--- Json Value ---*/
JSType::Value::operator int8_t() {
    return round(value.toDouble());
}

JSType::Value::operator int16_t() {
    return round(value.toDouble());
}

JSType::Value::operator int32_t() {
    return round(value.toDouble());
}

JSType::Value::operator uint8_t() {
    return round(value.toDouble());
}

JSType::Value::operator uint16_t() {
    return round(value.toDouble());
}

JSType::Value::operator uint32_t() {
    return round(value.toDouble());
}

JSType::Value::operator float() {
    return value.toFloat();
}

JSType::Value::operator double() {
    return value.toDouble();
}

JSType::Value::operator String() {
    return value;
}

JSType::Value::operator bool() {
    return value.equals("true");
}

JSType::Value::operator void *() {
    return NULL;
}

JSType::Value::operator Json() {
    return Json(value.c_str());
}

JSType::Value::operator JsonArray() {
    return JsonArray(value.c_str());
}

/*--- Json Array ---*/
JsonArray::JsonArray() : v(std::vector<JSType::Element>()) {}

JsonArray::JsonArray(const char *str) : v(std::vector<JSType::Element>()) {
    JSUtil.parse(str, NULL, this);
}

JsonArray::JsonArray(const String &str) : v(std::vector<JSType::Element>()) {
    JSUtil.parse(str.c_str(), NULL, this);
}

JsonArray::JsonArray(std::vector<int> &arr) : v(std::vector<JSType::Element>()) {
    for (int i : arr) push(i);
}

JsonArray &JsonArray::push(const String &value, const JSType::Type &type) {
    v.emplace_back(value, type);
    return *this;
}

JsonArray &JsonArray::push(const Json &value) {
    push(value.toString(), JSType::jsonObject);
    return *this;
}

JsonArray &JsonArray::push(const JsonArray &value) {
    push(value.toString(), JSType::jsonArray);
    return *this;
}

JsonArray &JsonArray::push(const char *value) {
    push(String(value));
    return *this;
}

JsonArray &JsonArray::push(const String &value) {
    push(value, JSType::jsonString);
    return *this;
}

JsonArray &JsonArray::push(const int8_t &value) {
    push(String(value), JSType::jsonInteger);
    return *this;
}

JsonArray &JsonArray::push(const int16_t &value) {
    push(String(value), JSType::jsonInteger);
    return *this;
}

JsonArray &JsonArray::push(const int32_t &value) {
    push(String(value), JSType::jsonInteger);
    return *this;
}

JsonArray &JsonArray::push(const uint8_t &value) {
    push(String(value), JSType::jsonInteger);
    return *this;
}

JsonArray &JsonArray::push(const uint16_t &value) {
    push(String(value), JSType::jsonInteger);
    return *this;
}

JsonArray &JsonArray::push(const uint32_t &value) {
    push(String(value), JSType::jsonInteger);
    return *this;
}

JsonArray &JsonArray::push(const bool &value) {
    push(value ? "true" : "false", JSType::jsonBoolean);
    return *this;
}

JsonArray &JsonArray::push(const void *value) {
    push("null", JSType::jsonNull);
    return *this;
}

JsonArray &JsonArray::push(const float &value, const uint8_t &precision) {
    push(String(value, precision), JSType::jsonFloat);
    return *this;
}

JsonArray &JsonArray::push(const double &value, const uint8_t &precision) {
    push(String(value, precision), JSType::jsonFloat);
    return *this;
}

JSType::Value JsonArray::getValue(const uint16_t &index) const {
    if (index < v.size()) {
        return {v[index].getValue()};
    }
    return {String()};
}

JSType::Value JsonArray::operator[](const uint16_t &index) const {
    if (index < v.size()) {
        return {v[index].getValue()};
    }
    return {String()};
}

JSType::Element JsonArray::getElement(const uint16_t &index) const {
    if (index < v.size()) {
        return v[index];
    }
    return JSType::Element();
}

JSType::Type JsonArray::getType(const uint16_t &index) const {
    if (index < v.size()) {
        return v[index].getType();
    }
    return JSType::undefined;
}

String JsonArray::getTypeString(const uint16_t &index) const {
    if (index < v.size()) {
        return JSUtil.typeToString(v[index].getType());
    }
    return JSUtil.typeToString(JSType::undefined);
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
        str += (v[i].getType() == JSType::jsonString ? "\"" : "") + v[i].getValue() + (v[i].getType() == JSType::jsonString ? "\"" : "");
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

Json &Json::add(const String &name, const String &value, const JSType::Type &type) {
    doc.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(value, type));
    indexList.emplace(counter++, name);
    return *this;
}

Json &Json::add(const String &name, const Json &value) {
    add(name, value.toString(), JSType::jsonObject);
    return *this;
}

Json &Json::add(const String &name, const JsonArray &value) {
    add(name, value.toString(), JSType::jsonArray);
    return *this;
}

Json &Json::add(const String &name, const char *value) {
    add(name, String(value));
    return *this;
}

Json &Json::add(const String &name, const String &value) {
    add(name, value, JSType::jsonString);
    return *this;
}

Json &Json::add(const String &name, const int8_t &value) {
    add(name, String(value), JSType::jsonInteger);
    return *this;
}

Json &Json::add(const String &name, const int16_t &value) {
    add(name, String(value), JSType::jsonInteger);
    return *this;
}

Json &Json::add(const String &name, const int32_t &value) {
    add(name, String(value), JSType::jsonInteger);
    return *this;
}

Json &Json::add(const String &name, const uint8_t &value) {
    add(name, String(value), JSType::jsonInteger);
    return *this;
}

Json &Json::add(const String &name, const uint16_t &value) {
    add(name, String(value), JSType::jsonInteger);
    return *this;
}

Json &Json::add(const String &name, const uint32_t &value) {
    add(name, String(value), JSType::jsonInteger);
    return *this;
}

Json &Json::add(const String &name, const float &value, const uint8_t &precision) {
    add(name, String(value, precision), JSType::jsonFloat);
    return *this;
}

Json &Json::add(const String &name, const double &value, const uint8_t &precision) {
    add(name, String(value, precision), JSType::jsonFloat);
    return *this;
}

Json &Json::add(const String &name, const bool &value) {
    add(name, value ? "true" : "false", JSType::jsonBoolean);
    return *this;
}

Json &Json::add(const String &name, const void *value) {
    add(name, "null", JSType::jsonNull);
    return *this;
}

JSType::Value Json::getValue(const String &name) const {
    if (contains(name)) {
        return {doc.at(name).getValue()};
    }
    return {String()};
}

JSType::Value Json::getValue(const uint16_t &index) const {
    if (indexList.count(index) > 0) {
        return {getElement(index).getValue()};
    }
    return {String()};
}

JSType::Value Json::operator[](const String &name) const {
    if (contains(name)) {
        return {doc.at(name).getValue()};
    }
    return {String()};
}

JSType::Value Json::operator[](const uint16_t &index) const {
    if (indexList.count(index) > 0) {
        return {getElement(index).getValue()};
    }
    return {String()};
}

JSType::Element Json::getElement(const String &name) const {
    if (contains(name)) {
        return doc.at(name);
    } else {
        return JSType::Element();
    }
}

JSType::Element Json::getElement(const uint16_t &index) const {
    if (indexList.count(index) > 0) {
        return doc.at(indexList.at(index));
    } else {
        return JSType::Element();
    }
}

JSType::Type Json::getType(const String &name) const {
    if (contains(name)) {
        return doc.at(name).getType();
    }
    return JSType::undefined;
}

JSType::Type Json::getType(const uint16_t &index) const {
    if (indexList.count(index) > 0) {
        return doc.at(indexList.at(index)).getType();
    }
    return JSType::undefined;
}

String Json::getTypeString(const String &name) const {
    if (contains(name)) {
        return JSUtil.typeToString(doc.at(name).getType());
    }
    return JSUtil.typeToString(JSType::undefined);
}

String Json::getTypeString(const uint16_t &index) const {
    if (indexList.count(index) > 0) {
        return JSUtil.typeToString(doc.at(indexList.at(index)).getType());
    }
    return JSUtil.typeToString(JSType::undefined);
}

String Json::getKey(const uint16_t &index) const {
    if (indexList.count(index) > 0) {
        return indexList.at(index);
    }
    return String();
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
        str += (getElement(i).getType() == JSType::jsonString ? "\"" : "") + getElement(i).getValue() + (getElement(i).getType() == JSType::jsonString ? "\"" : "");
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