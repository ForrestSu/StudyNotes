#include <stdio.h>
#include <stdlib.h>
#include <json/json.h>
#include <string>
#include <assert.h>
#include <iostream>

/* 只定义一个jsonObj   -> ok 缺省为NullObj
 * ""                -> fail
 * "abc"             -> fail
 * "\"abc\""         -> ok  StringObj
 * "{}"              -> ok  isObject
 * "{\"code\":100}"  -> ok  isObject
 * "[]"              -> ok  Array
 */

void testIsNull()
{
    Json::Value jsonObj;
    printf("jsonObj is Null? <%d>\n", jsonObj.isNull());

    Json::Reader jsonReader;
    std::string val = "";
    bool is_success = jsonReader.parse(val, jsonObj);
    if(is_success == false){
        printf("error: <%s> \n", jsonReader.getFormattedErrorMessages().c_str());
    }
    printf("testIsNull(): jsonObj is Null?  <%d>\n", jsonObj.isNull());
}

void testIsString()
{
    Json::Value jsonObj;
    Json::Reader jsonReader;
    std::string val = "abc";
    bool is_success = jsonReader.parse(val, jsonObj);
    if (is_success == false) {
        printf("testIsString() error: <%s>\n", jsonReader.getFormattedErrorMessages().c_str());
    }
}

void testIsObject()
{
    Json::Value jsonObj;
    Json::Reader jsonReader;
    std::string val = "{}";
    bool is_success = jsonReader.parse(val, jsonObj);
    assert(is_success == true);
    printf("testIsObject():  jsonObj is Object?  <%d>\n", jsonObj.isObject());
}

void testIsArray()
{
    Json::Value jsonObj;
    Json::Reader jsonReader;
    std::string val = "[]";
    bool is_success = jsonReader.parse(val, jsonObj);
    assert(is_success == true);
    printf("testIsArray():  jsonObj is Array?  <%d>\n", jsonObj.isArray());
}

/**
 * 解析json可能发生异常
 */
void testParse() {

    Json::Value json_obj;
    std::string txt = "\"json\"";
    Json::Reader jsonReader;
    bool is_success = jsonReader.parse(txt, json_obj);
    assert(is_success == true);
    assert(json_obj.isString());
    try {
        /**
         * TODO 下一行会抛出异常，要求jsonObj.type = Null 或者 Object，否则抛出异常
         */
        json_obj["uuid"] = "xxx";
    } catch (Json::LogicError &e) {
        std::cout << e.what() <<std::endl;
    }
}

/**
 * g++ -Wall -O0 test_json.cpp -ljsoncpp -o main
 */
int main()
{
    testIsNull();
    testIsString();
    testIsObject();
    testIsArray();
    // parse
    testParse();
    return 0;
}
