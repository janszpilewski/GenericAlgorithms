#include <iostream>
#include <cassert>

#include "GeneralAlgorithmsAndDataStructures.h"

namespace GADS = GeneralAlgorithmsAndDataStructures;


// Basic tests

void testBinarySearch()
{
    GADS::GenericVector<int> vct = {1,2,3,4,5};

    // Valid elements
    auto pos = GADS::binSearch(vct,4);
    assert(pos == 3);

    pos = GADS::binSearch(vct,1);
    assert(pos == 0);

    pos = GADS::binSearch(vct,5);
    assert(pos == 4);

    
    // Nonexsitent elements
    pos = GADS::binSearch(vct,6);
    assert(pos >= vct.size());

    pos = GADS::binSearch(vct,0);
    assert(pos >= vct.size());

    pos = GADS::binSearch(vct,-1);
    assert(pos >= vct.size());

    // start > end
    pos = GADS::binSearch(vct, 6, 5, 3);
    assert(pos >= vct.size());
}

void testLowerBound()
{
    GADS::GenericVector<int> vct = { 1,2,4,5 };

    // Valid elements
    auto pos = GADS::lowerBound(vct, 4);
    assert(pos == 3);

    pos = GADS::lowerBound(vct, 1);
    assert(pos == 1);
    
    pos = GADS::lowerBound(vct, 2);
    assert(pos == 2);

    pos = GADS::lowerBound(vct, 5);
    assert(pos >= vct.size());

    pos = GADS::lowerBound(vct, 6);
    assert(pos >= vct.size());

    pos = GADS::lowerBound(vct, 0);
    assert(pos == 0);

    pos = GADS::lowerBound(vct, -1);
    assert(pos == 0);

    // start > end
    pos = GADS::lowerBound(vct, 6, 5, 3);
    assert(pos >= vct.size());

    GADS::GenericVector<int> vct2 = { 1,2,4,5,8 };
}

void testUpperBound()
{
    GADS::GenericVector<int> vct = { 1,2,4,5 };

    // Valid elements
    auto pos = GADS::upperBound(vct, 3);
    assert(pos == 1);

    pos = GADS::upperBound(vct, 2);
    assert(pos == 0);

    pos = GADS::upperBound(vct, 5);
    assert(pos == 3);

    pos = GADS::upperBound(vct, -1);
    assert(pos >= vct.size());

    pos = GADS::upperBound(vct, 0);
    assert(pos >= vct.size());

    pos = GADS::upperBound(vct, 6);
    assert(pos == 3);

    // start > end
    pos = GADS::upperBound(vct, 6, 5, 3);
    assert(pos >= vct.size());
}

void testPartitionLamuto()
{
    GADS::GenericVector<int> vct = { 2,0,8,7,4,1,6,3,9,5 };
    auto pivotVal = vct.back(); // 5 is pivot
    auto pivot = GADS::partitionLomuto<int> ( vct.begin(), vct.end() );

    // Check pivot value
    assert(*pivot == pivotVal);

    // Left range (elements smaller or equal)
    for (auto it = vct.cbegin(); it < pivot; ++it)
    {
        assert(*pivot >= *it);
    }

    // Right range (elements bigger)
    for (auto it = pivot+1; it < vct.cend(); ++it)
    {
        assert(*pivot < *it);
    }

    // Incorrect iterator order
    pivot = GADS::partitionLomuto<int>(vct.end(), vct.begin());
    assert(pivot == vct.begin());

    // Empty vector
    {
        GADS::GenericVector<int> vct = {};
        pivot = GADS::partitionLomuto<int>(vct.begin(), vct.end());
        assert(pivot == vct.end()); 
    }

    // One element
    {
        GADS::GenericVector<int> vct = {1};
        pivot = GADS::partitionLomuto<int>(vct.begin(), vct.end());
        assert(pivot == vct.begin());
    }

    // Custom pivot
    {
        GADS::GenericVector<int> vct = { 2,0,8,7,4,1,6,3,9,5 };
        auto pivot = vct.begin() + 2;
        auto pivotVal = *pivot;
        pivot = GADS::partitionLomuto<int>(vct.begin(), vct.end(), pivot);

        // Check pivot value
        assert(*pivot == pivotVal);

        // Incorrect iterator order
        pivot = GADS::partitionLomuto<int>(vct.end(), vct.begin(), pivot);
        assert(pivot == vct.begin());
    
    }
}

