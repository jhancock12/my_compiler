// Includes
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cctype>

// My headers
#include "single_line.h"

int main()
{
    // I am going to build a calculator
    // First thing is inputs, then operations, then I'll think again
    std::cout << "Running" << std::endl;
    std::string inp = "(((7 + 7)    / 7) / 6)";

    std::string compact_string = compactify_string(inp);

    compact_string = solve_line(compact_string);
    
    std::cout << "result: " << compact_string << std::endl;

    
    
    return 0;
}
