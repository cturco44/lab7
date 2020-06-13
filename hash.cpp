// Testing file provided for EECS 281 Lab 7

#include "hashtable.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;
void test_inf_loop() {
    HashTable<string, int> midterm;
    midterm.insert("sam", 50);
    midterm.insert("jim", 50);
    midterm.erase("sam");
    midterm.erase("jim");
    midterm.insert("hi", 99);
    midterm.insert("hit", 50);
    
    
    midterm.erase("hi");
    midterm.erase("hit");
    midterm.insert("crap", 88);
    assert(midterm["crap"] == 88);

}

int main() {

    // VERY BASIC EXAMPLE (will likely not compile with default/empty hashtable.h file)
    HashTable<std::string, int> midterm;
    std::cout << midterm.insert("sam", 50) << " ";
    assert(midterm.size() == 1);
    std::cout << midterm.insert("fee", 100) << " ";
    assert(midterm.size() == 2);
    std::cout << midterm.insert("milo", 95) << " ";
    assert(midterm.size() == 3);
    std::cout << midterm.insert("gabe", 88) << " \n";
    assert(midterm.size() == 4);
    std::cout << midterm["sam"] << " ";
    std::cout << midterm["fee"] << " ";
    std::cout << midterm["milo"] << " ";
    std::cout << midterm["gabe"] << " \n";
    std::cout << midterm.erase("sam") << " ";
    assert(midterm.size() == 3);
    std::cout << midterm["sam"] << "\n";
    // ADD MORE TESTS OF YOUR OWN
    
    test_inf_loop();

    return 0;
}
