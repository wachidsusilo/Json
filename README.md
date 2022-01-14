## Json Library for Arduino, ESP8266, and ESP32
A Simple yet easy to use Json library to _parse_ and _stringify_ a `Json` object. No need to allocate buffer, no need additional setup, just create your `Json` object and you are done!

## Example
Using `Json` is as simple as it should be!

### Parse Json from String
Just create `Json` object and the String into it's constructor.
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
json.add("null"    , (void*)NULL);
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
To get the value, you can cast it or just assign it to an existing variable.
```c++
String    value   = json["key"];
int       integer = json["integer"];
float     pi      = json["float"];
bool      isAlive = json["boolean"];
Json      obj     = json["object"];
JsonArray arr     = json["array"];  
```

### Modifying Elements of a Json Object
```c++
json["key"]      = "new value";
json["integer"]  = 5;
json["float"]    = 6.28;
json["boolean"]  = false;
json["object"]   = Json().add("key", "value");
json["array"]    = JsonArray().push("value");
```

### Miscellaneous
All functions are available here.
```c++
//Stringify a Json object
String payload = json.toString();

//Deleting element
json.erase("key");

//Get the type of the element
String type = json.getTypeString("key");

//Get the size of Json object
size_t size = json.size();

//Check whether an element does exist
bool isExist = json.contains("key");

//Attach debugger
Json::attachDebugger(Serial);

//Detach debugger
Json::detachDebugger();

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
