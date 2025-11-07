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
        int counter = 0;
        while (!stop && counter < MAXIMUM_ITERS){
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
            ++counter;
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

int operation_counter(std::string compact_string){
    std::vector<char> ops = {'*', '/', '+', '-'};
    int operation_count = 0;
    for (int i = 0; i < compact_string.length(); ++i){
        if (std::find(ops.begin(), ops.end(), compact_string[i]) != ops.end()) {
            ++operation_count;
        }
    }
    return operation_count;
}

bool operator_tree_check(char op_now, char op_check){
    std::map<char, int> values = {{'/', 3}, {'*', 2}, {'+', 1}, {'-', 0}};
    return values[op_now] >= values[op_check];
}

bool checker(std::vector<char> op_type){
    bool check = true;
    int length = op_type.size();
    for (int i = 0; i < length - 1; ++i){
        if (!operator_tree_check(op_type[i], op_type[i+1])){
            check = false;
        }
    }
    return check;
}

void update_op_idx(std::vector<int> op_idx, int insert_pos)
{
    for (int& idx : op_idx) {
        if (idx >= insert_pos) idx++;
    }
}

void update_indices(std::map<int, std::vector<int>> nearest_neighbors,
                    int insert_pos)
{
    std::map<int, std::vector<int>> updated;

    for (auto kv : nearest_neighbors) {
        int key = kv.first;              
        std::vector<int> neigh = kv.second;

        if (key >= insert_pos) key++;

        if (neigh[0] != 0 && neigh[0] >= insert_pos) neigh[0]++;

        if (neigh[1] >= insert_pos) neigh[1]++;

        updated[key] = neigh;
    }

    nearest_neighbors = updated;
}

void update_nn_idx_op(std::map<int, std::vector<int>>& nearest_neighbors, std::vector<int>& nearest_neighbor, std::string& no_bracket_string, std::vector<int>& op_idx, int i, std::vector<int>& sorted_op_idx){
    nearest_neighbor = nearest_neighbors[sorted_op_idx[i]];
    no_bracket_string.insert(nearest_neighbor[0], 1, '(');
    update_indices(nearest_neighbors, nearest_neighbor[0]);
    update_op_idx(op_idx, nearest_neighbor[0]);
    nearest_neighbor = nearest_neighbors[sorted_op_idx[i]];
    no_bracket_string.insert(nearest_neighbor[1] + 1, 1, ')');
    update_indices(nearest_neighbors, nearest_neighbor[1] + 1);
    update_op_idx(op_idx, nearest_neighbor[1] + 1);
}

std::string bracket_adder(std::string no_bracket_string) {
    std::vector<char> ops = {'*', '/', '+', '-'};
    std::map<char, int> ops_value = {{'/', 3}, {'*', 2}, {'+', 1}, {'-', 0}};
    int length = no_bracket_string.length();
    int op_count = 0;
    std::vector<int> op_idx;
    std::vector<char> op_type;

    if (length == 0){
        
    }

    int open_count = 0;
    int close_count = 0;

    for (int i = 0; i < length; ++i) {
        if (std::find(ops.begin(), ops.end(), no_bracket_string[i]) != ops.end()) {
            op_count++;
            op_idx.push_back(i);
            op_type.push_back(no_bracket_string[i]);
        }
        else if (no_bracket_string[i] == '('){
            ++open_count;
        }
        else if (no_bracket_string[i] == ')'){
            ++close_count;
        }
    }

    if (open_count > close_count){
        return "ERROR! Number of open and close brackets should be equal - too many opens!";
    }
    else if (open_count < close_count){
        return "ERROR! Number of open and close brackets should be equal - too many closes!";
    }
    if (op_count == 0) {
        return "ERROR! No operations found in equation";
    }
    else if (op_count == 1){
        return '(' + no_bracket_string + ')';
    }
    else {
        std::map<int, int> value_for_index;
        std::map<int, std::vector<int>> nearest_neighbors;

        for (int i = 0; i < op_idx.size(); ++i) {
            value_for_index[op_idx[i]] = ops_value[op_type[i]];
        }

        std::vector<int> original_op_idx = op_idx;

        for (int i = 0; i < original_op_idx.size(); ++i) {
            if (i == 0)
                nearest_neighbors[original_op_idx[i]] = {0, original_op_idx[i + 1]};
            else if (i == original_op_idx.size() - 1)
                nearest_neighbors[original_op_idx[i]] = {original_op_idx[i - 1] + 1, length};
            else
                nearest_neighbors[original_op_idx[i]] = {original_op_idx[i - 1] + 1, original_op_idx[i + 1]};
        }

        std::vector<int> sorted_op_idx = op_idx;
        std::sort(sorted_op_idx.begin(), sorted_op_idx.end(), [&](int a, int b) { return value_for_index[a] > value_for_index[b]; });

        op_type.clear();
        for (int idx : sorted_op_idx) {
            op_type.push_back(no_bracket_string[idx]);
        }

        std::vector<int> nearest_neighbor;

        for (int i = 0; i < original_op_idx.size(); ++i) {
            update_nn_idx_op(nearest_neighbors, nearest_neighbor, no_bracket_string, op_idx, i, sorted_op_idx);

            int length_of_bracket = nearest_neighbor[1] - nearest_neighbor[0];
            if (i+1 < sorted_op_idx.size()){
                print(sorted_op_idx[i+1], "sorted_op_idx[i+1]");
                print(sorted_op_idx[i],"sorted_op_idx[i]");
                if (sorted_op_idx[i+1] < sorted_op_idx[i]) {
                    nearest_neighbors[sorted_op_idx[i+1]][1] = nearest_neighbors[sorted_op_idx[i+1]][1] + length_of_bracket;
                }
                else {
                    nearest_neighbors[sorted_op_idx[i+1]][0] = nearest_neighbors[sorted_op_idx[i+1]][0] - length_of_bracket;
                }
            }
        }
        return no_bracket_string;
    }
}

std::string solve_line(std::string compact_string, int line){
    int bracket_count = bracket_counter(compact_string);
    int op_count = operation_counter(compact_string);
    bracket_count = bracket_counter(compact_string);
    int counter = 0;
    while (bracket_count > 0 && counter < MAXIMUM_ITERS){
        compact_string = solver_inner_term(compact_string, line);
        error_handler(compact_string, line);
        bracket_count = bracket_counter(compact_string);
        op_count = operation_counter(compact_string);
        if (bracket_count == 0 && op_count > 0){
            compact_string = bracket_adder(compact_string);
        }
        else {
            return compact_string;
        }
        ++counter;
    }
}
