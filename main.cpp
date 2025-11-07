// Includes
#include "includes.h"

// My headers
#include "multi_line.h"


int main()
{
    std::cout << "Running" << std::endl;
    std::string lines = "9+1 * 3 / 1; 5+3";
    print(lines, "input_string:");
    std::vector<std::string> result = solve_lines(lines);    
    print(result, "result");
        
    return 0;
}
