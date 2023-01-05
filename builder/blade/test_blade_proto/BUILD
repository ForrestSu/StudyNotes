cc_binary(
  name = 'http_server',
  srcs = [
    'http_server.cpp',
  ],
  deps = [
    ':proto_http',
    '#brpc',
    '#gflags',
    '#pthread',
  ]
)

proto_library(
  name = 'proto_http',
  srcs = [
    'http.proto'
  ],
)
