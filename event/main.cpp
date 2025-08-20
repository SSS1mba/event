#include <iostream>
#include "Event.h"

void test_func_1(const std::string& msg)
{
    std::cout << "1" << msg << "\n";
}

void test_func_2(const std::string& msg)
{
    std::cout << "2" << msg << "\n";
}

int main()
{
    gms::Event<void, const std::string&> ev;
    
    ev.Subscribe([](const std::string& s) {std::cout << s <<"lambd\n";});
    ev.Subscribe(std::move(test_func_1));
    ev.Subscribe(&test_func_2);

    ev.Invoke("test");

    ////////////////////////
    void (*p1)(const std::string&) = test_func_1;
    void (*p2)(const std::string&) = test_func_1;

    bool res = p1 == p2;
}

