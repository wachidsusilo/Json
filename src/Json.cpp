#include "Json.h"

static String _emptyValue = "";
static JSType::Element _emptyElement = JSType::Element(_emptyValue, JSType::undefined);

/**--- Json Type ---**/
JSType::Element::Element()
    : value(&_emptyValue), type(undefined) {}

JSType::Element::~Element() {
    if (value && value != &_emptyValue) delete value;
}

JSType::Element::Element(const String &value, const Type &type)
    : value(new String(value)), type(type) {}

JSType::Element::Element(const Element &e)
    : value(new String(e.getValue())), type(e.getType()) {}

JSType::Element::Element(const int8_t &value)
    : value(new String(value)), type(jsonInteger) {}

JSType::Element::Element(const int16_t &value)
    : value(new String(value)), type(jsonInteger) {}

JSType::Element::Element(const int32_t &value)
    : value(new String(value)), type(jsonInteger) {}

JSType::Element::Element(const uint8_t &value)
    : value(new String(value)), type(jsonInteger) {}

JSType::Element::Element(const uint16_t &value)
    : value(new String(value)), type(jsonInteger) {}

JSType::Element::Element(const uint32_t &value)
    : value(new String(value)), type(jsonInteger) {}

JSType::Element::Element(const float &value)
    : value(new String(JSUtil.toString(value))), type(jsonFloat) {}

JSType::Element::Element(const double &value)
    : value(new String(JSUtil.toString(value))), type(jsonFloat) {}

JSType::Element::Element(const String &value)
    : value(new String(value)), type(jsonString) {}

JSType::Element::Element(const char *value)
    : value(new String(value)), type(jsonString) {}

JSType::Element::Element(const void *value)
    : value(new String("null")), type(jsonNull) {}

JSType::Element::Element(const bool &value)
    : value(new String(value ? "true" : "false")), type(jsonBoolean) {}

JSType::Element::Element(const Json &value)
    : value(new String(value.toString())), type(jsonObject) {}

JSType::Element::Element(const JsonArray &value)
    : value(new String(value.toString())), type(jsonArray) {}

JSType::Element::operator int8_t() {
    return round((*value).toDouble());
}

JSType::Element::operator int16_t() {
    return round((*value).toDouble());
}

JSType::Element::operator int32_t() {
    return round((*value).toDouble());
}

JSType::Element::operator uint8_t() {
    return round((*value).toDouble());
}

JSType::Element::operator uint16_t() {
    return round((*value).toDouble());
}

JSType::Element::operator uint32_t() {
    return round((*value).toDouble());
}

JSType::Element::operator float() {
    return (*value).toFloat();
}

JSType::Element::operator double() {
    return (*value).toDouble();
}

JSType::Element::operator String() {
    return *value;
}

JSType::Element::operator bool() {
    return (*value).equals("true");
}

JSType::Element::operator void *() {
    return NULL;
}

JSType::Element::operator Json() {
    return Json((*value).c_str());
}

JSType::Element::operator JsonArray() {
    return JsonArray((*value).c_str());
}

JSType::Element &JSType::Element::operator=(const Element &e) {
    if(this == &e) return *this;
    *value = e.getValue();
    type = e.getType();
    return *this;
}

JSType::Type JSType::Element::getType() const {
    return type;
}

String &JSType::Element::getValue() {
    return *value;
}

const String &JSType::Element::getValue() const {
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

JsonArray::JsonArray(std::vector<int> &arr) : v(std::vector<JSType::Element>()) {
    for (int i : arr) push(i);
}

JsonArray &JsonArray::push(const String &value, const JSType::Type &type) {
    v.emplace_back(value, type);
    return *this;
}

JsonArray &JsonArray::push(const JSType::Element &value) {
    push(value.getValue(), value.getType());
    return *this;
}

JSType::Element &JsonArray::operator[](const uint16_t &index) {
    if (index < v.size()) {
        return v.at(index);
    }
    return _emptyElement;
}

JSType::Element &JsonArray::getElement(const uint16_t &index) {
    if (index < v.size()) {
        return v[index];
    }
    return _emptyElement;
}

const JSType::Element &JsonArray::operator[](const uint16_t &index) const {
    if (index < v.size()) {
        return v.at(index);
    }
    return _emptyElement;
}

const JSType::Element &JsonArray::getElement(const uint16_t &index) const {
    if (index < v.size()) {
        return v[index];
    }
    return _emptyElement;
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

Json &Json::add(const String &name, const JSType::Element &value) {
    add(name, value.getValue(), value.getType());
    return *this;
}

JSType::Element &Json::operator[](const String &name) {
    if (contains(name)) {
        return doc.at(name);
    }
    return _emptyElement;
}

JSType::Element &Json::operator[](const uint16_t &index) {
    if (indexList.count(index) > 0) {
        return getElement(index);
    }
    return _emptyElement;
}

JSType::Element &Json::getElement(const String &name) {
    if (contains(name)) {
        return doc.at(name);
    }
    return _emptyElement;
}

JSType::Element &Json::getElement(const uint16_t &index) {
    if (indexList.count(index) > 0) {
        return doc.at(indexList.at(index));
    }
    return _emptyElement;
}

const JSType::Element &Json::operator[](const String &name) const {
    if (contains(name)) {
        return doc.at(name);
    }
    return _emptyElement;
}

const JSType::Element &Json::operator[](const uint16_t &index) const {
    if (indexList.count(index) > 0) {
        return doc.at(indexList.at(index));
    }
    return _emptyElement;
}

const JSType::Element &Json::getElement(const String &name) const {
    if (contains(name)) {
        return doc.at(name);
    }
    return _emptyElement;
}

const JSType::Element &Json::getElement(const uint16_t &index) const {
    if (indexList.count(index) > 0) {
        return doc.at(indexList.at(index));
    }
    return _emptyElement;
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