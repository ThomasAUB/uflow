#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "uflow/uflow.hpp"


TEST_CASE("basic uFlow tests") {

    struct TextNode : uflow::INode<> {
        TextNode(std::string_view inText) : mText(inText) {}
        bool operator()() override {
            std::cout << mText << std::endl;
            return true;
        }
        std::string_view mText;
    };

    uflow::Flow flow, flow2;

    TextNode my("My");
    TextNode name("name");
    TextNode is("is");
    TextNode john("John");

    TextNode your("Your");
    TextNode jack("Jack");

    TextNode question("?");
    TextNode exclam("!");

    uflow::Switch<2> sw;

    flow >> my >> name;
    flow2 >> your >> name;

    name >> is >> sw;

    sw[0] >> jack;
    sw[1] >> john;

    uflow::Fork<2> fork;

    john >> fork;

    fork[0] >> question;
    fork[1] >> exclam;

    sw.select(0);
    flow();

    sw.select(1);
    flow2();

}

