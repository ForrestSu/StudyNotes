package main

import (
	"fmt"
	"log"
	"testing"

	"./protos"
	jsonpb "github.com/golang/protobuf/jsonpb"
	proto "github.com/golang/protobuf/proto"
)

/**
reference:
Converting Protobuf3 with enum to JSON in Go
https://stackoverflow.com/questions/49962893/converting-protobuf3-with-enum-to-json-in-go
*/

func TestPb2BytesArray(t *testing.T) {
	test := &example.Test{
		Label: "hello",
		Type:  example.Exchange_TYPE_SZ,
		Reps:  []int64{1, 2, 3},
	}
	t.Log(test)

	data, err := proto.Marshal(test)
	if err != nil {
		t.Fatal("Marshaling error: ", err)
	}
	newTest := &example.Test{}
	err = proto.Unmarshal(data, newTest)
	if err != nil {
		t.Fatal("unMarshaling error: ", err)
	}
	// Now test and newTest contain the same data.
	if test.GetLabel() != newTest.GetLabel() {
		t.Fatalf("data mismatch %q != %q", test.GetLabel(), newTest.GetLabel())
	}
	// etc.
}

func TestPb2Json(t *testing.T) {
	//simpleJsonObj = `{"label":"hello","type":17,}`
	protoMsg := &example.Test{
		Label: "hello",
		Type:  example.Exchange_EX_UNKNOWN,
		Reps:  []int64{1, 2, 3},
	}

	m := jsonpb.Marshaler{
		EnumsAsInts:  true, //将枚举值转换为整型
		EmitDefaults: true, //是否忽略为缺省值的字段
		//Indent: "\t", //设置序列化后的缩进符
		OrigName: true, //是否使用字段原始的字段名称
	}
	log.Println(m)
	result, err := m.MarshalToString(protoMsg)
	if err != nil {
		t.Fatal("error: fail to marshal!", err)
	}
	//result is: {"label":"hello","type":0,"reps":["1","2","3"]}
	t.Log("result is:", result)
}

func TestJson2pb(t *testing.T) {
	jsonStr := `{"label":"hello","type":1,"reps":["1","2","3"]}`

	msg := &example.Test{}
	if err := jsonpb.UnmarshalString(jsonStr, msg); err != nil {
		t.Fatal(err)
	}
	// label:"hello" type:TYPE_SH reps:1 reps:2 reps:3
	t.Log(msg)
	fmt.Printf("%+v\n", proto.MarshalTextString(msg))
}
