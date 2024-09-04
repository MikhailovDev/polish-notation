#ifndef TOKEN_H
#define TOKEN_H

namespace polish_notation::token {
struct Token {
    enum class Id {
        num,
        x,
        plus,
        minus,
        mult,
        div,
        lBrace,
        rBrace,
        sin,
        cos,
        tan,
        ctg,
        sqrt,
        ln
    };

    enum class Priority { minus = 0, plus = 0, mult = 1, div = 1 };

    union Data {
        double num;
        Priority priority;
    };

    Id id;
    Data data;
};
} // namespace polish_notation::token

#endif // TOKEN_H