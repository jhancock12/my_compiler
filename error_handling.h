#include <cstdlib>
// so far:
// std::string, std::vector<std::string>, std::tuple<std::string, std::vector<int>> "ERROR!"

void print_error(std::string message, int line = 0){
    std::cout << "-----------------------" << std::endl;
    std::cout << "We have encountered an error!" << std::endl;
    std::cout << "Namely: " << message << std::endl;
    std::cout << "This occured in line: " << line << std::endl;
    std::cout << "-----------------------" << std::endl;
    exit(1);
}

void error_handler(std::string message, int line = 0){
    if (message.substr(0,6) == "ERROR!"){
        print_error(message, line);
    }
}

void error_handler(std::vector<std::string> message, int line = 0){
    if (message[0].substr(0,6) == "ERROR!"){
        print_error(message[0], line);
    }
}

void error_handler(std::tuple<std::string, std::vector<int>>message, int line = 0){
    if (std::get<0>(message).substr(0,6) == "ERROR!"){
        print_error(std::get<0>(message), line);
    }
}