#include "polish_notation/parser/parser.h"

#include <gtest/gtest.h>

#include <tuple>
#include <vector>

#include "polish_notation/token/token.h"
#include "queue_test.h"
#include "token_test.h"

using namespace polish_notation::parser;

using polish_notation::tests::data_structures::queue_test::QueueTest;
using polish_notation::tests::token_test::TokenTest;
using polish_notation::token::t_id;
using polish_notation::token::Token;
using std::string;
using std::vector;

// getLineWithoutSpaces tests.
TEST(GetLineWithoutSpacesTest, PassEmptyLine) {
    string src("");

    string result = getLineWithoutSpaces(src);

    EXPECT_EQ(result, src);
    EXPECT_EQ(result.size(), 0);
}

TEST(GetLineWithoutSpacesTest, PassLineWithoutSpaces) {
    string src("line_without_spaces");

    string result = getLineWithoutSpaces(src);

    EXPECT_EQ(result, src);
}

TEST(GetLineWithoutSpacesTest, PassLineOnlyWithSpaces) {
    string src("   ");

    string result = getLineWithoutSpaces(src);

    EXPECT_EQ(result.empty(), true);
}

TEST(GetLineWithoutSpacesTest, PassLineStartsWithSpaces) {
    string src("   line");
    string expected("line");

    string result = getLineWithoutSpaces(src);

    EXPECT_EQ(result, expected);
}

TEST(GetLineWithoutSpacesTest, PassLineEndsWithSpaces) {
    string src("line    ");
    string expected("line");

    string result = getLineWithoutSpaces(src);

    EXPECT_EQ(result, expected);
}

TEST(GetLineWithoutSpacesTest, PassLineWithSingleSpacesBetweenWords) {
    string src("it's a line with single spaces");
    string expected("it'salinewithsinglespaces");

    string result = getLineWithoutSpaces(src);

    EXPECT_EQ(result, expected);
}

TEST(GetLineWithoutSpacesTest,
     PassLineWithSpacesAtTheStartAndEndAndBetweenWords) {
    string src("    it's    a line   with single spaces   ");
    string expected("it'salinewithsinglespaces");

    string result = getLineWithoutSpaces(src);

    EXPECT_EQ(result, expected);
}

// trySetTokenQueueFromStr tests.
namespace try_set_token_queue_from_str {
class StrWithTokensGenerator
    : public testing::TestWithParam<std::pair<
          const std::string,
          std::tuple<const std::string, QueueTest<TokenTest>, bool>>> {};

INSTANTIATE_TEST_SUITE_P(
    TrySetTokenQueueFromStrGenerator, StrWithTokensGenerator,
    testing::Values(
        std::pair<const std::string,
                  std::tuple<const std::string, QueueTest<TokenTest>, bool>>(
            {"EmptyStr", {std::string(), QueueTest<TokenTest>(), true}}),
        std::pair<const std::string,
                  std::tuple<const std::string, QueueTest<TokenTest>, bool>>(
            {"NumberWithDotThatHasNoFractPartIsNotPass",
             {"sin+55.",
              QueueTest<TokenTest>({TokenTest(t_id::sin), TokenTest(t_id::plus),
                                    TokenTest(55)}),
              false}}),
        std::pair<const std::string,
                  std::tuple<const std::string, QueueTest<TokenTest>, bool>>(
            {"RandomCase",
             {"sqrt(cosx+5)xx)",
              QueueTest<TokenTest>(
                  {TokenTest(t_id::sqrt), TokenTest(t_id::lBrace),
                   TokenTest(t_id::cos), TokenTest(t_id::x),
                   TokenTest(t_id::plus), TokenTest(5), TokenTest(t_id::rBrace),
                   TokenTest(t_id::x), TokenTest(t_id::x),
                   TokenTest(t_id::rBrace)}),
              true}})),
    [](const testing::TestParamInfo<StrWithTokensGenerator::ParamType>& info) {
        return info.param.first;
    });

TEST_P(StrWithTokensGenerator, PassNullptrStr) {
    auto in = GetParam();
    std::string src(::std::get<0>(in.second));
    QueueTest<TokenTest> result;
    QueueTest<TokenTest> expected(::std::get<1>(in.second));

    bool isCorrect = trySetTokenQueueFromStr((QueueTest<Token>&) result, src);

    ASSERT_EQ(result.size(), expected.size());
    ASSERT_EQ(isCorrect, ::std::get<2>(in.second));
    while (!result.isEmpty())
        EXPECT_EQ(result.dequeue(), expected.dequeue());
}
} // namespace try_set_token_queue_from_str

