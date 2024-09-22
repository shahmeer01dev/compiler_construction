#include <iostream>      // Library for input-output streams
#include <fstream>       // File stream library for file handling
#include <unordered_set> // Library for hash sets, it is used for average constant time complexity

using namespace std; // Allow the use of standard library names without prefixing std::

// Keywords in C++
const unordered_set<string> keywords = {
    "#include", "auto", "break", "case", "catch", "char", "class", "const", "continue",
    "default", "delete", "do", "double", "else", "enum", "explicit", "export",
    "extern", "float", "for", "goto", "if", "inline", "int", "long", "mutable",
    "namespace", "new", "operator", "private", "protected", "public", "register",
    "return", "short", "signed", "sizeof", "static", "struct", "switch",
    "template", "this", "throw", "try", "typedef", "typeid", "typename",
    "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", "true", "false", "bool"};

// Operators and Delimiters
const unordered_set<char> operators = {'+', '-', '*', '/', '=', '<', '>', '&', '|', '^', '%', '!', '~'};
const unordered_set<char> delimiters = {';', ',', ':', '(', ')', '{', '}', '[', ']', ' '};

// Function to process the file and remove comments
void processFile(const string &filename, string &cleanedCode)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Unable to open file" << endl;
        return;
    }

    string code, line;
    while (getline(file, line))
    {
        code += line + "\n";
    }
    file.close();

    // Remove single-line comments
    size_t pos;
    while ((pos = code.find("//")) != string::npos)
    {
        size_t end = code.find("\n", pos);
        code.erase(pos, end - pos);
    }

    // Remove multi-line comments
    while ((pos = code.find("/*")) != string::npos)
    {
        size_t end = code.find("*/", pos);
        if (end != string::npos)
        {
            code.erase(pos, end - pos + 2);
        }
        else
        {
            code.erase(pos); // If no closing comment found, erase everything from the position
            break;
        }
    }

    // Clean code: keep operators and delimiters but remove extra spaces
    for (char ch : code)
    {
        if (!isspace(ch) || delimiters.find(ch) != delimiters.end())
        {
            cleanedCode += ch;
        }
    }

    // Trim trailing characters (if any unwanted characters remain)
    if (!cleanedCode.empty() && cleanedCode.back() == '/')
    {
        cleanedCode.pop_back(); // Remove trailing '/'
    }
}

// void processFile(const string &filename, string &cleanedCode)
// {
//     ifstream file(filename);
//     if (!file.is_open())
//     {
//         cerr << "Unable to open file" << endl;
//         return;
//     }

//     string code, line;
//     while (getline(file, line))
//     {
//         code += line + "\n";
//     }
//     file.close();

//     // Remove single-line comments
//     size_t pos;
//     while ((pos = code.find("//")) != string::npos)
//     {
//         size_t end = code.find("\n", pos);
//         code.erase(pos, end - pos);
//     }

//     // Remove multi-line comments
//     while ((pos = code.find("/*")) != string::npos)
//     {
//         size_t end = code.find("*/", pos);
//         code.erase(pos, end - pos + 2);
//     }

//     // Clean code: keep operators and delimiters but remove extra spaces
//     for (char ch : code)
//     {
//         // !isspace(ch): Checks if the character is not a whitespace character (spaces, tabs, newlines)
//         // delimiters.find(ch) != delimiters.end(): Checks if the character is a delimiter
//         if (!isspace(ch) || delimiters.find(ch) != delimiters.end())
//         {
//             cleanedCode += ch;
//         }
//     }
// }

// Additional functions for token identification
// end() is a special iterator in C++ that doesn't represent an actual element; it represents a position just after the last element in the container.
bool isDelimiter(char ch)
{
    return delimiters.find(ch) != delimiters.end();
}

bool isOperator(char ch)
{
    return operators.find(ch) != operators.end();
}

bool isKeyword(const string &str)
{
    return keywords.find(str) != keywords.end();
}

bool validIdentifier(const string &str)
{
    if (isdigit(str[0]) || isDelimiter(str[0]))
        return false;
    for (char ch : str)
    {
        if (!isalnum(ch) && ch != '_') // only alphanumeric and underscore allowed
            return false;
    }
    return true;
}

bool isInteger(const string &str)
{
    for (char ch : str)
    {
        if (!isdigit(ch))
            return false;
    }
    return true;
}

bool isRealNumber(const string &str)
{
    bool hasDecimal = false;
    for (char ch : str)
    {
        if (ch == '.')
        {
            if (hasDecimal)
                return false; // Second decimal point
            hasDecimal = true;
        }
        else if (!isdigit(ch))
        {
            return false;
        }
    }
    return hasDecimal;
}