void testFindNthSmallestElement()
{
    bool exceptionHandled = false;
    GADS::GenericVector<int> vct;

    try
    {
        GADS::ValueType res = GADS::findNthSmallestElem<int>(vct.begin(), vct.end(), 1);
        (void)res;
    }
    catch (const std::out_of_range&)
    {
        exceptionHandled = true;
    }

    assert(exceptionHandled);

    GADS::GenericVector<int> vct2{ 1 };
    GADS::ValueType res2 = GADS::findNthSmallestElem<int>(vct2.begin(), vct2.end(), 1);
    assert(res2 == 1);

    GADS::GenericVector<int> vct3_init{ 2,0,8,7,4,1,6,3,9,5 };
    GADS::GenericVector<int> vct3 = vct3_init;
    GADS::ValueType res3 = GADS::findNthSmallestElem<int>(vct3.begin(), vct3.end(), 1);
    assert(res3 == 0);
    vct3 = vct3_init;
    GADS::ValueType res4 = GADS::findNthSmallestElem<int>(vct3.begin(), vct3.end(), 4);
    assert(res4 == 3);
    vct3 = vct3_init;
    GADS::ValueType res5 = GADS::findNthSmallestElem<int>(vct3.begin(), vct3.end(), 10);
    assert(res5 == 9);


}


void testQuickSort()
{
    // Without duplicates
    GADS::GenericVector<int> vct = {2,9,8,7,0,1,6,3,5,4};
    GADS::quickSortInPlace(vct);

    assert(std::is_sorted(vct.cbegin(), vct.cend()));
 
    // With duplicates
    GADS::GenericVector<int> vct2 = { 2,0,8,6,4,0,6,2,4,8 };
    GADS::quickSortInPlace(vct2);

    assert(std::is_sorted(vct2.cbegin(), vct2.cend()));

    for (size_t i = 0; i < vct2.size(); i++)
        assert(static_cast<size_t>(vct2[i]) == i - (i%2) );
}

void testMergeSort()
{
    // Without duplicates
    GADS::GenericVector<int> vct = { 2,9,8,7,0,1,6,3,5,4 };
    GADS::mergeSort(vct);

    assert(std::is_sorted(vct.cbegin(), vct.cend()));

    // With duplicates
    GADS::GenericVector<int> vct2 = { 2,0,8,6,4,0,6,2,4,8 };
    GADS::mergeSort(vct2);

    assert(std::is_sorted(vct2.cbegin(), vct2.cend()));

    for (size_t i = 0; i < vct2.size(); i++)
        assert(static_cast<size_t>(vct2[i]) == i - (i % 2));
}

void testReverseArray()
{
    GADS::DataVct vct1 = {1,2,3,4,5};

    // Reverse all elements
    GADS::reverseArray(vct1,0,4);

    for (size_t i = 0; i < vct1.size(); i++)
        assert(static_cast<size_t>(vct1[i]) == vct1.size() - i);

    // Reverse all elements back
    GADS::reverseArray(vct1,0,4);

    for (size_t i = 0; i < vct1.size(); i++)
        assert(static_cast<size_t>(vct1[i]) == i + 1);

    // Reverse a few elems
    GADS::reverseArray(vct1,1,vct1.size() - 2);

    assert(vct1.front() == 1);
    assert(vct1.back() == 5);

    for (size_t i = 1; i < vct1.size() - 1; i++)
        assert(static_cast<size_t>(vct1[i]) == vct1.size() - i);

    // Reverse single elem
    GADS::DataVct vct2 = {1};
    GADS::reverseArray(vct2);
    assert(vct2.front() == 1);

    // Reverse even elem set
    GADS::DataVct vct3 = {1,2,3,4};
    GADS::reverseArray(vct3);
    for (size_t i = 0; i < vct3.size(); i++)
        assert(static_cast<size_t>(vct3[i]) == vct3.size() - i);

}

void testLexicographicPermutations()
{
    GADS::DataVct vct1 = {1,2,3,4,5};
    bool res = GADS::nextIncreasingLexicographicPermutation(vct1);
    assert(res);
    assert(vct1[3] == 5);
    assert(vct1[4] == 4);

    GADS::DataVct vct2 = {3,2,1};
    res = GADS::nextIncreasingLexicographicPermutation(vct2);
    assert(!res);
    res = GADS::nextDecreasingLexicographicPermutation(vct2);
    assert(res);
    assert(vct2[2] == 2);
    assert(vct2[1] == 1);

}

