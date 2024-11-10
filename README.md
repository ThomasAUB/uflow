# uflow

## Basic example

```cpp
#include "uflow.hpp"

struct Node : uflow::INode<int> {
    bool operator()(int& i) override {
        i--;
        return (i > 0);
    }
};

int main() {

    uflow::Flow<int> flow;

    Node n1, n2, n3;

    flow >> n1 >> n2 >> n3;

    flow(4);

    return 0;
}
```

## Switch

```cpp
#include "uflow.hpp"

struct Node : uflow::INode<int> {
    bool operator()(int& i) override {
        i--;
        return (i > 0);
    }
};

int main() {

    uflow::Flow<int> flow;

    uflow::BasicNode<int> n1([](int& i) { i++; return (i > 0); });
    uflow::BasicNode<int> n2([](int& i) { i++; return (i > 0); });
    uflow::BasicNode<int> n3([](int& i) { i++; return (i > 0); });

    uflow::Switch<3, int> sw;

    sw[0] >> n2;
    sw[1] >> n3;

    sw.set(0);

    flow >> n1 >> sw;

    flow(4);

    return 0;
}
```

## Fork

```cpp
#include "uflow.hpp"

int main() {

    uflow::Flow<int> flow;

    uflow::BasicNode<int> n1([](int& i) { i++; return (i > 0); });
    uflow::BasicNode<int> n2([](int& i) { i++; return (i > 0); });
    uflow::BasicNode<int> n3([](int& i) { i++; return (i > 0); });
    uflow::BasicNode<int> n4([](int& i) { i++; return (i > 0); });

    uflow::Fork<3, int> fork;

    fork[0] >> n2;
    fork[1] >> n3;
    fork[2] >> n4;

    flow >> n1 >> fork;

    flow(4);

    return 0;
}
```