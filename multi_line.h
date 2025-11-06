#include "single_line.h"

std::vector<std::string> parse_lines(std::string compact_lines){
    int length = compact_lines.length();
    std::vector<std::string> compact_string_list;
    if (length != 0){
        std::string compact_string_temp = "";
        for (int i = 0; i < length; ++i){
            compact_string_temp += compact_lines[i];
            if (compact_lines[i] == ';' || i == length - 1){
                compact_string_list.push_back(compact_string_temp);
                compact_string_temp = ""; 
            }
        }
        return compact_string_list;
    }
    else {
        return {"ERROR! Input cannot be of length zero"};
    }
}

std::vector<std::string> solve_lines(std::string compact_lines){
    std::vector<std::string> compact_string_list = parse_lines(compact_lines);
    int length_list = compact_string_list.size();
    std::string compact_string; 
    std::vector<std::string> solved_lines = {};
    for (int i = 0; i < length_list; ++i){
        compact_string = solve_line(compact_string_list[i], i);
        solved_lines.push_back(compact_string);
    }
    return solved_lines;
}