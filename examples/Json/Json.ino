#include "data.h"
#include "Json.h"

void printValue(String name, String value, String type) {
    Serial.println("(" + type + ") " + name + ": " + value);
}

void setup() {
    Serial.begin(115200);
    Json::attachDebugger(Serial);
    delay(1000);

    Serial.println("******** Json Example *********\n");

    /**Initialize Json Object from a String*/
    Json json(" {\"name\":\"projects/project-name-1ab2c/databases/(default)/documents\", \"fields\":{\"08:24:21\":{\"integerValue\":\"1234\"},\"data\":{\"arrayValue\":{\"values\":[{\"timestampValue\":\"2021-01-13T02:20:20Z\"},{\"geoPointValue\":{\"latitude\":17,\"longitude\":32}},{\"booleanValue\":true},{\"stringValue\":\"{\\\"myname\\\":\\\"isJohnson\\\", \\\"data\\\":{\\\"mydata\\\":\\\"something\\\",\\\"mynumber\\\":27}}\"},{\"stringValue\":\"200,210,220,230,240\"},{\"stringValue\":\"[{\\\"data\\\":\\\"data1\\\"},{\\\"bool\\\":false},{\\\"number\\\":153}]\"}]}}},\"array\":[234,true,null,\"string\",{\"object\":\"value\"},\"[{\\\"key1\\\":\\\"value1\\\"},\\\"data\\\",426,null,false]\",4.62,2e5,0.23,5.1E5,6.01e2,-0.6,-27,2e-7,5e+5],\"createTime\":\"2021-01-01T13:36:21.457729Z\",\"updateTime\":\"2021-01-13T07:42:31.519410Z\",\"number1\":2.46,\"number2\":4e5,\"number3\":0.76,\"number4\":-24,\"number5\":-0.56,\"number6\":-2.5e-5,\"number7\":-4.6e+3} ");

    /*Get size*/
    int jsonSize = json.size();
    Serial.println("Size: " + String(jsonSize));

    /*Check if the object contains specified name*/
    bool isContain = json.contains("frog");
    Serial.println(String("Is contain 'frog'? ") + (isContain ? "value with name 'frog' does exist" : "value with name 'frog' does not exist"));

    /*erase value with specified name*/
    json.erase("number4");
    Serial.println("Value with name 'number4' has been deleted");

    /*Get type of specified name (JSType::Type)*/
    /**
     (JSType::Type) is an enumeration of Json Type
     including: jsonString, jsonInteger, jsonFloat,
     jsonBoolean, jsonNull, jsonArray, and jsonObject
  */
    JSType::Type t = json.getType("name");

    /*Get type of specified name (String)*/
    /**
     This is Stringified version of (JSType::Type)
     including: string, integer, float, boolean,
     null, array, and object
  */
    String type = json.getTypeString("name");
    Serial.println("Type of 'name' is (" + type + ")");
    type = json.getTypeString("fields");
    Serial.println("Type of 'fields' is (" + type + ")");

    /*Get type and name(key) of Json Object*/
    Serial.println("\nThis Json Object contains:");
    for (int i = 0; i < json.size(); i++) {
        Serial.println("(" + json.getTypeString(i) + ") " + json.getKey(i));
    }

    /*Stringify Json Object without indentation (compact)*/
    Serial.println("\n*** Compact String ***");
    Serial.println(json.toString());

    /*Stringify Json Object with indentation*/
    Serial.println("\n*** Pretty String ***");
    JSUtil.prettyPrint(Serial, json);

    Serial.println("\n++++ Generating Json Object +++++\n");

    /*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
    /**Build your own Json Object*/
    Json js;

    /*Add stringValue*/
    js.add("name", "michael");

    /*Add integerValue*/
    js.add("year", 2021);

    /*Add booleanValue*/
    js.add("isNotified", true);

    /*Add floatValue*/
    js.add("pi", 3.14159265359);

    /**Add nullValue*/
    js.add("pointer", (void*)NULL);

    /*Create Array*/
    JsonArray jsonArray;
    jsonArray.push("jessica");    //insert String
    jsonArray.push(65432);        //insert int
    jsonArray.push(false);        //insert boolean
    jsonArray.push(5.234);        //insert float
    jsonArray.push((void*)NULL);  //insert null
    jsonArray.push(JsonArray()    //insert Array
                       .push("height")
                       .push(165));
    jsonArray.push(Json()  //insert Json Object
                       .add("name", "Hannah")
                       .add("body-weight", 65));

    /*Add Array*/
    js.add("my-array", jsonArray);

    /*Add Object*/
    js.add("color", Json()
                        .add("red", 65)
                        .add("green", 212)
                        .add("blue", 162)
                        .add("alpha", 255));

    Serial.println("\n++++ Get value from a Json Object by using template (generic) +++++\n");
    Serial.println(js["name"].as<String>());
    Serial.println(js["year"].as<int>());
    Serial.println(js["isNotified"].as<bool>());
    Serial.println(js["pi"].as<float>());
    Serial.println(js["my-array"].as<JsonArray>().toString());
    Serial.println(js["color"].as<Json>().toString());

    Serial.println("\n++++ Get value from a Json Object by assigning it to the existing variable +++++\n");

    /*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
    /**
     +++ Get value from a Json Object +++
     getValue() and operator[] can be used
     interchangeably. For example, to get
     value with name 'frog', you could do:
     # json.getValue("frog")
     or just,
     # json["frog"];
  */
    /*Get stringValue*/
    String name = js.getElement("name");
    printValue("name", name, js.getTypeString("name"));

    /*Get integerValue (32 bit)*/
    int year = js["year"];
    printValue("year", String(year), js.getTypeString("year"));

    /*Get booleanValue*/
    bool isNotified = js.getElement("isNotified");
    printValue("isNotified", isNotified ? "true" : "false", js.getTypeString("isNotified"));

    /*Set the value of an element*/
    js["isNotified"] = false;
    isNotified = js.getElement("isNotified");
    printValue("isNotified", isNotified ? "true" : "false", js.getTypeString("isNotified"));

    /*Get floatValue*/
    float piFloat = js["pi"];
    printValue("pi", String(piFloat, 15), js.getTypeString("pi"));

    /*Get doubleValue*/
    double piDouble = js.getElement("pi");
    printValue("pi", String(piDouble, 15), js.getTypeString("pi"));

    /*Get nullValue*/
    void* ptr = js["pointer"];
    printValue("pointer", String((int)ptr), js.getTypeString("pointer"));

    /*Get Array*/
    JsonArray arr = js.getElement("my-array");
    printValue("my-array", "", js.getTypeString("my-array"));
    JSUtil.prettyPrint(Serial, arr);

    /*Get Json Object*/
    Json obj = js["color"];
    printValue("color", "", js.getTypeString("color"));
    JSUtil.prettyPrint(Serial, obj);

    Serial.println("\n++++ Get value from a Json Array +++++\n");

    /*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
    /**
     +++ Get value from a Json Array +++
     getValue() and operator[] can be used
     interchangeably. For example, to get
     value with index '1', you could do:
     # jsonArray.getValue(1)
     or just,
     # jsonArray[1];
  */
    /*Get stringValue*/
    String m_string = jsonArray[0];
    printValue("0", m_string, jsonArray.getTypeString(0));

    /*Get integerValue (32 bit)*/
    int m_int = jsonArray.getElement(1);
    printValue("1", String(m_int), jsonArray.getTypeString(1));

    /*Get booleanValue*/
    bool m_bool = jsonArray[2];
    printValue("2", m_bool ? "true" : "false", jsonArray.getTypeString(2));

    /*Get floatValue*/
    float m_float = jsonArray.getElement(3);
    printValue("3", String(m_float, 15), jsonArray.getTypeString(3));

    /*Get doubleValue*/
    double m_double = jsonArray[3];
    printValue("3", String(m_double, 15), jsonArray.getTypeString(3));

    /*Get nullValue*/
    void* m_void = jsonArray.getElement(4);
    printValue("4", String((int)m_void), jsonArray.getTypeString(4));

    /*Get Array*/
    JsonArray m_arr = jsonArray[5];
    printValue("5", "", jsonArray.getTypeString(5));
    JSUtil.prettyPrint(Serial, m_arr);

    /*Get Json Object*/
    Json m_json = jsonArray.getElement(6);
    printValue("6", "", jsonArray.getTypeString(6));
    JSUtil.prettyPrint(Serial, m_json);

    std::vector<int> data;
    for (int i = 0; i < 10; i++) data.emplace_back(i);
    JsonArray jsArray(data);
    JSUtil.prettyPrint(Serial, jsArray);
}

void loop() {
    if (Serial.available()) {
        String str = Serial.readString();
        if (str.equals("a")) {
            Serial.println(ESP.getFreeHeap());
        } else {
            /*Get Json Array from PROGMEM*/
            JsonArray js(data);
            JSUtil.prettyPrint(Serial, js);
        }
    }
}
