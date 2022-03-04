## Json Library for ESP8266 and ESP32
A Simple and easy to use Json library to _parse_ and _stringify_ a `Json` object. No need to allocate buffer, no need additional setup, just create your `Json` object and you are done!

## Example
Using `Json` is as simple as it should be!

### Parse Json from String
Just create `Json` object and put the String into it's constructor.
```c++
Json      json("{\"name\":\"John\", \"age\":30, \"car\":null}");
JsonArray jsonArray("[3.14, false, \"Hello World\"]");
```
### Adding Elements to a Json Object
Create empty `Json` object and fill it by calling `add`.
```c++
Json json;
json.add("key"     , "value");
json.add("integer" , 2);
json.add("float"   , 3.141592, 6);
json.add("boolean" , true);
json.add("null");
json.add("array"   , JsonArray()
                        .push(2)
                        .push(3.14)
                        .push(false)
                        .push("Hello"));
json.add("object"  , Json()
                        .add("key", "value")
                        .add("number", 12)
                        .add("bool", false));
```

### Retrieving Elements from a Json Object
Retrieving element is very easy, you can cast it or just assign it to an existing variable. If such element doesn't exist, a new element with the provided key will be created. Those newly created elements will contain value `null`.
```c++
//Assign elements to the existing variable
String    value   = json["key"];
int       integer = json["integer"];
float     pi      = json["float"];
bool      isAlive = json["boolean"];
Json      obj     = json["object"];
JsonArray arr     = json["array"];

//Retrieve elements by casting
Serial.println((String)    json["key"]);
Serial.println((int)       json["integer"]);
Serial.println((float)     json["float"]);
Serial.println((bool)      json["boolean"]);
Serial.println((Json)      json["object"]);
Serial.println((JsonArray) json["array"]);

//Retrieve elements using template (generic)
Serial.println(json["key"].as<String>());
Serial.println(json["integer"].as<int>());
Serial.println(json["float"].as<float>());
Serial.println(json["boolean"].as<bool>());
Serial.println(json["object"].as<Json>());
Serial.println(json["array"].as<JsonArray>());
```

### Modifying Elements of a Json Object
Assigning a direct child is very easy and straightforward. Assigning a nested object on the other hand, you need to cast it as a reference to be able to change it's value. The new value doesn't necessarily has the same type as the old value. You can assign the existing element with an arbitrary value and type.
```c++
json["key"]      = "new value";
json["integer"]  = "i am not an integer anymore";
json["float"]    = 6.28;
json["boolean"]  = "i am also not a boolean anymore";
json["object"]   = Json().add("key", "value");
json["array"]    = JsonArray().push("value");

//Nested Object
json["object"].as<Json&>()["key"] = "new value"
json["array"].as<JsonArray&>()[0] = "new value"
```

### Miscellaneous
Other methods are described here.
```c++
//Stringify a Json object
String payload = json.toString();

//Removing element
json.remove("key");

//Get the type of the element
String type = json.getTypeString("key");

//Get the size of Json object
size_t size = json.size();

//Check whether an element does exist
bool isExist = json.contains("key");

//Attach debugger
JSUtil.attachDebugger(Serial);

//Detach debugger
JSUtil.detachDebugger();

//Iterating element
for (auto& element : json) {
    Serial.print(element.first);
    Serial.print(" ");
    Serial.print(element.second.getValue());
    Serial.print(" ");
    Serial.print(JSUtil.typeToString(element.second.getType()));
}

//Stream Json object into Serial
JSUtil.prettyPrint(Serial, json);
```
