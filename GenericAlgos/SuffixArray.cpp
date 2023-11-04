#include <iostream>
#include <numeric>
#include <limits>
#include <stdexcept>

#include "GeneralAlgorithmsAndDataStructures.h"
#include "SuffixArray.h"

namespace GADS = GeneralAlgorithmsAndDataStructures;

namespace // helpers for buildSuffixMatrix()
{

    struct  SortEntry
    {
        // Helper for sorting using precalculated block order
        bool isSmallerOrEqual(const SortEntry& b) const
        {
            if (chunk[0] == b.chunk[0])
                return chunk[1] <= b.chunk[1];
            else
                return chunk[0] < b.chunk[0];
        }

        GADS::IndexType chunk[2]; // Order of first and second chunk of size 2^k-1 calculated in previous step
        GADS::IndexType pos; // Reference to the starting position of prefix
    };


    inline bool operator <= (const SortEntry& a, const SortEntry& b)
    {
        return a.isSmallerOrEqual(b);
    }

} // End of anonymous namespace

// Algorithm based on maintaining the order of the string�s suffixes sorted by their 2^k long prefixes
void GADS::SuffixArray::buildSuffixArray()
{
    const IndexType N = m_text.size(); // Size of input text

    // Add first line containing string characters, with their int code equivalent to order of cnt = 1
    m_prefixOrderMatrix.emplace_back(IndexVct(N));
    std::copy(std::begin(m_text), std::end(m_text), std::begin(m_prefixOrderMatrix.back()));

    GenericVector<SortEntry> prefixes(N); // Helper vector for sorting prefixes

    for (IndexType cnt = 1; cnt < N; cnt <<= 1) // Performing log ceil(N) steps to sort prefixes of size up to N (longest suffix)
    {
        const IndexVct& lastOrder = m_prefixOrderMatrix.back();

        // Setting up data for sorting, building prefixes of size cnt * 2 by composing 2 prefixes of size cnt
        for (IndexType i = 0; i < N; i++)
        {
            prefixes[i].chunk[0] = lastOrder[i]; // First chunk of the prefix

            if (i + cnt < N)
                prefixes[i].chunk[1] = lastOrder[i + cnt]; // Second chunk of the prefix
            else
                prefixes[i].chunk[1] = 0; // End of prefix out of range, use null character, lexicographically before all other ones

            prefixes[i].pos = i; // Original prefix position before sorting 
        }

        quickSortInPlace(prefixes);

        m_prefixOrderMatrix.emplace_back(IndexVct(N)); // Add another line to matrix filled with sorted prefixes
        GADS::IndexVct& newOrder = m_prefixOrderMatrix.back();

        // Setting new line of sequences of length 2^k
        // Assign sorted order of prefixes 2^k to related suffixes 
        for (IndexType i = 0; i < N; i++)
        {
            if ( // If the prefix is the same compared to the previous one, use previous sorted order
                (i > 0)
                && (prefixes[i].chunk[0] == prefixes[i - 1].chunk[0])
                && (prefixes[i].chunk[1] == prefixes[i - 1].chunk[1])
                )
                newOrder[prefixes[i].pos] = newOrder[prefixes[i - 1].pos];
            else
                newOrder[prefixes[i].pos] = i;
        }

    }

    // Fill array of starting indexes of suffixes in lexicographically sorted order.
    for (IndexType i = 0; i < N; i++)
        m_suffixArray[i] = prefixes[i].pos;
}

// Finds length of longest common prefix
GADS::IndexType GADS::SuffixArray::lcp(IndexType x, IndexType y) const
{
    const IndexType N = m_text.size(); // Size of input text

    if ((x >= N) || (y >= N))
        throw std::out_of_range("Input data out of range");

    if (x == y) return N - x;

    IndexType len = 0;

    for (IndexType k = m_prefixOrderMatrix.size() - 1; k >= 0 && x < N && y < N; k--)
    {
        if (m_prefixOrderMatrix[k][x] == m_prefixOrderMatrix[k][y])
        { // Match found for a sequence 2^k, check also possible tails of len 2^k-1, 2^k-2 etc.
            IndexType foundPartLen = 1llu << k;
            x += foundPartLen;
            y += foundPartLen;
            len += foundPartLen;
        }

        if (k == 0) // Do not decrement unsigned k in loop
            break;
    }
    return len;
}

// Checks if text contains a substring in time log size(text) * size(subsr)
GADS::IndexType GADS::SuffixArray::searchSubstring(const std::string& substr) const
{
    if (substr.size() == 0)
        throw std::out_of_range("Substring is empty");


    IndexType res = GADS::binSearch(m_suffixArray, substr,
        0, m_suffixArray.size() - 1,
        [this](IndexType idx, const std::string& substr)
        {
            return this->isLexEqual(idx, substr);
        },
        [this](IndexType idx, const std::string& substr)
        {
            return this->isLexSmaller(idx, substr);
        }
        );

    return res;
}

// Helper checks if a prefix of the suffix starting from index idx is equal to given substring 
// Result = 0 substr is contained
// Result = 1 suffix is lexicographically smaller
// Result = 2 suffix is lexicographically bigger
GADS::SuffixArray::CompareResult
GADS::SuffixArray::compareSubstring(IndexType idx, const std::string& substr) const
{
    if (idx >= outOfRangeVal())
        throw std::out_of_range("Input data out of range");

    if (substr.size() == 0)
        throw std::out_of_range("Substring is empty");

    IndexType len = std::min(m_text.size() - idx, substr.size());

    CompareResult result = CompareResult::Undefined; // Initial substrings of len 0, result is match

    // Verify every character in substring
    for (IndexType i = 0; i < len; i++)
    {
        if (m_text[idx + i] != substr[i])
        {
            if (m_text[idx + i] < substr[i])
                result = CompareResult::Smaller;
            else
                result = CompareResult::Bigger;
            break;
        }
    }

    if (result == CompareResult::Undefined) // Substring is matching suffix of size len
    {
        if (m_text.size() - idx < substr.size()) // Suffix too short to contain entire substr
            result = CompareResult::Smaller;
        else
            result = CompareResult::Equal; // substr is prefix of given suffix
    }

    return result;
}

// Returns minimal lexicographic rotation of the initial text
std::string GADS::SuffixArray::minLexRotation() const
{
    // Get the lexicographically smallest suffix idx
    const GADS::IndexType minLexSuffixIdx = m_suffixArray[0];

    // Get the lexicographically smallest suffix
    std::string rotation = m_text.substr(minLexSuffixIdx);

    // Complete it with its prefix if needed
    if (m_suffixArray[0] > 0)
    {
        rotation += m_text.substr(0, minLexSuffixIdx);
    }

    return rotation;
}

