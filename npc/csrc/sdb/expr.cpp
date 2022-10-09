#include "expr.h"
#include "exprinternal.h"
#include "simtop.h"
namespace expr_namespace {

    Expr::Expr() {

        cout << "表达式求值模块初始化" << endl;
        init_regex();
    }

    Expr::~Expr() {
        cout << "表达式求值模块销毁" << endl;
    }


    /**
     * @brief 创建正则表达式规则
     *
     */
    void Expr::init_regex() {
        int i;
        char error_msg[128];
        int ret;
        for (i = 0; i < rules.size(); i++) {
            ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
            if (ret != 0) {
                regerror(ret, &re[i], error_msg, 128);
                printf("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
                exit(0);
            }
        }
    }
    /**
     * @brief 正则表达式规则匹配
     *
     * @param e 字符串
     * @return true
     * @return false
     */
    bool Expr::make_token(char* e) {
        int position = 0;
        int i;
        regmatch_t pmatch;
        nr_token = 0;
        while (e[position] != '\0') {
            /* Try all rules one by one. */
            for (i = 0; i < rules.size(); i++) {
                if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
                    char* substr_start = e + position;
                    int substr_len = pmatch.rm_eo;

                    // printf("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
                    //     i, rules[i].regex, position, substr_len, substr_len, substr_start);
                    position += substr_len;
                    /* 记录匹配规则,空格除外 */
                    if (rules[i].token_type != TK_NOTYPE) {
                        sprintf(tokens[nr_token].str, "%.*s", substr_len, substr_start);
                        tokens[nr_token].type = rules[i].token_type;
                        // printf("tokens->str:%s,tpye:%d,index:%d\n", tokens[nr_token].str, \
                        //     tokens[nr_token].type, nr_token);
                        nr_token++;
                    }
                    break;
                }
            }
            if (i == rules.size()) {
                printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
                return false;
            }
        }
        return true;
    }
    /**
     * @brief 得到表达式求值结果
     *
     * @param e 表达式
     * @param success
     * @return Expr::word_t
     */
    Expr::word_t Expr::getResult(char* e, bool* success) {
        if (!make_token(e)) {
            *success = false;
            printf("make_token fail!\n");
            exit(0);
            return 0;
        }
        Exprinternal expr_in(tokens, nr_token);

        for (size_t i = 0; i < nr_token; i++) {
            /* 如果是逻辑运算符 ，&& ｜｜，拆为两个表达式，目前只支持两个 */
            if (isLogicOprator(tokens[i])) {
                Exprinternal expr_left(tokens, i);
                Exprinternal expr_right(&tokens[i + 1], nr_token - (i + 1));
                uint64_t left_ret = expr_left.getResult();
                uint64_t right_ret = expr_right.getResult();
                uint64_t final_ret = getLogicResult(left_ret, right_ret, tokens[i]);
                return final_ret;
            }
        }

        uint64_t ret = expr_in.getResult();
        return ret;
    }


    bool Expr::isLogicOprator(Token t) {
        if (t.type == TK_AND
            || t.type == TK_OR) {
            return true;
        }
        return false;
    }

    bool Expr::getLogicResult(uint64_t leftval, uint64_t rightval, Token t) {
        bool ret = false;
        switch (t.type) {
        case TK_AND:
            if (leftval == 1 && rightval == 1) {
                ret = true;
            }
            break;
        case TK_OR:
            if (leftval == 1 || rightval == 1) {
                ret = true;
            }
            break;
        default:
            break;
        }
        return ret;
    }

    // /* 供外部调用 */
    // uint64_t exprgetResult(char* e, bool* success) {
    //     static Expr u_expr;
    //     uint64_t ret = u_expr.getResult(e, success);
    //     return ret;
    // }
}