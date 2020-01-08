# protoc --go_out=. inputs/x.protos
## writes ./github.com/golang/protobuf/p/x.pb.go
# protoc --go_out=paths=source_relative:. inputs/x.protos
## generate ./inputs/x.pb.go

protoc --go_out=paths=source_relative:. protos/test.proto