// trySetTokenFromStr tests.
TEST(TrySetTokenFromStrTest, PassNullptrStr) {
    const char* src = nullptr;
    TokenTest result {};

    int indent = trySetTokenFromStr(result, src);

    EXPECT_EQ(result, TokenTest());
    EXPECT_EQ(indent, -1);
}

TEST(TrySetTokenFromStrTest, PassEmptyLine) {
    const char* src = "";
    TokenTest result {};

    int indent = trySetTokenFromStr(result, src);

    EXPECT_EQ(result, TokenTest());
    EXPECT_EQ(indent, -1);
}

TEST(TrySetTokenFromStrTest, PassLineWithIntegerNmbAndDotWithChars) {
    const char* src = "555.abc";
    TokenTest result {};

    int indent = trySetTokenFromStr(result, src);

    EXPECT_EQ(result, TokenTest(555));
    EXPECT_EQ(indent, 3);
}

TEST(TrySetTokenFromStrTest, PassLineWithOneCharTokenSubstr) {
    vector<const char*> srcs(
        {"xabc", "+abc", "-abc", "*abc", "/abc", "(abc", ")abc"});
    vector<TokenTest> srcTokens(
        {TokenTest(TokenTest::Id::x), TokenTest(TokenTest::Id::plus),
         TokenTest(TokenTest::Id::minus), TokenTest(TokenTest::Id::mult),
         TokenTest(TokenTest::Id::div), TokenTest(TokenTest::Id::lBrace),
         TokenTest(TokenTest::Id::rBrace)});

    TokenTest result;
    for (int i {}; i < srcs.size(); ++i) {
        int indent = trySetTokenFromStr(result, srcs[i]);

        EXPECT_EQ(result, srcTokens[i]);
        EXPECT_EQ(indent, 1);
    }
}

TEST(TrySetTokenFromStrTest, PassLineWithTwoCharTokenSubstr) {
    const char* src = "lnabc";
    TokenTest result {};

    int indent = trySetTokenFromStr(result, src);

    EXPECT_EQ(result, TokenTest(TokenTest::Id::ln));
    EXPECT_EQ(indent, 2);
}

TEST(TrySetTokenFromStrTest, PassLineWithThreeCharTokenSubstr) {
    vector<const char*> srcs({"sinabc", "cosabc", "tanabc", "ctgabc"});
    vector<TokenTest> srcTokens(
        {TokenTest(TokenTest::Id::sin), TokenTest(TokenTest::Id::cos),
         TokenTest(TokenTest::Id::tan), TokenTest(TokenTest::Id::ctg)});

    TokenTest result;
    for (int i {}; i < srcs.size(); ++i) {
        int indent = trySetTokenFromStr(result, srcs[i]);

        EXPECT_EQ(result, srcTokens[i]);
        EXPECT_EQ(indent, 3);
    }
}

TEST(TrySetTokenFromStrTest, PassLineWithFourCharTokenSubstr) {
    const char* src = "sqrtabc";
    TokenTest result {};

    int indent = trySetTokenFromStr(result, src);

    EXPECT_EQ(result, TokenTest(TokenTest::Id::sqrt));
    EXPECT_EQ(indent, 4);
}