void testLexicographicRCombinations()
{
    GADS::IndexVct vct1 = { 0,1,4,5 };

    bool res = GADS::nextLexicographicRCombination(5, vct1);
    assert(res);
    assert(vct1[0] == 0);
    assert(vct1[1] == 2);
    assert(vct1[2] == 3);
    assert(vct1[3] == 4);

    GADS::IndexVct vct2 = { 0,1 };
    res = GADS::nextLexicographicRCombination(1, vct2);
    assert(!res);

    GADS::IndexVct vct3;
    res = GADS::nextLexicographicRCombination(1, vct3);
    assert(!res);
}

void testGenerateRCombinations()
{
    GADS::DataVct vct1 = { 0,1,4,5 };

    GADS::GeneratedSequences seq1 = GADS::generateRCombinations(vct1,3);
    assert(seq1.size() == 4); // (n k)

    std::map<int, int> counter1;
    for (const auto& s : seq1)
    {
        assert(s.size() == 3); // k
        for (auto v : s)
            counter1[v]++;
    }

    for (auto&& p : counter1)
        assert(p.second == 3); // (n k) - (n-1 k)
 
    GADS::DataVct vct2 = { 0,1 };
    GADS::GeneratedSequences seq2 = GADS::generateRCombinations(vct2,2);
    assert(seq2.size() == 1);  // (n k)
    for (const auto& s : seq2)
    {
        assert(s.size() == 2);
    }

    std::map<int, int> counter2;
    for (const auto& s : seq2)
    {
        assert(s.size() == 2); // k
        for (auto v : s)
            counter2[v]++;
    }

    for (auto&& p : counter2)
        assert(p.second == 1);  // (n k) - (n-1 k)

    GADS::DataVct vct3;
    GADS::GeneratedSequences seq3 = GADS::generateRCombinations(vct3,1);
     assert(seq3.size() == 0);
 
}

void testGenerateSubsets()
{
    GADS::DataVct vct1 = { 0,1,4 };

    GADS::DataVct expectedSubsets[] = { {0}, {1}, {0,1}, {4}, {0,4}, {1,4}, {0,1,4}, {} };

    GADS::SubsetGenerator subGen(vct1);

    for (const auto& expectedSubset : expectedSubsets)
    {
        GADS::DataVct sub = subGen.nextSubset();
        assert(sub == expectedSubset);
    }

    GADS::DataVct vct2 = {};
    GADS::SubsetGenerator subGen2(vct2);
    GADS::DataVct sub2 = subGen2.nextSubset();
    assert(sub2.size() == 0);
}


void testModularExp()
{
    unsigned long val = GADS::modularExp(2, 4, 10);
    assert(val == 6);

    val = GADS::modularExp(3, 644, 645);
    assert(val == 36);

    val = GADS::modularExp(-1, 0, 1);
    assert(val == 1);

    val = GADS::modularExp(2, 5, 10);
    assert(val == 2);


    // Handling incorrect input
    bool exceptionHandled = false;
    try
    {
        GADS::modularExp(1, 1, 0);
    }
    catch (const std::invalid_argument&)
    {
        exceptionHandled = true;
    }
    assert(exceptionHandled);
}

void testLongestCommonSubsequence()
{
    const std::string s1 = "anaconda";
    const std::string s2 = "ala";
    const std::string s3 = "anagram";
    const std::string s4;
    const std::string s5 = "aab";
    const std::string s6 = "aaa";
    const std::string s7 = "abb";
    const std::string s8 = "bb";

    GADS::IndexType seqLen = GADS::longestCommonSubsequence(s1,s2);
    assert(seqLen == 2);

    GADS::GenericSet < std::string > sequences;

    seqLen = GADS::longestCommonSubsequence(s1,s3,&sequences);
    assert(seqLen == 4);
    assert(sequences.size() == 1);
    assert(sequences.count("anaa") == 1);

    seqLen = GADS::longestCommonSubsequence(s3,s1,&sequences);
    assert(seqLen == 4);
    assert(sequences.size() == 1);
    assert(sequences.count("anaa") == 1);

    sequences.clear();
    seqLen = GADS::longestCommonSubsequence(s1,s4,&sequences);
    assert(seqLen == 0);
    assert(sequences.size() == 0);

    sequences.clear();
    seqLen = GADS::longestCommonSubsequence(s1,s8,&sequences);
    assert(seqLen == 0);
    assert(sequences.size() == 0);

    seqLen = GADS::longestCommonSubsequence(s5,s6);
    assert(seqLen == 2);
    seqLen = GADS::longestCommonSubsequence(s5,s7);
    assert(seqLen == 2);

    sequences.clear();
    seqLen = GADS::longestCommonSubsequence(s1,s6,&sequences);
    assert(seqLen == 3);
    assert(sequences.size() == 1);
    assert(sequences.count("aaa") == 1);

}

