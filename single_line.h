#include "error_handling.h"

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

std::string bracket_to_result(std::string compact_string){
    std::string compact_string_1 = "";
    bool stop = false;
    int counter = 1;
    int length = compact_string.length();

    if (compact_string[0] != '('){
        return "ERROR! First symbol must be a '('";
    }
    else if (compact_string[compact_string.length() - 1] != ')'){
        return "ERROR! Final symbol must be a ')'";
    }
    else {
        std::vector<char> ops = {'+', '-', '*', '/'};
        char type_of_op;
        while (!stop){
            if (!(std::find(ops.begin(), ops.end(), compact_string[counter]) != ops.end())){
                compact_string_1 += compact_string[counter];
                counter++;
            }
            else {
                type_of_op = compact_string[counter];
                stop = true;
                counter++; 
            }
            if (counter == length){
                return "ERROR! No operations found in equation";
            }
        }
        stop = false;
        std::string compact_string_2 = compact_string.substr(counter, compact_string.length()-2);

        if (type_of_op == '/' && std::stod(compact_string_2) == 0.0){
            return "ERROR! Cannot divide by zero";
        }
        else{
            double result;
            if (type_of_op == '+'){
                result = std::stod(compact_string_1) + std::stod(compact_string_2);
            }
            else if (type_of_op == '-'){
                result = std::stod(compact_string_1) - std::stod(compact_string_2);
            }
            else if (type_of_op == '*'){
                result = std::stod(compact_string_1) * std::stod(compact_string_2);
            }
            else if (type_of_op == '/'){
                result = std::stod(compact_string_1) / std::stod(compact_string_2);
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
    std::vector<int> error_vector = {0};
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

std::string solver_inner_term(std::string compact_string, int line){
    std::tuple<std::string, std::vector<int>> parsed_string_idx = parse_brackets_get_idx(compact_string);
    error_handler(parsed_string_idx, line);
    std::string parsed_string = std::get<0>(parsed_string_idx);
    std::vector<int> idx = std::get<1>(parsed_string_idx);
    std::string result = bracket_to_result(parsed_string);
    error_handler(result, line);
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

bool operator_tree_check(char op_now, char op_check){
    std::map<char, int> values = {{'/', 3}, {'*', 2}, {'+', 1}, {'-', 0}};
    return values[op_now] >= values[op_check];
}


std::string bracket_adder(std::string no_bracket_string){
    // This may be somewhat more challenging than i thought, and may require a tree
    std::vector<char> ops = {'*', '/', '+', '-'};
    int length = no_bracket_string.length();
    int op_count = 0;
    std::vector<int> op_idx;
    std::vector<char> op_type;
    for (int i = 0; i < length; ++i){
        if ((std::find(ops.begin(), ops.end(), no_bracket_string[i]) != ops.end())){
            op_count++;
            op_idx.push_back(i);
            op_type.push_back(no_bracket_string[i]);
        }
    }

    // Order: *, /, +, -
    if (op_count == 0){
        return "ERROR! No operations found in equation";
    }
    else {
        // Thoughts:
        // Want to apply DMAS rules
        // This means that I want to determine an ordering, then which digits touch it
        // Then bracket around them, and then repeat, where we ignore the previous term
        //
        // Cases: 
        // a + b * c => (a+(b*c))
        // a*b/c => (a*(b/c))
        // a*a*a*a => (((a*a)*a)*a) - big endian
        // 
        // This means:
        // Find the position of the highest order operators in the list op_type
        // Start from the first instance of this, count forward and backwards
        // If we reach an end or another symbol, we open/close
        // Ones counting backwards should then place open
        // Ones counting forwards should be closes
        // Once the first set of brackets is added

        // I think I need to sort of the op_type based on the operator tree check
        std::vector<int> sorted_idx(op_idx);
        std::map<int, std::vector<int>> nearest_neighbors; // Remeber to add to these as we add brackets
        bool check = true;
        for (int i = 0; i < length - 1; ++i){
            if (!operator_tree_check(op_type[i], op_type[i+1])){
                check = false;
            }
        }
        int count = 0;
        std::vector<char> sorted_type;
        print(op_type, "op_type, before loop");
        while (!check && count < MAXIMUM_ITERS){
            std::vector<int> sorted_idx_temp;
            print("----------------");
            for (int i = 0; i < length; ++i){
                print(sorted_idx[i], "sorted_idx[i]");
                print(op_type[sorted_idx[i]], "op_type[sorted_idx[i]]");
                print(op_type[sorted_idx[(i+1) % sorted_idx.size()]], "op_type[sorted_idx[i+1 % length]]");
                if (!operator_tree_check(op_type[sorted_idx[i]], op_type[sorted_idx[(i+1) % sorted_idx.size()]])){
                    sorted_idx_temp.push_back(sorted_idx[(i+1) % sorted_idx.size()]);
                }
                else {
                    sorted_idx_temp.push_back(sorted_idx[i]);
                }
            }
            print(op_type, "op_type");
            sorted_idx.assign(sorted_idx_temp.begin(),sorted_idx_temp.end());
            for (int i = 0; i < length - 1; ++i){
            if (!operator_tree_check(op_type[i], op_type[i+1])){
                    check = false;
                }
            }
            sorted_type = {};
            for (int i : sorted_idx){
                sorted_type.push_back(op_type[i]);
            }
            print(sorted_type, "sorted_type");
            ++count;
        }
    }
    return no_bracket_string;
}


std::string solve_line(std::string compact_string, int line){
    int bracket_count = bracket_counter(compact_string);
    while (bracket_count > 0){
        compact_string = solver_inner_term(compact_string, line);
        error_handler(compact_string, line);
        bracket_count = bracket_counter(compact_string);
    }
    return compact_string;
}
