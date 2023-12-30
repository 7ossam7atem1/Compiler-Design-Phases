#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string>
using namespace std;

enum TokenTypes {
    CustomCinToken, CustomCoutToken, CustomCinOpToken, CustomCoutOpToken, CustomLeftParenToken, CustomRightParenToken, CustomPlusToken, CustomMultToken, CustomIdentifierToken, CustomIntToken, CustomRealToken, CustomErrorToken, CustomEndSourceToken
};

class CustomParser {
private:
    ifstream inputFile;
    TokenTypes currentTokenType;

    TokenTypes checkReserved(string s) {
        if (s == "cin") return CustomCinToken;
        else if (s == "cout") return CustomCoutToken;
        else return CustomIdentifierToken;
    }

    TokenTypes getToken() {
        char ch;
        string s;
        int state = 0;

        while (state >= 0 && state <= 11 && !inputFile.eof()) {
            switch (state) {
                case 0:
                    inputFile.get(ch);
                    if (inputFile.eof()) return CustomEndSourceToken;
                    else if (isspace(ch)) { s = ch; state = 0; }
                    else if (isdigit(ch)) { s = ch; state = 1; }
                    else if (ch == '.') { s = ch; state = 2; }
                    else if (isalpha(ch)) { s = ch; state = 4; }
                    else if (ch == '<') state = 5;
                    else if (ch == '>') state = 6;
                    else if (ch == '+') state = 7;
                    else if (ch == '*') state = 8;
                    else if (ch == '(') state = 9;
                    else if (ch == ')') state = 10;
                    else state = 11;
                    break;
                       //3.14
                case 1:
                    inputFile.get(ch);
                    if (isdigit(ch)) { s += ch; state = 1; }
                    else if (ch == '.') state = 3;
                    else { inputFile.putback(ch); return CustomIntToken; }
                    break;

                case 2:
                    inputFile.get(ch);
                    if (isdigit(ch)) { s += ch; state = 2; }
                    else {
                        inputFile.putback(ch);
                        return CustomRealToken;
                    }
                    break;

                case 3:
				
                    inputFile.get(ch);
                    if (isdigit(ch)) { s += ch; state = 3; }
                    else {
                        inputFile.putback(ch);
                        return CustomRealToken;
                    }
                    break;

                case 4:
                    inputFile.get(ch);
                    if (isalnum(ch)) { s += ch; state = 4; }
                    else {
                        inputFile.putback(ch);
                        return checkReserved(s);
                    }
                    break;

                case 5:
                    inputFile.get(ch);
                    if (ch == '<') return CustomCoutOpToken;
                    else return CustomErrorToken;
                    break;

                case 6:
                    inputFile.get(ch);
                    if (ch == '>') return CustomCinOpToken;
                    else return CustomErrorToken;
                    break;

                case 7:
                    return CustomPlusToken; break;

                case 8:
                    return CustomMultToken; break;

                case 9:
                    return CustomLeftParenToken; break;

                case 10:
                    return CustomRightParenToken; break;

                case 11:
                    return CustomErrorToken; break;
            }
        }
        return CustomEndSourceToken;
    }

    string getTokenName(TokenTypes t) {
        switch (t) {
            case CustomCinToken: return "Custom Cin Token"; break;
            case CustomCoutToken: return "Custom Cout Token"; break;
            case CustomCinOpToken: return "Custom >> Token"; break;
            case CustomCoutOpToken: return "Custom << Token"; break;
            case CustomIdentifierToken: return "Custom Identifier Token"; break;
            case CustomIntToken: return "Custom Int Token"; break;
            case CustomRealToken: return "Custom Real Token"; break;
            case CustomPlusToken: return "Custom + Token"; break;
            case CustomMultToken: return "Custom * Token"; break;
            case CustomLeftParenToken: return "Custom ( Token"; break;
            case CustomRightParenToken: return "Custom ) Token"; break;
            case CustomErrorToken: return "Custom Error Token"; break;
            case CustomEndSourceToken: return "Custom End of File"; break;
			
        }
        return "Custom Error Token";
    }

public:

    CustomParser(string filename) {
        inputFile.open(filename.c_str());
    }

    ~CustomParser() {
        inputFile.close();
    }

    void match(TokenTypes terminal) {
        if (currentTokenType == terminal)
            cout << getTokenName(currentTokenType) << " is matched the language" << endl;
        else
            syntaxError(currentTokenType);
        currentTokenType = getToken();
    }

    void syntaxError(TokenTypes terminal) {
        cout << getTokenName(terminal) << " is not included/Syntax Error\n";
    }

    void sampleParser() {
        currentTokenType = getToken();
        program();
        match(CustomEndSourceToken);
    }

    void program() {
        if (currentTokenType == CustomCinToken) {
            match(CustomCinToken);
            match(CustomCinOpToken);
            expressionSequence();
        } else if (currentTokenType == CustomCoutToken) {
            match(CustomCoutToken);
            match(CustomCoutOpToken);
            expressionSequence();
        } else
            syntaxError(currentTokenType);
    }

    void expressionSequence() {
        expression();
        while (currentTokenType == CustomIdentifierToken || currentTokenType == CustomIntToken || currentTokenType == CustomRealToken ||
               currentTokenType == CustomLeftParenToken || currentTokenType == CustomRightParenToken) {
            expression();
        }
    }

    void expression() {
        switch (currentTokenType) {
            case CustomIdentifierToken:
                match(CustomIdentifierToken);
                break;
            case CustomIntToken:
                match(CustomIntToken);
                break;
            case CustomRealToken:
                match(CustomRealToken);
                break;
            case CustomLeftParenToken:
                match(CustomLeftParenToken);
                if (currentTokenType == CustomPlusToken) {
                    match(CustomPlusToken);
                    expressionSequence();
                    match(CustomRightParenToken);
                } else if (currentTokenType == CustomMultToken) {
                    match(CustomMultToken);
                    expressionSequence();
                    match(CustomRightParenToken);
                } else {
                    syntaxError(currentTokenType);
                }
                break;
        }
    }
};

int main() {
    string inputFileName;
    cout << "Enter File Name You wan to check syntax: ";
    cin >> inputFileName;
    CustomParser customParser(inputFileName);
    customParser.sampleParser();
    customParser.~CustomParser();
    return 0;
}
