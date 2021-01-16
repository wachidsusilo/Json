#include "Json.h"

/**--- Json Type ---**/
JSType::Type JSType::Element::getType() const {
  return type;
}

String JSType::Element::getValue() const {
  return value;
}

void JSType::Element::setType(const Type &type) {
  this->type = type;
}

void JSType::Element::setValue(const String &value) {
  this->value = value;
}

bool JSType::Element::operator ==(const Element &e) {
  return value.equals(e.getValue()) && (type == e.getType());
}

/**--- Json Utility ---**/
Json JsonUtil::parse(const String &str) {
  if (!str.startsWith("{\"") || !str.endsWith("}")) {
    return Json();
  }

  String s = str.substring(1, str.length() - 1);
  s.trim();

  Json doc;
  bool first = true;

  while (s.length() > 0) {
    if (s.startsWith(",")) {
      s.remove(0, 1);
      s.trim();
    } else {
      if (!first) return Json();
      first = false;
    }

    if (!s.startsWith("\"")) return Json();
    String key = "";
    bool found = false;
    for (int i = 1; i < s.length(); i++) {
      if (String(s[i]).equals("\"") && !String(s[i - 1]).equals("\\")) {
        key = s.substring(1, i);
        s.remove(0, i + 1);
        s.trim();
        found = true;
        break;
      }
    }
    if (!found) return Json();
    if (!s.startsWith(":")) return Json();
    s.remove(0, 1);
    s.trim();

    if (s.startsWith("\"")) {
      bool found = false;
      for (int i = 1; i < s.length(); i++) {
        if (String(s[i]).equals("\"") && !String(s[i - 1]).equals("\\")) {
          doc.add(key, JSType::Element(s.substring(1, i), JSType::jsonString));
          s.remove(0, i + 1);
          s.trim();
          found = true;
          break;
        }
      }
      if (!found) return Json();
    } else if (s.startsWith("{")) {
      int x = 1;
      for (int i = 1; i < s.length(); i++) {
        if (String(s[i]).equals("{")) {
          x++;
        } else if (String(s[i]).equals("}")) {
          x--;
        }
        if (x == 0) {
          doc.add(key, JSType::Element(s.substring(0, i + 1), JSType::jsonObject));
          s.remove(0, i + 1);
          s.trim();
          break;
        }
      }
      if (x != 0) return Json();
    } else if (s.startsWith("[")) {
      int x = 1;
      for (int i = 1; i < s.length(); i++) {
        if (String(s[i]).equals("[")) {
          x++;
        } else if (String(s[i]).equals("]")) {
          x--;
        }
        if (x == 0) {
          doc.add(key, JSType::Element(s.substring(0, i + 1), JSType::jsonArray));
          s.remove(0, i + 1);
          s.trim();
          break;
        }
      }
      if (x != 0) return Json();
    } else {
      String buf = s.substring(0, s.indexOf(","));
      buf.trim();
      s.remove(0, s.indexOf(","));
      s.trim();
      if (buf.equals("true") || buf.equals("false") || buf.equals("null")) {
        doc.add(key, JSType::Element(buf, buf.equals("null") ? JSType::jsonNull : JSType::jsonBoolean));
      } else {
        if (buf.length() > 1) {
          if ((buf.startsWith("0") && !String(buf[1]).equals(".")) || (!isdigit(buf[0]) && !String(buf[0]).equals("-"))) {
            return Json();
          }
        } else {
          if (buf.isEmpty() || !isdigit(buf[0])) {
            return Json();
          }
        }
        int d = 0, e = 0;
        for (int i = 0; i < buf.length(); i++) {
          if (String(buf[i]).equals(".")) {
            if (++d > 1 || e > 0) {
              return Json();
            }
          } else if (String(buf[i]).equals("e") || String(buf[i]).equals("E")) {
            if (++e > 1) {
              return Json();
            }
          } else if (String(buf[i]).equals("-") || String(buf[i]).equals("+")) {
            if (i != 0 && !(String(buf[i - 1]).equals("e") || String(buf[i - 1]).equals("E"))) {
              return Json();
            }
          } else if (!isdigit(buf[i])) {
            return Json();
          }
        }
        doc.add(key, JSType::Element(buf, isFloat(buf) ? JSType::jsonFloat : JSType::jsonInteger));
      }
    }
  }

  return doc;
}

