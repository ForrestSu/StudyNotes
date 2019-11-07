cc_binary(
  name = 'app',
  srcs = [
    'test-protobuf.cpp',
  ],
  deps = [
    ':test-proto',
    '#pthread',
  ]
)

proto_library(
  name = 'test-proto',
  srcs = [
    'test-protobuf.proto'
  ],
)