// Extract substring for parsing 
string subString(const string &str, int left, int length)
{
    return str.substr(left, length - left + 1);
}

// Parsing the input STRING (The process of transforming the data from one format to another)
void parse(const string &code, string &newCode)
{
    int left = 0, right = 0;
    int len = code.size();
    bool insideString = false;

    while (right <= len)
    {
        if (insideString)
        {
            if (code[right] == '"')
            {
                insideString = false;
                string subStr = subString(code, left, right);
                cout << "'" << subStr << "' : String Literal\n";
                newCode += subStr;
                left = right + 1;
            }
            right++;
            continue;
        }

        if (code[right] == '"')
        {
            insideString = true;
            left = right;
            right++;
            continue;
        }

        if (!isDelimiter(code[right]) && !isOperator(code[right]))
        {
            right++;
            continue;
        }

        // Check if left and right are valid for extracting a token
        if (left < right)
        {
            string subStr = subString(code, left, right - 1);

            if (isKeyword(subStr))
                cout << "'" << subStr << "'  :  Keyword\n";
            else if (isInteger(subStr))
                cout << "'" << subStr << "'  :  Integer\n";
            else if (isRealNumber(subStr))
                cout << "'" << subStr << "'  :  Real Number\n";
            else if (validIdentifier(subStr))
                cout << "'" << subStr << "'  :  Identifier\n";
            else
                cout << "'" << subStr << "'  :  Invalid Identifier\n";

            newCode += subStr; // Add valid token to newCode
        }

        if (isOperator(code[right]))
        {
            newCode += code[right];
            cout << "'" << code[right] << "'  :  Operator\n";
        }

        if (isDelimiter(code[right]) && code[right] != ' ')
        {
            newCode += code[right];
            cout << "'" << code[right] << "'  :  Delimiter\n";
        }

        left = right + 1; // Update left to current right position
        right++; // Move to next character
    }
}

// void parse(const string &code, string &newCode)
// {
//     int left = 0, right = 0;
//     int len = code.size();
//     bool insideString = false;

//     while (right <= len && left <= right)
//     {
//         if (insideString)
//         {
//             // Continue until we find the closing quote
//             if (code[right] == '"')
//             {
//                 insideString = false;
//                 string subStr = subString(code, left, right);
//                 cout << "'" << subStr << "' : String Literal\n";
//                 newCode += subStr;
//                 left = right + 1;
//             }
//             right++;
//             continue;
//         }

//         // If a double quote is found, it sets insideString to true and updates the left pointer.
//         if (code[right] == '"')
//         {
//             insideString = true;
//             left = right;
//             right++;
//             continue;
//         }

//         // If the current character is neither a delimiter nor an operator, the right pointer is incremented to continue reading the token.
//         if (!isDelimiter(code[right]) && !isOperator(code[right]))
//             right++;

//         // This block detects when a token ends (when a delimiter or operator is encountered, or at the end of the string).
//         // If a valid token is found (i.e., left is not equal to right), it extracts the substring.
//         if (isDelimiter(code[right]) || isOperator(code[right]) || right == len)
//         {
//             if (left != right)
//             {
//                 string subStr = subString(code, left, right - 1);

//                 if (isKeyword(subStr))
//                     cout << "'" << subStr << "'  :  Keyword\n";
//                 else if (isInteger(subStr))
//                     cout << "'" << subStr << "'  :  Integer\n";
//                 else if (isRealNumber(subStr))
//                     cout << "'" << subStr << "'  :  Real Number\n";
//                 else if (validIdentifier(subStr))
//                     cout << "'" << subStr << "'  :  Identifier\n";
//                 else
//                     cout << "'" << subStr << "'  :  Invalid Identifier (Identifiers must start with a letter or underscore and contain only alphanumeric characters or underscores)\n";

//                 // Add the token to the new code (without spaces)
//                 newCode += subStr;
//             }

//             // If it's an operator, add it to the new code
//             if (isOperator(code[right]))
//             {
//                 newCode += code[right];
//                 cout << "'" << code[right] << "'  :  Operator\n";
//             }

//             // If it's a delimiter, handle it (print and add to new code)
//             if (isDelimiter(code[right]) && code[right] != ' ')
//             {
//                 newCode += code[right];
//                 cout << "'" << code[right] << "'  :  Delimiter\n";
//             }

//             left = right + 1;
//         }
//         right++;
//     }
// }

int main()
{
    string filename = "sourceFile.cpp";
    string cleanedCode;
    processFile(filename, cleanedCode);

    cout << "\nToken     :     Lexeme\n\n";
    string newCode;
    parse(cleanedCode, newCode);

    cout << "\nCode after removing spaces:\n\n";
    cout << newCode << endl;

    return 0;
}
