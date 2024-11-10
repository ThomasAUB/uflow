#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "uflow/uflow.hpp"


TEST_CASE("basic uFlow tests") {

    struct NodeA : uflow::INode<int, int> {
        bool operator()(int& i, int&) override {
            i--;
            return (i > 0);
        }
    };

    struct NodeB : uflow::INode<int, int> {
        bool operator()(int& i, int&) override {
            i *= 2;
            return (i < 8);
        }
    };

    NodeB nb1, nb2, nb3;
    uflow::Flow<int, int> subFlow(nb1, nb2, nb3);

    NodeA n1, n2, n3, n4, n5, n6, n7;
    uflow::Flow<int, int> flow;

    NodeA ns1, ns2;
    uflow::Fork<2, int, int> fork;

    fork[0] >> ns1;
    fork.get<1>() >> ns2;

    uflow::Switch<3, int, int> sw;

    sw[0] >> n1;

    sw.set(0);

    uflow::BasicNode<int, int> bn([] (int& a, int& b) { return a < 60; });

    flow >> n1 >> n2 >> n3 >> subFlow >> fork >> sw >> bn >> n4 >> n5 >> n6 >> n7;

    flow(5, 2);

}

