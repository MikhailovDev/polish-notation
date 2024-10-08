#include "user_input.h"

#include <string>

#include "polish_notation/exceptions/type_conversion_error/type_conversion_error.h"
#include "polish_notation/parser/parser.h"

namespace polish_notation::user_input {
using polish_notation::renderer::FieldInfo;
using std::cout, std::endl, std::cin;

void setFunctionStr(::std::string& fStr) {
    cout << "Enter the function: ";
    getline(std::cin, fStr);
    cout << endl;
}

void setFieldInfo(FieldInfo& fInfo) {
    printActionAndSetAnswer("Enter field width: ", fInfo.width);
    printActionAndSetAnswer("Enter field height: ", fInfo.height);
    cout << endl;

    cout << "Enter center of coordinates (x, y): " << endl;
    printActionAndSetAnswer("Enter 'x': ", fInfo.centerOfCoordinates.first);
    printActionAndSetAnswer("Enter 'y': ", fInfo.centerOfCoordinates.second);
    cout << endl;

    cout << "Enter domain [n, m]: " << endl;
    printActionAndSetAnswer("Enter 'n': ", fInfo.domain.first);
    printActionAndSetAnswer("Enter 'm': ", fInfo.domain.second);
    cout << endl;

    cout << "Enter codomain [k, l]: " << endl;
    printActionAndSetAnswer("Enter 'k': ", fInfo.codomain.first);
    printActionAndSetAnswer("Enter 'l': ", fInfo.codomain.second);
}

template <typename T>
void printActionAndSetAnswer(const char* action, T& data) {
    cout << action;
    std::string str;
    (cin >> str).get();
    data = polish_notation::parser::convertStrPartToDouble(str.c_str());
}
} // namespace polish_notation::user_input