TEST(TrySetTokenFromStrTest, PassLineWithErrorSubstr) {
    const char* src = "sQrtabc";
    TokenTest result {};

    int indent = trySetTokenFromStr(result, src);

    EXPECT_EQ(result, TokenTest());
    EXPECT_EQ(indent, -1);
}

// setNumberFromStr tests.
TEST(SetNumberFromStrTest, PassNullptrStr) {
    const char* src = nullptr;
    double expectedNumber = 0.;
    int expectedIndent = 0;
    double result {};

    int indent = setNumberFromStr(result, src);

    EXPECT_EQ(result, expectedNumber);
    EXPECT_EQ(indent, expectedIndent);
}

TEST(SetNumberFromStrTest, PassEmptyStr) {
    const char* src = "";
    double expectedNumber = 0.;
    int expectedIndent = 0;
    double result {};

    int indent = setNumberFromStr(result, src);

    EXPECT_EQ(result, expectedNumber);
    EXPECT_EQ(indent, expectedIndent);
}

TEST(SetNumberFromStrTest, PassLineWithIntegerDigit) {
    const char* src = "5";
    double expectedNumber = 5.;
    int expectedIndent = 1;
    double result {};

    int indent = setNumberFromStr(result, src);

    EXPECT_EQ(result, expectedNumber);
    EXPECT_EQ(indent, expectedIndent);
}

TEST(SetNumberFromStrTest, PassLineWithIntegerDigits) {
    const char* src = "555";
    double expectedNumber = 555.;
    int expectedIndent = 3;
    double result {};

    int indent = setNumberFromStr(result, src);

    EXPECT_EQ(result, expectedNumber);
    EXPECT_EQ(indent, expectedIndent);
}

TEST(SetNumberFromStrTest, PassLineWithIntegerDigitsInTheBeginning) {
    const char* src = "555abc";
    double expectedNumber = 555.;
    int expectedIndent = 3;
    double result {};

    int indent = setNumberFromStr(result, src);

    EXPECT_EQ(result, expectedNumber);
    EXPECT_EQ(indent, expectedIndent);
}

TEST(SetNumberFromStrTest, PassLineWithDoubleNumber) {
    const char* src = "5.5";
    double expectedNumber = 5.5;
    int expectedIndent = 3;
    double result {};

    int indent = setNumberFromStr(result, src);

    EXPECT_EQ(result, expectedNumber);
    EXPECT_EQ(indent, expectedIndent);
}

TEST(SetNumberFromStrTest, PassLineWithTwoFractionParts) {
    const char* src = "55.5.55";
    double expectedNumber = 55.5;
    int expectedIndent = 4;
    double result {};

    int indent = setNumberFromStr(result, src);

    EXPECT_EQ(result, expectedNumber);
    EXPECT_EQ(indent, expectedIndent);
}

TEST(SetNumberFromStrTest, PassLineWithNonDigitsAtTheEnd) {
    const char* src = "55.55abc";
    double expectedNumber = 55.55;
    int expectedIndent = 5;
    double result {};

    int indent = setNumberFromStr(result, src);

    EXPECT_EQ(result, expectedNumber);
    EXPECT_EQ(indent, expectedIndent);
}

TEST(SetNumberFromStrTest, PassLineWithNumberWithTheroesAtTheBeginning) {
    const char* src = "005.005abc";
    double expectedNumber = 5.005;
    int expectedIndent = 7;
    double result {};

    int indent = setNumberFromStr(result, src);

    EXPECT_EQ(result, expectedNumber);
    EXPECT_EQ(indent, expectedIndent);
}

