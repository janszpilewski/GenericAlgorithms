#include <iostream>
#include <cassert>

#include "GeneralAlgorithmsAndDataStructures.h"
#include "SuffixArray.h"

namespace GADS = GeneralAlgorithmsAndDataStructures;

void testBuildSuffixArray()
{
    const std::string str1 = "abac";
    const GADS::SuffixArray sa1(str1);
    const GADS::IndexVct& res1 = sa1.get();
    const GADS::IndexVct expectedResult1 = { 0, 2, 1, 3 };
    assert(res1 == expectedResult1);

    const GADS::IndexVct res2 = GADS::SuffixArray("bobocel").get();
    const GADS::IndexVct expectedResult2 = { 0,2,4,5,6,1,3 };
    assert(res2 == expectedResult2);

    const GADS::SuffixArray sa3("bOBocEl");
    const GADS::IndexVct& res3 = sa3.get();
    const GADS::IndexVct expectedResult3 = { 2,5,1,0,4,6,3 };
    assert(res3 == expectedResult3);
}

void testGetNthSuffix()
{
    const GADS::SuffixArray sa3("bOBocEl");
    const std::string suf1 = sa3.getNthSuffix(3);
    assert(suf1 == "OBocEl");

    // Handling incorrect input
    bool exceptionHandled = false;
    try
    {
        const std::string suf2 = sa3.getNthSuffix(10);
        (void)suf2; // Supress compiler warning
    }
    catch (const std::out_of_range&)
    {
        exceptionHandled = true;
    }
    assert(exceptionHandled);
}

void testLcp()
{
    const GADS::SuffixArray sa4("lalala");
    GADS::IndexType len1 = sa4.lcp(0, 2);
    assert(len1 == 4);
    GADS::IndexType len2 = sa4.lcp(0, 4);
    assert(len2 == 2);
    GADS::IndexType len3 = sa4.lcp(1, 3);
    assert(len3 == 3);
    GADS::IndexType len4 = sa4.lcp(1, 5);
    assert(len4 == 1);

    // Handling incorrect input
    bool exceptionHandled = false;
    try
    {
        GADS::IndexType len5 = sa4.lcp(7, 0);
        (void)len5; // Supress compiler warning
    }
    catch (const std::out_of_range&)
    {
        exceptionHandled = true;
    }
    assert(exceptionHandled);
}

void testSearchSubstring()
{
    const GADS::SuffixArray sa3("bOBocEl");
    
    const GADS::IndexType res4 = sa3.searchSubstring("bOB");
    assert(res4 == 3);

    const GADS::IndexType res5 = sa3.searchSubstring("Boc");
    assert(res5 == 0);

    const GADS::IndexType res6 = sa3.searchSubstring("BocEl");
    assert(res6 == 0);

    const GADS::IndexType res7 = sa3.searchSubstring("BocEla");
    assert(!sa3.indexValid(res7));

    const GADS::IndexType res8 = sa3.searchSubstring("X");
    assert(!sa3.indexValid(res8));

    // Handling incorrect input
    bool exceptionHandled = false;
    try
    {
        const GADS::IndexType res9 = sa3.searchSubstring("");
        (void)res9; // Supress compiler warning
    }
    catch (const std::out_of_range&)
    {
        exceptionHandled = true;
    }
    assert(exceptionHandled);
}

void testMiinLexRotation()
{
    const GADS::SuffixArray sa3("bOBocEl");
    const GADS::SuffixArray sa5("");
    const GADS::IndexVct& res9 = sa5.get();
    const GADS::IndexVct expectedResult5{};
    assert(res9 == expectedResult5);

    std::string minRotation = sa3.minLexRotation();
    assert(minRotation == "BocElbO");

    const GADS::SuffixArray sa6("alabala");
    std::string minRotation2 = sa6.minLexRotation();
    assert(minRotation2 == "aalabal");
}

int main()
{
    testBuildSuffixArray();
    testGetNthSuffix();
    testLcp();
    testSearchSubstring();
    testMiinLexRotation();

    std::cout << "--- All suffix array tests executed ---" << std::endl;
}