#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "uflow/uflow.hpp"


TEST_CASE("basic uFlow tests") {

    struct NodeA : uflow::INode<int> {

        bool operator()(int& i) override {
            i--;
            std::cout << "node A : " << i << std::endl;
            return (i > 0);
        }

    };

    struct NodeB : uflow::INode<int> {

        bool operator()(int& i) override {
            i *= 2;
            std::cout << "node B : " << i << std::endl;
            return (i > 0);
        }

    };



    NodeA n1, n2, n3, n4, n5, n6, n7;
    uflow::Flow<int> flow(n1, n2, n3, n4, n5, n6, n7);

    NodeB nb1, nb2, nb3;
    uflow::Flow<int> subFlow(nb1, nb2, nb3);

    flow.insert(0, subFlow);

    flow(5);

}

