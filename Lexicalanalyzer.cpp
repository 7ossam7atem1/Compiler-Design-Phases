#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iomanip>

using namespace std;

bool isPunctuator(char ch) {
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
        ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
        ch == '&' || ch == '|') {
        return true;
    }
    return false;
}

bool validIdentifier(char* str) {
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||
        str[0] == '9' || isPunctuator(str[0]) == true) {
        return false;   
    }
    int i, len = strlen(str);
    if (len == 1) {
        return true;
    } else {
        for (i = 1; i < len; i++) {
            if (isPunctuator(str[i]) == true) {
                return false;
            }
        }
    }
    return true;
}

bool isOperator(char ch) {
    if (ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == '>' || ch == '<' ||
        ch == '=' || ch == '|' || ch == '&') {
        return true;
    }
    return false;
}

bool Pre(char* str) {
    if (!strcmp(str, "if") || !strcmp(str, "else") ||
        !strcmp(str, "while") || !strcmp(str, "do") ||
        !strcmp(str, "break") || !strcmp(str, "continue") ||
        !strcmp(str, "int") || !strcmp(str, "double") ||
        !strcmp(str, "float") || !strcmp(str, "return") ||
        !strcmp(str, "char") || !strcmp(str, "case") ||
        !strcmp(str, "long") || !strcmp(str, "short") ||
        !strcmp(str, "typedef") || !strcmp(str, "switch") ||
        !strcmp(str, "unsigned") || !strcmp(str, "void") ||
        !strcmp(str, "static") || !strcmp(str, "struct") ||
        !strcmp(str, "sizeof") || !strcmp(str, "long") ||
        !strcmp(str, "volatile") || !strcmp(str, "typedef") ||
        !strcmp(str, "enum") || !strcmp(str, "const") ||
        !strcmp(str, "union") || !strcmp(str, "extern") ||
        !strcmp(str, "bool")) {
        return true;
    } else {
        return false;
    }
}

bool isNumber(char* str) {
    int i, len = strlen(str), numOfDecimal = 0;
    if (len == 0) {
        return false;
    }
    for (i = 0; i < len; i++) {
        if (numOfDecimal > 1 && str[i] == '.') {
            return false;
        } else if (numOfDecimal <= 1) {
            numOfDecimal++;
        }
        if (str[i] != '0' && str[i] != '1' && str[i] != '2' &&
            str[i] != '3' && str[i] != '4' && str[i] != '5' &&
            str[i] != '6' && str[i] != '7' && str[i] != '8' &&
            str[i] != '9' || (str[i] == '-' && i > 0)) {
            return false;
        }
    }
    return true;
}

char* subString(char* realStr, int l, int r) {
    int i;

    char* str = (char*)malloc(sizeof(char) * (r - l + 2));

    for (i = l; i <= r; i++) {
        str[i - l] = realStr[i];
        str[r - l + 1] = '\0';
    }
    return str;
}

void printTableRow(const char* category, const char* value) {
    cout << "| " << setw(19) << left << category << "| " << setw(29) << left << value << "|\n";
}

void parse(char* str) {
    int left = 0, right = 0;
    int len = strlen(str);

    while (right <= len && left <= right) {
        if (isPunctuator(str[right]) == false) {
            right++;
        }

        if (isPunctuator(str[right]) == true && left == right) {
            if (isOperator(str[right]) == true) {
                printTableRow("Operator", string(1, str[right]).c_str());
            }
            right++;
            left = right;
        } else if (isPunctuator(str[right]) == true && left != right ||
                   (right == len && left != right)) {
            char* sub = subString(str, left, right - 1);

            if (Pre(sub) == true) {
                printTableRow("Keyword", sub);
            } else if (isNumber(sub) == true) {
                printTableRow("Number", sub);
            } else if (validIdentifier(sub) == true &&
                       isPunctuator(str[right - 1]) == false) {
                printTableRow("Identifier", sub);
            } else if (validIdentifier(sub) == false &&
                       isPunctuator(str[right - 1]) == false) {
                printTableRow("Invalid Identifier", sub);
            }

            left = right;
        }
    }
}

int main() {
    ifstream inputFile("input.txt");

    if (!inputFile.is_open()) {
        cout << "Error while opening the file\n";
        exit(0);
    }

    char c[100];

    cout << "+-----------------+-------------------------+\n";
    cout << "|     Category    |          Value          |\n";
    cout << "+-----------------+-------------------------+\n";

    while (inputFile.getline(c, 100)) {
        parse(c);
    }

    cout << "+-----------------+-------------------------+\n";

    inputFile.close();

    return 0;
}
