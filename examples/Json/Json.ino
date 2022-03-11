#include "data.h"
#include "Json.h"

void printValue(String name, String value, String type) {
    Serial.println("(" + type + ") " + name + ": " + value);
}

void setup() {
    Serial.begin(115200);
    JSUtil::attachDebugger(Serial);
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
    json.remove("number4");
    Serial.println("Value with name 'number4' has been deleted");

    /*Get type of specified name (JSType::Type)*/
    /**
     (JSType::Type) is an enumeration of Json Type
     including: jsonString, jsonInteger, jsonFloat,
     jsonBoolean, jsonNull, jsonArray, and jsonObject
  */
    Element::Type t = json.getType("name");

    /*Get type of specified name (String)*/
    /**
     This is Stringified version of (JSType::Type)
     including: string, integer, float, boolean,
     null, array, and object
  */
    String type = json.getTypeName("name");
    Serial.println("Type of 'name' is (" + type + ")");
    type = json.getTypeName("fields");
    Serial.println("Type of 'fields' is (" + type + ")");

    /*Get type and name(key) of Json Object*/
    Serial.println("\nThis Json Object contains:");
    for (int i = 0; i < json.size(); i++) {
        Serial.println("(" + json.getTypeName(i) + ") " + json.getKey(i));
    }

    /*Stringify Json Object without indentation (compact)*/
    Serial.println("\n*** Compact String ***");
    Serial.println(json.toString());

    /*Stringify Json Object with indentation*/
    Serial.println("\n*** Pretty String ***");
    JSUtil::prettyPrint(Serial, json);

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
    js.add("pointer");

    /*Create Array*/
    JsonArray jsonArray;
    jsonArray.push("jessica");    // insert String
    jsonArray.push(65432);        // insert int
    jsonArray.push(false);        // insert boolean
    jsonArray.push(5.234);        // insert float
    jsonArray.push();             // insert null
    jsonArray.push(JsonArray()    // insert Array
                       .push("height")
                       .push(165));
    jsonArray.push(Json()  // insert Json Object
                       .add("name", "Hannah")
                       .add("body-weight", 65));

    /*Add Array*/
    js.add("my-array", jsonArray);

    /*Create Object*/
    Json color;
    color.add("red", 65);
    color.add("green", 212);
    color.add("blue", 162);
    color.add("alpha", 255);

    /*Add Object*/
    js.add("color", color);

    Serial.println("\n++++ Equality Check +++++\n");
    Serial.println("Is name equal to 'michael'? " + String(js["name"] == "michael" ? "Yes" : "No"));
    Serial.println("Is year equal to 2021? " + String(js["year"] == 2021 ? "Yes" : "No"));
    Serial.println("Is pi equal to 3.14159265359? " + String(js["pi"] == 3.14159265359 ? "Yes" : "No"));
    Serial.println("Is isNotified equal to true? " + String(js["isNotified"] == true ? "Yes" : "No"));
    Serial.println("Is pointer equal to NULL? " + String(js["pointer"].isNull() ? "Yes" : "No"));
    Serial.println("Is my-array equal to " + jsonArray.toString() + "? " + String(js["my-array"] == jsonArray ? "Yes" : "No"));
    Serial.println("Is color equal to " + color.toString() + "? " + String(js["color"] == color ? "Yes" : "No"));

    Serial.println("\n++++ Inequality Check +++++\n");
    Serial.println("Is name not equal to 'michael'? " + String(js["name"] != "michael" ? "Yes" : "No"));
    Serial.println("Is year not equal to 2021? " + String(js["year"] != 2021 ? "Yes" : "No"));
    Serial.println("Is pi not equal to 3.14159265359? " + String(js["pi"] != 3.14159265359 ? "Yes" : "No"));
    Serial.println("Is isNotified not equal to true? " + String(js["isNotified"] != true ? "Yes" : "No"));
    Serial.println("Is pointer not equal to NULL? " + String(!js["pointer"].isNull() ? "Yes" : "No"));
    Serial.println("Is my-array not equal to " + jsonArray.toString() + "? " + String(js["my-array"] != jsonArray ? "Yes" : "No"));
    Serial.println("Is color not equal to " + color.toString() + "? " + String(js["color"] != color ? "Yes" : "No"));
    
    Serial.println("\n++++ Greater Than Check +++++\n");
    Serial.println("Is name is greater than 'jessica'? " + String(js["name"] > "jessica" ? "Yes" : "No"));
    Serial.println("Is pi greater than 3.14? " + String(js["pi"] > 3.14 ? "Yes" : "No"));
    Serial.println("Is year greater than 2020? " + String(js["year"] > 2020 ? "Yes" : "No"));

    Serial.println("\n++++ Less Than Check +++++\n");
    Serial.println("Is name is less than 'patrick'? " + String(js["name"] < "patrick" ? "Yes" : "No"));
    Serial.println("Is pi less than 3.15? " + String(js["pi"] < 3.15 ? "Yes" : "No"));
    Serial.println("Is year less than 2022? " + String(js["year"] < 2022 ? "Yes" : "No"));

    Serial.println("\n++++ Greater Than or Equal Check +++++\n");
    Serial.println("Is name is greater than or equal to 'jessica'? " + String(js["name"] >= "jessica" ? "Yes" : "No"));
    Serial.println("Is pi greater than or equal to 3.14? " + String(js["pi"] >= 3.14 ? "Yes" : "No"));
    Serial.println("Is year greater than or equal to 2020? " + String(js["year"] >= 2020 ? "Yes" : "No"));

    Serial.println("\n++++ Less Than or Equal Check +++++\n");
    Serial.println("Is name is less than or equal to 'patrick'? " + String(js["name"] <= "patrick" ? "Yes" : "No"));
    Serial.println("Is pi less than or equal to 3.15? " + String(js["pi"] <= 3.15 ? "Yes" : "No"));
    Serial.println("Is year less than or equal to 2022? " + String(js["year"] <= 2022 ? "Yes" : "No"));

    Serial.println("\n++++ Get value from a Json Object by using template (generic) +++++\n");
    Serial.println(js["name"].as<String>());
    Serial.println(js["year"].as<int>());
    Serial.println(js["isNotified"].as<bool>());
    Serial.println(js["pi"].as<float>());
    Serial.println(js["my-array"].as<JsonArray>().toString());
    Serial.println(js["color"].as<Json>().toString());

    Serial.println("\n++++ Get Value of Nested Object +++++\n");
    printValue("js(my-array)(0)", js["my-array"][0], js["my-array"][0].getTypeName());
    printValue("js(my-array)(1)", js["my-array"][1], js["my-array"][1].getTypeName());
    printValue("js(my-array)(2)", js["my-array"][2], js["my-array"][2].getTypeName());
    printValue("js(my-array)(3)", js["my-array"][3], js["my-array"][3].getTypeName());
    printValue("js(my-array)(4)", js["my-array"][4], js["my-array"][4].getTypeName());
    printValue("js(my-array)(5)", js["my-array"][5], js["my-array"][5].getTypeName());
    printValue("js(my-array)(5)(0)", js["my-array"][5][0], js["my-array"][5][0].getTypeName());
    printValue("js(my-array)(5)(1)", js["my-array"][5][1], js["my-array"][5][1].getTypeName());
    printValue("js(my-array)(6)", js["my-array"][6], js["my-array"][6].getTypeName());
    printValue("js(my-array)(6)(name)", js["my-array"][6]["name"], js["my-array"][6]["name"].getTypeName());
    printValue("js(my-array)(6)(body-weight)", js["my-array"][6]["body-weight"], js["my-array"][6]["body-weight"].getTypeName());

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
    printValue("name", name, js.getTypeName("name"));

    /*Get integerValue (32 bit)*/
    int year = js["year"];
    printValue("year", String(year), js.getTypeName("year"));

    /*Get booleanValue*/
    bool isNotified = js.getElement("isNotified");
    printValue("isNotified", isNotified ? "true" : "false", js.getTypeName("isNotified"));

    /*Set the value of an element*/
    js["isNotified"] = false;
    isNotified = js.getElement("isNotified");
    printValue("isNotified", isNotified ? "true" : "false", js.getTypeName("isNotified"));

    /*Get floatValue*/
    float piFloat = js["pi"];
    printValue("pi", String(piFloat, 15), js.getTypeName("pi"));

    /*Get doubleValue*/
    double piDouble = js.getElement("pi");
    printValue("pi", String(piDouble, 15), js.getTypeName("pi"));

    /*Get Array*/
    JsonArray arr = js.getElement("my-array");
    printValue("my-array", "", js.getTypeName("my-array"));
    JSUtil::prettyPrint(Serial, arr);

    /*Get Json Object*/
    Json obj = js["color"];
    printValue("color", "", js.getTypeName("color"));
    JSUtil::prettyPrint(Serial, obj);

    /*Set the value of a nested object*/
    js["color"].as<Json&>()["red"] = 255;
    obj = js["color"];
    printValue("color", "", js.getTypeName("color"));
    JSUtil::prettyPrint(Serial, obj);

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
    printValue("0", m_string, jsonArray.getTypeName(0));

    /*Get integerValue (32 bit)*/
    int m_int = jsonArray.getElement(1);
    printValue("1", String(m_int), jsonArray.getTypeName(1));

    /*Get booleanValue*/
    bool m_bool = jsonArray[2];
    printValue("2", m_bool ? "true" : "false", jsonArray.getTypeName(2));

    /*Get floatValue*/
    float m_float = jsonArray.getElement(3);
    printValue("3", String(m_float, 15), jsonArray.getTypeName(3));

    /*Get doubleValue*/
    double m_double = jsonArray[3];
    printValue("3", String(m_double, 15), jsonArray.getTypeName(3));

    /*Get Array*/
    JsonArray m_arr = jsonArray[5];
    printValue("5", "", jsonArray.getTypeName(5));
    JSUtil::prettyPrint(Serial, m_arr);

    /*Get Json Object*/
    Json m_json = jsonArray.getElement(6);
    printValue("6", "", jsonArray.getTypeName(6));
    JSUtil::prettyPrint(Serial, m_json);

    std::vector<int> data;
    for (int i = 0; i < 10; i++) data.emplace_back(i);
    JsonArray jsArray(data);
    JSUtil::prettyPrint(Serial, jsArray);
}

void loop() {
    if (Serial.available()) {
        String str = Serial.readString();
        if (str.equals("a")) {
            // Serial.println(ESP.getFreeHeap());
        } else {
            /*Get Json Array from PROGMEM*/
            JsonArray js(data);
            JSUtil::prettyPrint(Serial, js);
        }
    }
}
