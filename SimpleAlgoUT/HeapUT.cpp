#include <iostream>
#include <cassert>

#include "GeneralAlgorithmsAndDataStructures.h"
#include "Heap.h"

namespace GADS = GeneralAlgorithmsAndDataStructures;

struct ClassData
{
    ClassData(int v) : value(v) {}
    ClassData(const ClassData& other) : value(other.value) {}
    ClassData& operator = (const ClassData& other)
    {
        value = other.value;
        return *this;
    }

    ClassData(ClassData&& other) noexcept : value(std::move(other.value)) {}
    ClassData& operator = (ClassData&& other) noexcept
    {
        value = std::move(other.value);
        return *this;
    }

    ~ClassData() {}

    int value = 0;
};

bool operator < (const ClassData& lhs, const ClassData& rhs) { return lhs.value < rhs.value; }
bool operator == (const ClassData& lhs, const ClassData& rhs) { return lhs.value == rhs.value; }


void testHeapBasic()
{
    GADS::GenericVector<int> vct = { 2,8,7,4,1,6 };

    // Initialize from vector as lvalue
    GADS::MinHeap<int> minHeap(vct);
    assert(vct[0] == 2);

    auto v = minHeap.extractTop();
    assert(v == 1);

    v = minHeap.extractTop();
    assert(v == 2);

    // Initialize from vector as lvalue
    GADS::MaxHeap<int> maxHeap(vct);
    v = maxHeap.extractTop();
    assert(v == 8);

    v = maxHeap.extractTop();
    assert(v == 7);

    // initializer list as lvalue
    auto vct2 = { 3,2,1 };

    // Initialize from temporary built from initializer list as lvalue
    GADS::MinHeap<int> minHeap2(vct2);
    v = minHeap2.extractTop();
    assert(v == 1);

    // Initialize from moved initializer list
    GADS::MinHeap<int> minHeap21(std::move(vct2));
    v = minHeap21.extractTop();
    assert(v == 1);

    // Build from empty heap
    GADS::MinHeap<int> minHeap3;
    minHeap3.insert(4);
    minHeap3.insert(1);
    v = minHeap3.extractTop();
    assert(v == 1);

    // deleting items
    assert(minHeap3.size() == 1);
    minHeap3.deleteItem(4);
    assert(minHeap3.size() == 0);
    minHeap3.insert(4);
    minHeap3.insert(1);
    assert(minHeap3.size() == 2);
    minHeap3.deleteItem(4);
    assert(minHeap3.size() == 1);
    minHeap3.deleteItem(1);
    assert(minHeap3.size() == 0);


    // Return generic vector as rvalue
    auto makeVct = []() { return GADS::GenericVector<int> {3, 2, 1}; };

    // Initialization with rvalue
    GADS::MaxHeap<int> maxHeap3 = makeVct();
    v = maxHeap3.extractTop();
    assert(v == 3);

    // Initialization with Heap copy
    auto maxHeap4 = maxHeap3;
    v = maxHeap4.extractTop();
    assert(v == 2);

    // Initialization with initializer list as rvalue
    GADS::MaxHeap<int> maxHeap5 = { 4,2,1 };
    v = maxHeap5.extractTop();
    assert(v == 4);
}


void testHeapObjects()
{
    GADS::GenericVector<ClassData> vct = { 2,8,7,4,1,6 };
    GADS::MinHeap<ClassData> minHeap(vct);
    minHeap.deleteItem(1);
    auto v = minHeap.extractTop();
    assert(v == 2);
    ClassData cd{ 1 }; // pass lvalue
    minHeap.insert(cd);
    v = minHeap.extractTop();
    assert(v == 1);
}

void testHeapSorter()
{
    GADS::GenericVector<int> vct = { 2,8,7,4,1,6 };

    // Initialize from vector as lvalue
    GADS::HeapSorter<int> sorter(vct);
    sorter.sort();
    assert(std::is_sorted(vct.cbegin(), vct.cend()));
}

void testHeapSort()
{
    GADS::GenericVector<int> vct = { 2,8,7,4,1,6 };

    // Initialize from vector as lvalue
    GADS::heapsort<int>(vct);
    assert(std::is_sorted(vct.cbegin(), vct.cend()));
}

int main()
{
    testHeapBasic();
    testHeapObjects();
    testHeapSorter();
    testHeapSort();

    std::cout << "--- All binary heap tests executed ---" << std::endl;
}