void testApproximativeStringMatching()
{
    const std::string s1 = "kola";
    const std::string s2 = "koala";
    const std::string s3 = "a";
    GADS::MatchConfigGenericImpl matchConfig;

    GADS::IndexType apprCost = GADS::approximativeStringMatching(s1,s2,matchConfig);
    assert(apprCost == 1);

    apprCost = GADS::approximativeStringMatching(s1,s1,matchConfig);
    assert(apprCost == 0);

    apprCost = GADS::approximativeStringMatching(s2,s1,matchConfig);
    assert(apprCost == 1);

    apprCost = GADS::approximativeStringMatching(s1,s3,matchConfig);
    assert(apprCost == 3);

}

void testPartitionRange()
{
    {
        GADS::DataVct data;
        GADS::IndexType dividers = 0;
        GADS::IndexSeq result = GADS::partitionRange(data, dividers);
        assert(result.size() == 0);
    }

    {
        GADS::DataVct data = { 1, 1 };
        GADS::IndexType dividers = 0;
        GADS::IndexSeq result = GADS::partitionRange(data, dividers);
        assert(result.size() == 0);
    }

    {
        GADS::DataVct data = { 1, 1 };
        GADS::IndexType dividers = 1;
        GADS::IndexSeq result = GADS::partitionRange(data, dividers);
        assert(result.size() == 1);
        const GADS::IndexSeq expectedResult = { 0 };
        assert(result == expectedResult);
    }

    {
        GADS::DataVct data = { 1,1,1,1,1 };
        GADS::IndexType dividers = 4;
        GADS::IndexSeq result = GADS::partitionRange(data, dividers);
        assert(result.size() == 4);
        const GADS::IndexSeq expectedResult = { 0, 1, 2, 3 };
        assert(result == expectedResult);
    }

    {
        GADS::DataVct data = { 1,1,4,1,1,1 };
        GADS::IndexType dividers = 2;
        GADS::IndexSeq result = GADS::partitionRange(data, dividers);
        assert(result.size() == 2);
        const GADS::IndexSeq expectedResult = { 1, 2 };
        assert(result == expectedResult);
    }

    {
        GADS::DataVct data = { 1,1,4,1,1,4 };
        GADS::IndexType dividers = 3;
        GADS::IndexSeq result = GADS::partitionRange(data, dividers);
        assert(result.size() == 3);
        const GADS::IndexSeq expectedResult = { 1, 2, 4 };
        assert(result == expectedResult);
    }

    {
        GADS::DataVct data = { 1,1,4,1,1,4 };
        GADS::IndexType dividers = 5;
        GADS::IndexSeq result = GADS::partitionRange(data, dividers);
        assert(result.size() == 4);
        const GADS::IndexSeq expectedResult = { 0, 1, 2, 4 };
        assert(result == expectedResult);
    }

    {
        GADS::DataVct data = { 1,1,1,1 };
        GADS::IndexType dividers = 5;
        GADS::IndexSeq result = GADS::partitionRange(data, dividers);
        assert(result.size() == 3);
        const GADS::IndexSeq expectedResult = { 0, 1, 2 };
        assert(result == expectedResult);
    }

    {
        GADS::DataVct data = { 1,1 };
        GADS::IndexType dividers = 5;
        GADS::IndexSeq result = GADS::partitionRange(data, dividers);
        assert(result.size() == 1);
        const GADS::IndexSeq expectedResult = { 0 };
        assert(result == expectedResult);
    }

    {
        GADS::DataVct data = { 1 };
        GADS::IndexType dividers = 5;
        GADS::IndexSeq result = GADS::partitionRange(data, dividers);
        assert(result.size() == 0);
    }
}



int main()
{
    testBinarySearch();
    testLowerBound();
    testUpperBound();
    testPartitionLamuto();
    testFindNthSmallestElement();
    testQuickSort();
    testMergeSort();
    testReverseArray();
    testLexicographicPermutations();
    testLexicographicRCombinations();
    testGenerateRCombinations();
    testGenerateSubsets();
    testModularExp();
    testLongestCommonSubsequence();
    testApproximativeStringMatching();
    testPartitionRange();

    std::cout << "--- All basic tests executed ---" << std::endl;
}