JsonArray JsonUtil::parseArray(const String &str) {
  if (!str.startsWith("[") || !str.endsWith("]")) {
    return JsonArray();
  }

  String s = str.substring(1, str.length() - 1);
  s.trim();
  JsonArray arr;
  bool first = true;

  while (s.length() > 0) {
    if (s.startsWith(",")) {
      s.remove(0, 1);
      s.trim();
    } else {
      if (!first) return JsonArray();
      first = false;
    }

    if (s.startsWith("\"")) {
      bool found = false;
      for (int i = 1; i < s.length(); i++) {
        if (String(s[i]).equals("\"") && !String(s[i - 1]).equals("\\")) {
          arr.push(s.substring(1, i), JSType::jsonString);
          s.remove(0, i + 1);
          s.trim();
          found = true;
          break;
        }
      }
      if (!found) return JsonArray();
    } else if (s.startsWith("{")) {
      int x = 1;
      for (int i = 1; i < s.length(); i++) {
        if (String(s[i]).equals("{")) {
          x++;
        } else if (String(s[i]).equals("}")) {
          x--;
        }
        if (x == 0) {
          arr.push(s.substring(0, i + 1), JSType::jsonObject);
          s.remove(0, i + 1);
          s.trim();
          break;
        }
      }
      if (x != 0) return JsonArray();
    } else if (s.startsWith("[")) {
      int x = 1;
      for (int i = 1; i < s.length(); i++) {
        if (String(s[i]).equals("[")) {
          x++;
        } else if (String(s[i]).equals("]")) {
          x--;
        }
        if (x == 0) {
          arr.push(s.substring(0, i + 1), JSType::jsonArray);
          s.remove(0, i + 1);
          s.trim();
          break;
        }
      }
      if (x != 0) return JsonArray();
    } else {
      String buf = s.substring(0, s.indexOf(","));
      buf.trim();
      s.remove(0, s.indexOf(","));
      s.trim();
      if (buf.equals("true") || buf.equals("false") || buf.equals("null")) {
        arr.push(buf, buf.equals("null") ? JSType::jsonNull : JSType::jsonBoolean);
      } else {
        if (buf.length() > 1) {
          if ((buf.startsWith("0") && !String(buf[1]).equals(".")) || (!isdigit(buf[0]) && !String(buf[0]).equals("-"))) {
            return JsonArray();
          }
        } else {
          if (buf.isEmpty() || !isdigit(buf[0])) {
            return JsonArray();
          }
        }
        int d = 0, e = 0;
        for (int i = 0; i < buf.length(); i++) {
          if (String(buf[i]).equals(".")) {
            if (++d > 1 || e > 0) {
              return JsonArray();
            }
          } else if (String(buf[i]).equals("e") || String(buf[i]).equals("E")) {
            if (++e > 1) {
              return JsonArray();
            }
          } else if (String(buf[i]).equals("-") || String(buf[i]).equals("+")) {
            if (i != 0 && !(String(buf[i - 1]).equals("e") || String(buf[i - 1]).equals("E"))) {
              return JsonArray();
            }
          } else if (!isdigit(buf[i])) {
            return JsonArray();
          }
        }
        arr.push(buf, isFloat(buf) ? JSType::jsonFloat : JSType::jsonInteger);
      }
    }
  }

  return arr;
}

