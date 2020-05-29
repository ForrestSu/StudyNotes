#include <iostream>
#include <gflags/gflags.h>

DEFINE_bool(isvip, false, "If Is VIP");
DEFINE_string(ip, "127.0.0.1", "connect ip");
DEFINE_int32(port, 80, "listen port");

int main(int argc, char **argv)
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << "ip:" << FLAGS_ip << std::endl;
    std::cout << "port:" << FLAGS_port << std::endl;
    std::cout << "isvip:" << FLAGS_isvip << std::endl;
    return 0;
}

/**
 * ./gflags_test --help
 * ./gflags_test -ip "127.0.0.2" --port 8001 -isvip=true
 */
