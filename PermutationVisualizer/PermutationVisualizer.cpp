#include <iostream>
#include <cassert>
#include <iterator>     // std::ostream_iterator
#include <algorithm>    // std::copy

#include "GeneralAlgorithmsAndDataStructures.h"

namespace GA = GeneralAlgorithmsAndDataStructures;

// Send permutation to given output stream
template<class T>
void showPermutation(const T& permutation, std::ostream& outStream)
{
    auto iter = permutation.cbegin();
    while ( iter != permutation.cend() )
    {
        outStream << *iter++;
        if ( iter != permutation.cend() )
            outStream << ' ';
    }

    if ( !permutation.empty() ) // Some data was shown
        outStream << std::endl;
}

void showLexicographicPermutationsEmpty()
{
    GA::DataVct vct1;
    bool res = GA::nextIncreasingLexicographicPermutation(vct1);
    assert(!res);
    showPermutation(vct1,std::cout);
}

void showLexicographicPermutationsSingle()
{
    GA::DataVct vct1 = {1,2,3,4,5};
    bool res = GA::nextIncreasingLexicographicPermutation(vct1);
    assert(res);
    showPermutation(vct1,std::cout);
}

void showIncreasingLexicographicPermutations1()
{
    GA::DataVct vct1 = {1,2,3};
    showPermutation(vct1,std::cout);
    while (GA::nextIncreasingLexicographicPermutation(vct1))
        showPermutation(vct1,std::cout);
}

void showDecreasingLexicographicPermutations1()
{
    GA::DataVct vct1 = {3,2,1};
    showPermutation(vct1,std::cout);
    while (GA::nextDecreasingLexicographicPermutation(vct1))
        showPermutation(vct1,std::cout);
}

void showRCombinations()
{
    std::cout << "R Combinations" << std::endl;
    GA::IndexVct vct1 = { 0,1,2 };
    showPermutation(vct1, std::cout);
    while (GA::nextLexicographicRCombination(6,vct1))
        showPermutation(vct1, std::cout);
}

void showRCombinationsRecursive()
{
    std::cout << "R Combinations recursive" << std::endl;
    GA::DataVct vct1 = { 0,1,2,4 };
    GA::GeneratedSequences seq1 = GA::generateRCombinations(vct1, 3);
    for (auto& s: seq1)
        showPermutation(s, std::cout);
}


int main()
{
    showLexicographicPermutationsEmpty();
    showLexicographicPermutationsSingle();
    showIncreasingLexicographicPermutations1();
    std::cout << std::endl;
    showDecreasingLexicographicPermutations1();
    std::cout << std::endl;
    showRCombinations();
    showRCombinationsRecursive();
}
