#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cctype>

std::string compactify_string(std::string inp){
    int length = inp.length();
    if (length != 0){
        std::string compact_string = "";
        for (int i = 0; i < length; ++i){
            if (!std::isspace(inp[i])){
                compact_string += inp[i];
            } 
        }
        return compact_string;
    }
    else {
        return "ERROR! Input cannot be of length zero";
    } 
}

std::string bracket_to_result(std::string term){
    std::string term_1 = "";
    bool stop = false;
    int counter = 1;

    if (term[0] != '('){
        return "ERROR! First symbol must be a '('";
    }
    else if (term[term.length() - 1] != ')'){
        return "ERROR! Final symbol must be a ')'";
    }
    else {
        std::vector<char> ops = {'+', '-', '*', '/'};
        char type_of_op;
        while (!stop){
            if (!(std::find(ops.begin(), ops.end(), term[counter]) != ops.end())){
                term_1 += term[counter];
                counter++;
            }
            else {
                type_of_op = term[counter];
                stop = true;
                counter++; 
            }
        }
        stop = false;
        std::string term_2 = term.substr(counter, term.length()-2);

        if (type_of_op == '/' && std::stod(term_2) == 0.0){
            return "ERROR! Cannot divide by zero";
        }
        else{
            double result;
            if (type_of_op == '+'){
                result = std::stod(term_1) + std::stod(term_2);
            }
            else if (type_of_op == '-'){
                result = std::stod(term_1) - std::stod(term_2);
            }
            else if (type_of_op == '*'){
                result = std::stod(term_1) * std::stod(term_2);
            }
            else if (type_of_op == '/'){
                result = std::stod(term_1) / std::stod(term_2);
            }
            return std::to_string(result);
        }   
    }
    return "ERROR!";
}

std::tuple<std::string, std::vector<int>> parse_brackets_get_idx(std::string compact_string){
    int length = compact_string.length();
    int start_idx;
    int end_idx = 0;
    int bracket_count = 0;
    int close_count = 0;
    for (int i = 0; i < length; ++i){
        if (compact_string[i] == '('){
            ++bracket_count;
            start_idx = i;
        }
        else if (compact_string[i] == ')'){
            ++close_count;
        }
        if (close_count == 1 && end_idx == 0){
            end_idx = i;
        }
    }
    std::vector<int> error_vector = {0};
    if (bracket_count > close_count){
        return std::make_tuple("ERROR! Number of open and close brackets should be equal - too many opens!", error_vector);
    }
    else if (bracket_count < close_count){
        return std::make_tuple("ERROR! Number of open and close brackets should be equal - too many closes!", error_vector);
    }

    std::string parsed_string = "";
    for (int i = 0; i < (end_idx - start_idx + 1); ++i){
        parsed_string += compact_string[start_idx + i];
    }
    std::vector<int> res_vector = {start_idx, end_idx};
    return std::make_tuple(parsed_string, res_vector);
}

std::string solver_inner_term(std::string compact_string){
    std::tuple<std::string, std::vector<int>> parsed_string_idx = parse_brackets_get_idx(compact_string);
    std::string parsed_string = std::get<0>(parsed_string_idx);
    std::vector<int> idx = std::get<1>(parsed_string_idx);
    std::string result = bracket_to_result(parsed_string);
    compact_string.replace(idx[0], idx[1] - idx[0] + 1, result);
    return compact_string;
}

int bracket_counter(std::string compact_string){
    int bracket_count = 0;
    for (int i = 0; i < compact_string.length(); ++i){
        if (compact_string[i] == '('){
            ++bracket_count;
        }
    }
    return bracket_count;
}

std::string solve_line(std::string compact_string){
    int bracket_count = bracket_counter(compact_string);
    while (bracket_count > 0){
        compact_string = solver_inner_term(compact_string);
        bracket_count = bracket_counter(compact_string);
    }
    return compact_string;
}