// convertStrPartToDouble tests.
TEST(ConvertStrPartToDoubleTest, PassNullptrStr) {
    const char* src = nullptr;
    double expected = 0.;

    double result = convertStrPartToDouble(src);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToDoubleTest, PassEmptyLine) {
    const char* src = "";
    double expected = 0.;

    double result = convertStrPartToDouble(src);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToDoubleTest, PassLineWithOnlyIntegerDigits) {
    const char* src = "555";
    double expected = 555.;

    double result = convertStrPartToDouble(src);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToDoubleTest, PassLineWithOnlyDoubleDigits) {
    const char* src = "555.55";
    double expected = 555.55;

    double result = convertStrPartToDouble(src);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToDoubleTest, PassLineWithOneDigitInIntPart) {
    const char* src = "0.555";
    double expected = 0.555;

    double result = convertStrPartToDouble(src);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToDoubleTest, PassLineWithOneDigitInFractPart) {
    const char* src = "555.5";
    double expected = 555.5;

    double result = convertStrPartToDouble(src);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToDoubleTest, PassLineWithTheroesAtTheFractPart) {
    const char* src = "5.000";
    double expected = 5.;

    double result = convertStrPartToDouble(src);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToDoubleTest, PassLineWithTheroesInIntAndFractPart) {
    const char* src = "0.0";
    double expected = 0.;

    double result = convertStrPartToDouble(src);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToDoubleTest, PassLineWithDigitsAndSymbolsAtTheBeginning) {
    const char* src = "abc555.55";
    double expected = 555.55;

    double result = convertStrPartToDouble(src + 3);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToDoubleTest, PassLineWithDigitsAndSymbolsAtTheEnd) {
    const char* src = "555.5cba";
    double expected = 555.5;

    double result = convertStrPartToDouble(src, 5);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToDoubleTest,
     PassLineWithDigitsAndSymbolsAtTheBeginningAndTheEnd) {
    const char* src = "abc555.555cba";
    double expected = 555.555;

    double result = convertStrPartToDouble(src + 3, 7);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToDoubleTest, CountIsLessThanDigitsInIntNum) {
    const char* src = "555";
    double expected = 55;

    double result = convertStrPartToDouble(src, 2);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToDoubleTest, CountIsLessThanDigitsInFractNum) {
    const char* src = "555.555";
    double expected = 555.5;

    double result = convertStrPartToDouble(src, 5);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToDoubleTest, CountCoversIntDigitsAndDot) {
    const char* src = "555.555";
    double expected = 555;

    double result = convertStrPartToDouble(src, 4);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToDoubleTest, CountIsBiggerThanDigitsInFractNum) {
    const char* src = "555.5";
    double expected = 555.5;

    double result = convertStrPartToDouble(src, 7);

    EXPECT_EQ(result, expected);
}

// convertStrPartToInt tests.
TEST(ConvertStrPartToIntTest, PassNullptrStr) {
    const char* src = nullptr;
    int expected = 0;

    int result = convertStrPartToInt(src);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToIntTest, PassEmptyLine) {
    const char* src = "";
    int expected = 0;

    int result = convertStrPartToInt(src);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToIntTest, PassLineWithOnlyDigits) {
    const char* src = "555";
    int expected = 555;

    int result = convertStrPartToInt(src);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToIntTest, PassLineWithTheroesAtTheBeginning) {
    const char* src = "005";
    int expected = 5;

    int result = convertStrPartToInt(src);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToIntTest, PassLineWithDigitsAndSymbolsAtTheBeginning) {
    const char* src = "abc555";
    int expected = 555;

    int result = convertStrPartToInt(src + 3);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToIntTest, PassLineWithDigitsAndSymbolsAtTheEnd) {
    const char* src = "555cba";
    int expected = 555;

    int result = convertStrPartToInt(src, 3);

    EXPECT_EQ(result, expected);
}

TEST(ConvertStrPartToIntTest,
     PassLineWithDigitsAndSymbolsAtTheBeginningAndTheEnd) {
    const char* src = "abc555cba";
    int expected = 555;

    int result = convertStrPartToInt(src + 3, 3);

    EXPECT_EQ(result, expected);
}