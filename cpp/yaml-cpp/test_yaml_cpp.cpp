#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>

/***
 * g++ -O2 -Wall -std=c++11  -lyaml-cpp test_yaml_cpp.cpp -o test_yaml
 * @return
 */
int main()
{
    YAML::Node config = YAML::LoadFile("config.yaml");

    // judge field is exist.
    if (config["lastLogin"]) {
        std::cout << "Last logged in: " << config["lastLogin"].as<int64_t>() << "\n";
    }else {
        std::cout << "no field <lastLogin>!" << std::endl;
    }
    const std::string username = config["username"].as<std::string>();
    std::cout << "username == [" << username << "]" << std::endl;
    const std::string password = config["password"].as<std::string>();
    std::cout << "password == [" << password << "]" << std::endl;
    const std::string Unknown = config["Unknown"].as<std::string>();
    std::cout << "Unknown == [" << Unknown << "]" << std::endl;
    //login(username, password);
    config["lastLogin"] = 1000 ;

    std::ofstream fout("config.yaml", std::ios::out);
    fout << config << std::endl;
    fout.flush();
    fout.close();
    return 0;
}

/** file: config.yaml
username: sunquan
password: 123456
Unknown: xxx
lastLogin: 1000
*/