String JsonUtil::prettify(const String &str, const uint8_t &space) {
  if (!str.startsWith("{\"") || !str.endsWith("}")) {
    return "Invalid JSON Object";
  }

  String indent = "";
  for (int i = 0; i <= space; i++) {
    indent += "  ";
  }

  String inBot = "";
  for (int i = 0; i < space; i++) {
    inBot += "  ";
  }

  String res = "{\n";
  String s = str.substring(1, str.length() - 1);
  s.trim();
  bool first = true, isFirst = true;

  while (s.length() > 0) {
    if (s.startsWith(",")) {
      s.remove(0, 1);
      s.trim();
    } else {
      if (!first) return "Invalid JSON Object";
      first = false;
    }

    if (isFirst) {
      isFirst = false;
    } else {
      res += ",\n";
    }

    if (!s.startsWith("\"")) return "Invalid JSON Object";
    bool found = false;
    for (int i = 1; i < s.length(); i++) {
      if (String(s[i]).equals("\"") && !String(s[i - 1]).equals("\\")) {
        res += indent + s.substring(0, i + 1) + ":";
        s.remove(0, i + 1);
        s.trim();
        found = true;
        break;
      }
    }
    if (!found) return "Invalid JSON Object";
    if (!s.startsWith(":")) return "Invalid JSON Object";
    s.remove(0, 1);
    s.trim();

    if (s.startsWith("\"")) {
      bool found = false;
      for (int i = 1; i < s.length(); i++) {
        if (String(s[i]).equals("\"") && !String(s[i - 1]).equals("\\")) {
          res += s.substring(0, i + 1);
          s.remove(0, i + 1);
          s.trim();
          found = true;
          break;
        }
      }
      if (!found) return "Invalid JSON Object";
    } else if (s.startsWith("{")) {
      int x = 1;
      for (int i = 1; i < s.length(); i++) {
        if (String(s[i]).equals("{")) {
          x++;
        } else if (String(s[i]).equals("}")) {
          x--;
        }
        if (x == 0) {
          res += prettify(s.substring(0, i + 1), space + 1);
          s.remove(0, i + 1);
          s.trim();
          break;
        }
      }
      if (x != 0) return "Invalid JSON Object";
    } else if (s.startsWith("[")) {
      int x = 1;
      for (int i = 1; i < s.length(); i++) {
        if (String(s[i]).equals("[")) {
          x++;
        } else if (String(s[i]).equals("]")) {
          x--;
        }
        if (x == 0) {
          res += prettifyArray(s.substring(0, i + 1), space + 1);
          s.remove(0, i + 1);
          s.trim();
          break;
        }
      }
      if (x != 0) return "Invalid JSON Array";
    } else {
      String buf = s.substring(0, s.indexOf(","));
      buf.trim();
      s.remove(0, s.indexOf(","));
      s.trim();
      if (buf.equals("true") || buf.equals("false") || buf.equals("null")) {
        res += buf;
      } else {
        if (buf.length() > 1) {
          if ((buf.startsWith("0") && !String(buf[1]).equals(".")) || (!isdigit(buf[0]) && !String(buf[0]).equals("-"))) {
            return "Invalid Number: " + buf;
          }
        } else {
          if (buf.isEmpty() || !isdigit(buf[0])) {
            return "Invalid Number: " + buf;
          }
        }
        int d = 0, e = 0;
        for (int i = 0; i < buf.length(); i++) {
          if (String(buf[i]).equals(".")) {
            if (++d > 1 || e > 0) {
              return "Invalid Number: " + buf;
            }
          } else if (String(buf[i]).equals("e") || String(buf[i]).equals("E")) {
            if (++e > 1) {
              return "Invalid Number: " + buf;
            }
          } else if (String(buf[i]).equals("-") || String(buf[i]).equals("+")) {
            if (i != 0 && !(String(buf[i - 1]).equals("e") || String(buf[i - 1]).equals("E"))) {
              return "Invalid Number: " + buf;
            }
          } else if (!isdigit(buf[i])) {
            return "Invalid Number: " + buf;
          }
        }
        res += buf;
      }
    }
  }

  res += "\n" + inBot + "}";
  return res;
}

