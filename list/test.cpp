#include <iostream>

#include "list.h"
#include <string>
#include <cassert>

const int ELEMENTS_COUNT = 100;

struct TestStruct {
    std::string key;
    std::string value1;
    std::string value2;
    TestStruct& operator=(TestStruct& src) {
        if (this == &src) {
            return *this;
        }
        key = src.key;
        value1 = src.value1;
        value2 = src.value2;
        return *this;
    }
};

typedef lab618::CSingleLinkedList<TestStruct> TestSingleList;
typedef lab618::CDualLinkedList<TestStruct> TestList;

static std::string makeRandomString(int minL = 1, int maxL = 10) {
    const char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const int alphabet_length = sizeof(alphabet);
    int length = rand() % maxL + minL;
    std::string s;
    s.reserve(length);
    for (int i = 0; i < length; ++i) {
        char temp = alphabet[rand() % (alphabet_length)];
        s += temp;
    }
    return s;
}

static void generate(TestStruct *pts) {
    pts->key = makeRandomString();
    pts->value1 = makeRandomString();
    pts->value2 = makeRandomString();
}

void TestListFunction() {
    // тест односвязного списка

    TestSingleList single_list;
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
        TestStruct ts;
        generate(&ts);
        single_list.pushBack(ts);
    }

    assert(single_list.getSize() == ELEMENTS_COUNT);
    
    for (TestSingleList::CIterator it = single_list.begin(); it.isValid(); ++it) {
        it.getLeaf();
        TestStruct ts = *it;
        try {
            single_list.erase(it);
        }
        catch (...) {
            std::cout << "List is empty!";
        }
    }
    
    assert(single_list.getSize() == 0);

    /// тест двусвязного списка

    TestList list;
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
        TestStruct ts;
        generate(&ts);
        list.pushBack(ts);
    }
    
    assert(list.getSize() == ELEMENTS_COUNT);
    
    for (TestList::CIterator it = list.begin(); it.isValid(); ++it) {
        it.getLeaf();
        TestStruct ts = *it;
        list.erase(it);
    }

    assert(list.getSize() == 0);
}

int main() {
    TestListFunction();
    return 0;
}