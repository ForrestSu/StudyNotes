#include <iostream>
#include <google/protobuf/util/json_util.h>

#include "proto/protobuf3_json/test-protobuf.pb.h"

/**
 * reference: https://stackoverflow.com/questions/34906305/protocol-buffer3-and-json/44291335#44291335
 */

int main()
{
  std::string json_string;
  proto::HelloWorld hello;

  // Populate hello.
  hello.set_query(std::string("Hello!"));
  hello.set_page_number(1);
  hello.set_result_per_page(10);

  // Create a json_string from hello.
  google::protobuf::util::JsonPrintOptions options;
  options.add_whitespace = true;
  options.always_print_primitive_fields = true;
  options.preserve_proto_field_names = true;
  MessageToJsonString(hello, &json_string, options);

  // Print json_string.
  std::cout << json_string << std::endl;


  // Parse the json_string into hello2.
  proto::HelloWorld hello2;
  google::protobuf::util::JsonParseOptions options2;
  JsonStringToMessage(json_string, &hello2, options2);

  // Print the values of hello2.
  std::cout
    << hello2.query() << ", "
    << hello2.page_number() << ", "
    << hello2.result_per_page() << std::endl;

  return 0;
}