String JsonUtil::prettifyArray(const String &str, const uint8_t &space) {
  if (!str.startsWith("[") || !str.endsWith("]")) {
    return "Invalid JSON Array";
  }

  String indent = "";
  for (int i = 0; i <= space; i++) {
    indent += "  ";
  }

  String inBot = "";
  for (int i = 0; i < space; i++) {
    inBot += "  ";
  }

  String res = "[\n";
  String s = str.substring(1, str.length() - 1);
  s.trim();
  bool first = true, isFirst = true;

  while (s.length() > 0) {
    if (s.startsWith(",")) {
      s.remove(0, 1);
      s.trim();
    } else {
      if (!first) "Invalid JSON Array";
      first = false;
    }

    if (isFirst) {
      isFirst = false;
    } else {
      res += ",\n";
    }

    if (s.startsWith("\"")) {
      bool found = false;
      for (int i = 1; i < s.length(); i++) {
        if (String(s[i]).equals("\"") && !String(s[i - 1]).equals("\\")) {
          res += indent + s.substring(0, i + 1);
          s.remove(0, i + 1);
          s.trim();
          found = true;
          break;
        }
      }
      if (!found) return "Invalid JSON Array";
    } else if (s.startsWith("{")) {
      int x = 1;
      for (int i = 1; i < s.length(); i++) {
        if (String(s[i]).equals("{")) {
          x++;
        } else if (String(s[i]).equals("}")) {
          x--;
        }
        if (x == 0) {
          res += indent + prettify(s.substring(0, i + 1), space + 1);
          s.remove(0, i + 1);
          s.trim();
          break;
        }
      }
      if (x != 0) return "Invalid JSON Object";
    } else if (s.startsWith("[")) {
      int x = 1;
      for (int i = 1; i < s.length(); i++) {
        if (String(s[i]).equals("[")) {
          x++;
        } else if (String(s[i]).equals("]")) {
          x--;
        }
        if (x == 0) {
          res += indent + prettifyArray(s.substring(0, i + 1), space + 1);
          s.remove(0, i + 1);
          s.trim();
          break;
        }
      }
      if (x != 0) return "Invalid JSON Array";
    } else {
      String buf = s.substring(0, s.indexOf(","));
      buf.trim();
      s.remove(0, s.indexOf(","));
      s.trim();
      if (buf.equals("true") || buf.equals("false") || buf.equals("null")) {
        res += indent + buf;
      } else {
        if (buf.length() > 1) {
          if ((buf.startsWith("0") && !String(buf[1]).equals(".")) || (!isdigit(buf[0]) && !String(buf[0]).equals("-"))) {
            return "Invalid Number: " + buf;
          }
        } else {
          if (buf.isEmpty() || !isdigit(buf[0])) {
            return "Invalid Number: " + buf;
          }
        }
        int d = 0, e = 0;
        for (int i = 0; i < buf.length(); i++) {
          if (String(buf[i]).equals(".")) {
            if (++d > 1 || e > 0) {
              return "Invalid Number: " + buf;
            }
          } else if (String(buf[i]).equals("e") || String(buf[i]).equals("E")) {
            if (++e > 1) {
              return "Invalid Number: " + buf;
            }
          } else if (String(buf[i]).equals("-") || String(buf[i]).equals("+")) {
            if (i != 0 && !(String(buf[i - 1]).equals("e") || String(buf[i - 1]).equals("E"))) {
              return "Invalid Number: " + buf;
            }
          } else if (!isdigit(buf[i])) {
            return "Invalid Number: " + buf;
          }
        }
        res += indent + buf;
      }
    }
  }

  res += "\n" + inBot + "]";
  return res;
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

/*--- Json Array Value ---*/
JSType::Value::operator int() {
  return round(value.toDouble());
}

JSType::Value::operator long() {
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

JSType::Value::operator void*() {
  return NULL;
}

JSType::Value::operator Json() {
  return Json(value);
}

JSType::Value::operator JsonArray() {
  return JSUtil.parseArray(value);
}

/*--- Json Array ---*/
JsonArray::JsonArray(): v(std::vector<JSType::Element>()) {}
JsonArray::JsonArray(const String &str): v(std::vector<JSType::Element>()) {
  *this = JSUtil.parseArray(str);
}

JsonArray JsonArray::push(const JSType::Element &e) {
  v.push_back(e);
  return *this;
}

JsonArray JsonArray::push(const String &value, const JSType::Type &type) {
  v.push_back(JSType::Element(value, type));
  return *this;
}

JsonArray JsonArray::push(Json value) {
  push(value.toString(), JSType::jsonObject);
  return *this;
}

JsonArray JsonArray::push(JsonArray value) {
  push(value.toString(), JSType::jsonArray);
  return *this;
}

JsonArray JsonArray::push(const char* value) {
  push(String(value));
  return *this;
}

JsonArray JsonArray::push(const String &value) {
  push(value, JSType::jsonString);
  return *this;
}

JsonArray JsonArray::push(const int &value) {
  push(String(value), JSType::jsonInteger);
  return *this;
}

JsonArray JsonArray::push(const long &value) {
  push(String(value), JSType::jsonInteger);
  return *this;
}

JsonArray JsonArray::push(const bool &value) {
  push(value ? "true" : "false", JSType::jsonBoolean);
  return *this;
}

JsonArray JsonArray::push(const void* value) {
  push("null", JSType::jsonNull);
  return *this;
}

JsonArray JsonArray::push(const float &value, const uint8_t &precision) {
  push(String(value, precision), JSType::jsonFloat);
  return *this;
}

JsonArray JsonArray::push(const double &value, const uint8_t &precision) {
  push(String(value, precision), JSType::jsonFloat);
  return *this;
}

JSType::Value JsonArray::getValue(const uint16_t &index) {
  if (index < v.size()) {
    return {v[index].getValue()};
  }
  return {String()};
}

JSType::Value JsonArray::operator [](const uint16_t &index) {
  if (index < v.size()) {
    return {v[index].getValue()};
  }
  return {String()};
}

JSType::Element JsonArray::getElement(const uint16_t &index) {
  if (index < v.size()) {
    return v[index];
  }
  return JSType::Element();
}

JSType::Type JsonArray::getType(const uint16_t &index) {
  if (index < v.size()) {
    return v[index].getType();
  }
  return JSType::undefined;
}

String JsonArray::getTypeString(const uint16_t &index) {
  if (index < v.size()) {
    return JSUtil.typeToString(v[index].getType());
  }
  return JSUtil.typeToString(JSType::undefined);
}

String JsonArray::toString() {
  String str("[");
  bool isFirst = true;
  for (int i = 0; i < v.size(); i++) {
    if (isFirst) {
      isFirst = false;
    } else {
      str += ",";
    }
    str += (v[i].getType() == JSType::jsonString ?  "\"" : "") + v[i].getValue() + (v[i].getType() == JSType::jsonString ?  "\"" : "");
  }
  str += "]";
  return str;
}

size_t JsonArray::size() {
  return v.size();
}

void JsonArray::erase(const uint16_t &index) {
  if (index < v.size()) {
    v.erase(v.begin() + index);
  }
}

bool JsonArray::contains(const JSType::Element &e){
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
Json::Json(): doc(std::map<String, JSType::Element>()), indexList(std::map<uint16_t, String>()), counter(0) {}
Json::Json(const String &str): doc(std::map<String, JSType::Element>()), indexList(std::map<uint16_t, String>()), counter(0) {
  *this = JSUtil.parse(str);
}

Json Json::add(const String &key, const JSType::Element &e) {
  doc.insert(std::pair<String, JSType::Element>(key, e));
  indexList.insert(std::pair<uint16_t, String>(counter++, key));
  return *this;
}

Json Json::add(const String &key, const String &value, const JSType::Type &type) {
  doc.insert(std::pair<String, JSType::Element>(key, JSType::Element(value, type)));
  indexList.insert(std::pair<uint16_t, String>(counter++, key));
  return *this;
}

Json Json::add(const String &name, Json value) {
  add(name, value.toString(), JSType::jsonObject);
  return *this;
}

Json Json::add(const String &name, JsonArray value) {
  add(name, value.toString(), JSType::jsonArray);
  return *this;
}

Json Json::add(const String &name, const char* value) {
  add(name, String(value));
  return *this;
}

Json Json::add(const String &name, const String &value) {
  add(name, value, JSType::jsonString);
  return *this;
}

Json Json::add(const String &name, const int &value) {
  add(name, String(value), JSType::jsonInteger);
  return *this;
}

Json Json::add(const String &name, const long &value) {
  add(name, String(value), JSType::jsonInteger);
  return *this;
}

Json Json::add(const String &name, const float &value, const uint8_t &precision) {
  add(name, String(value, precision), JSType::jsonFloat);
  return *this;
}

Json Json::add(const String &name, const double &value, const uint8_t &precision) {
  add(name, String(value, precision), JSType::jsonFloat);
  return *this;
}

Json Json::add(const String &name, const bool &value) {
  add(name, value ? "true" : "false", JSType::jsonBoolean);
  return *this;
}

Json Json::add(const String &name, const void* value) {
  add(name, "null", JSType::jsonNull);
  return *this;
}

JSType::Value Json::getValue(const String &name) {
  if (contains(name)) {
    return {doc[name].getValue()};
  }
  return {String()};
}

JSType::Value Json::getValue(const uint16_t &index) {
  if (indexList.count(index) > 0) {
    return {getElement(index).getValue()};
  }
  return {String()};
}

JSType::Value Json::operator [](const String &name) {
  if (contains(name)) {
    return {doc[name].getValue()};
  }
  return {String()};
}

JSType::Value Json::operator [](const uint16_t &index) {
  if (indexList.count(index) > 0) {
    return {getElement(index).getValue()};
  }
  return {String()};
}

JSType::Element Json::getElement(const String &name) {
  if (contains(name)) {
    return doc[name];
  } else {
    return JSType::Element();
  }
}

JSType::Element Json::getElement(const uint16_t &index) {
  if (indexList.count(index) > 0) {
    return doc[indexList[index]];
  } else {
    return JSType::Element();
  }
}

JSType::Type Json::getType(const String &name) {
  if (contains(name)) {
    return doc[name].getType();
  }
  return JSType::undefined;
}

JSType::Type Json::getType(const uint16_t &index) {
  if (indexList.count(index) > 0) {
    return doc[indexList[index]].getType();
  }
  return JSType::undefined;
}

String Json::getTypeString(const String &name) {
  if (contains(name)) {
    return JSUtil.typeToString(doc[name].getType());
  }
  return JSUtil.typeToString(JSType::undefined);
}

String Json::getTypeString(const uint16_t &index) {
  if (indexList.count(index) > 0) {
    return JSUtil.typeToString(doc[indexList[index]].getType());
  }
  return JSUtil.typeToString(JSType::undefined);
}

String Json::getKey(const uint16_t &index) {
  if (indexList.count(index) > 0) {
    return indexList[index];
  }
  return String();
}

int16_t Json::getIndex(const String &name) {
  for (int i = 0; i < indexList.size(); i++) {
    if (indexList[i].equals(name)) {
      return i;
    }
  }
  return -1;
}

String Json::toString() {
  String str("{");
  bool isFirst = true;
  for (int i = 0; i < doc.size(); i++) {
    if (isFirst) {
      isFirst = false;
    } else {
      str += ",";
    }
    str += "\"" + getKey(i) + "\":";
    str += (getElement(i).getType() == JSType::jsonString ?  "\"" : "") + getElement(i).getValue() + (getElement(i).getType() == JSType::jsonString ?  "\"" : "");
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
      buf.insert(std::pair<uint16_t, String>(counter++, indexList[i]));
    }
  }
  indexList = buf;
}
