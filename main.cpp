// Includes
#include "includes.h"

// My headers
#include "multi_line.h"

// Want to add a bracket ADDER to the single line stuff - mine doesnt work, need to rethink 

int main()
{
    std::cout << "Running" << std::endl;
    std::string inp = "(((7 + 9)    / 7)  / 6); (5  * 3); (5*   (3   +   2))";
    std::string no_bracket_string = "9+1 * 3";
    std::string compact_string = compactify_string(no_bracket_string);
    std::string result = bracket_adder(compact_string);
    
    
    return 0;
}
