#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cctype>
#include <map>

const int MAXIMUM_ITERS = 1000;

int print(int number, std::string name = ""){
    std::cout << name << ": " << number << std::endl;
    return 0;
}

int print(char a, std::string name = ""){
    std::cout << name << ": " << a << std::endl;
    return 0;
}

int print(std::string word, std::string name = ""){
    std::cout << name << ": " << word << std::endl;
    return 0;
}

int print(std::vector<int> vec, std::string name = ""){
    std::cout << name << ": [";
    for (int i = 0; i < vec.size(); ++i){
        std::cout << vec[i] << ", ";
    }
    std::cout << "]" << std::endl;
    return 0;
}

int print(std::vector<char> vec, std::string name = ""){
    std::cout << name << ": [";
    for (int i = 0; i < vec.size(); ++i){
        std::cout << vec[i] << ", ";
    }
    std::cout << "]" << std::endl;
    return 0;
}

int print(std::vector<std::string> vec, std::string name = ""){
    std::cout << name << ": [";
    for (int i = 0; i < vec.size(); ++i){
        std::cout << vec[i] << ", ";
    }
    std::cout << "]" << std::endl;
    return 0;
}