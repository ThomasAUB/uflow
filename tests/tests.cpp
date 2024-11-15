#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "uflow/uflow.hpp"


TEST_CASE("basic uFlow tests") {

    {
        struct Node : uflow::INode<int&> {
            bool operator()(int& i) override {
                i /= 2;
                return (i > 0);
            }
        };

        uflow::Flow<int&> flow;

        Node n1, n2, n3, n4, n5;

        flow >> n1 >> n2 >> n3 >> n4 >> n5;

        flow(6);

        int i = 6;
        flow(i);

        CHECK(i == 0);
    }

    ///////////////////////////////////////

    struct TextNode : uflow::INode<std::string&> {
        TextNode(std::string_view inText) : mText(inText) {}
        bool operator()(std::string& s) override {
            s += mText;
            return true;
        }
        const char* name() const { return mText.data(); }
        std::string_view mText;
    };

    struct CheckNode : uflow::INode<std::string&> {
        CheckNode(std::string_view inCheckString) :
            mCheckString(inCheckString) {}
        bool operator()(std::string& s) override {
            CHECK(s == mCheckString);
            return true;
        }
        const char* name() const override { return mCheckString.data(); }
        std::string_view mCheckString;
    };

    uflow::Flow<std::string&> flow, flow2;

    TextNode my("My ");
    TextNode name("name ");
    TextNode is("is ");
    TextNode john("John");

    TextNode your("Your ");
    TextNode jack("Jack");

    TextNode question("?");
    TextNode exclamation("!");

    uflow::Switch<2, std::string&> sw;

    flow >> my >> name >> is >> sw;
    flow2 >> your >> name;

    sw[0] >> jack;
    sw[1] >> john;

    uflow::Fork<2, std::string&> fork;

    john >> fork;

    fork[0] >> question;
    fork[1] >> exclamation;

    CheckNode cn1("My name is Jack");
    CheckNode cn2("Your name is John?");
    CheckNode cn3("Your name is John!");

    jack >> cn1;
    question >> cn2;
    exclamation >> cn3;

    sw.select(0);

    std::string str;
    flow(str); // "My name is Jack"

    CHECK(str == "My name is Jack");

    sw.select(1);

    str.clear();
    flow2(str); // "Your name is John?!"

    // check that the fork results were not taken into account
    CHECK(str == "Your name is John");


    flow.print(
        +[] (const char* txt) {
            std::cout << txt;
        }
    );

}

