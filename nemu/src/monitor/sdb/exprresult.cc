#include <stack>
#include <iostream>
#include <vector>
#include <common.h>

using namespace std;

class Exprresult {
private:
    typedef struct token {
        int type;
        char str[32];
    } Token;

    enum {
        TK_NOTYPE = 256, TK_EQ,
        TK_NUM,
        /* TODO: Add more token types */

    };
    vector<Token> tokens;
    stack<uint32_t> stackOpre, stackNum;
public:
    Exprresult(void* tokens_addr, int num);
    ~Exprresult();
    void printTokens();
    bool isOperator(Token val);
    bool isPriority(Token val);
    uint32_t calculate();
    void negNum();
    uint32_t run1();
};




Exprresult::Exprresult(void* tokens_addr, int num) {
    Token* p = (Token*)tokens_addr;
    for (int i = 0; i < num; i++) {
        tokens.push_back(p[i]);
    }
    //printTokens();
    negNum();
    printTokens();
    //cout << "isPriority:" << isPriority(tokens.at(0)) << endl;
}

Exprresult::~Exprresult() {
    cout << "Exprresult end" << endl;
}

void Exprresult::printTokens() {

    cout << "Exprresult start" << endl;
    for (int i = 0; i < tokens.size(); i++) {
        cout << " str: " << tokens.at(i).str \
            << " type: " << tokens.at(i).type << endl;
    }



}

uint32_t Exprresult::run1() {
    bool ret;
    for (int i = 0; i < tokens.size(); i++) {
        ret = isOperator(tokens.at(i));
        /* 数字直接入栈 */
        if (!ret) {
            stackNum.push(atoi(tokens.at(i).str));
            continue;
        }
        ret = isPriority(tokens.at(i));
        /* 优先级低，出栈计算 */
        if (!ret) {
            stackNum.push(calculate());
        }
        /* 右括号单独处理 */
        if (tokens.at(i).type == ')') {
            while (stackOpre.top() != '(') {
                stackNum.push(calculate());
            }
            stackOpre.pop();
        }
        /* 不是右括号操作符入栈 */
        else {
            stackOpre.push(tokens.at(i).type);
        }
    }
    while (!stackOpre.empty()) {
        stackNum.push(calculate());
    }
    cout << "stackNumsize:" << stackNum.size() << endl;
    cout << "calculate:" << stackNum.top() << endl;
    cout << "stackOpsize:" << stackOpre.size() << endl;

    return stackNum.top();

}

bool Exprresult::isOperator(Token val) {
    if (val.type != this->TK_NUM) {
        return true;
    }
    return false;

}

/**
 * @brief () * / + -
 * 优先级高：可直接入栈 优先级低：进行一次calculate
 * @param val
 * @return true : 新加入的操作码优先级高
 * @return false：新加入的操作码优先级低
 */
bool Exprresult::isPriority(Token val) {


    /* 特殊情况处理：操作数少于两个,或者为括号 */
    if ((stackNum.size() < 2)) return true;
    uint32_t toptype;
    toptype = stackOpre.top();
    if (toptype == '(') return true;


    bool ret = false;
    switch (val.type) {
    case '*':
    case '/':
        if (!stackOpre.empty()) {
            if ('+' == toptype || '-' == toptype) {
                ret = true;
            }
        }
        break;
    case '+':
    case '-':
        ret = false;
        break;
    case '(':
    case ')':
        ret = true;
        break;
    default:
        cout << "type undefine!!!!!" << endl;
        break;
    }
    return ret;
}

uint32_t Exprresult::calculate() {
    /* 运算符出栈 */
    uint32_t op = stackOpre.top();
    stackOpre.pop();
    uint32_t numright = stackNum.top();
    stackNum.pop();
    uint32_t numleft = stackNum.top();
    stackNum.pop();

    uint32_t result;
    switch (op) {
    case '+':
        result = numleft + numright;
        break;
    case '-':
        result = numleft - numright;
        break;
    case '*':
        result = numleft * numright;
        break;
    case '/':
        result = numleft / numright;
        break;
    default:
        cout << "result undefine!!!!!" << endl;
        break;
    }

    return result;
}


void Exprresult::negNum() {
    Token numzore;
    numzore.type = TK_NUM;
    sprintf(numzore.str, "%d", 0);
    vector<Token> tokencopy(tokens);
    int count = 0;;
    //https://blog.csdn.net/hechao3225/article/details/55101344
    for (int i = 0; i < tokencopy.size(); i++) {
        if (tokencopy.at(i).type == '-') {

            if (i == 0) {
                tokens.emplace(tokens.begin() + count++, numzore);
            }
            else if (isOperator(tokencopy.at(i - 1))
                && (tokencopy.at(i - 1).type != ')')) {
                tokens.emplace(tokens.begin() + i + count++, numzore);
            }
        }
    }
}


/* 供外部调用 */
extern "C" uint32_t exprcpp(void* tokens_addr, int num) {

    Exprresult test(tokens_addr, num);
    return test.run1();
}


