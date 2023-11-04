#include <iostream>
#include <numeric>
#include <limits>
#include <stdexcept>

#include "GeneralAlgorithmsAndDataStructures.h"

namespace GADS = GeneralAlgorithmsAndDataStructures;

// Reverses array
void GADS::reverseArray(DataVct& array, IndexType startIdx, IndexType endIdx)
{
    const IndexType halfRangeCount = (startIdx + endIdx + 1) / 2; // Mid exclusive
    for (IndexType i = startIdx; i < halfRangeCount; i++)
        std::swap(array[i], array[endIdx - i + startIdx]);
}

// Finds result of operation: base^exp % mod
unsigned long GADS::modularExp(int base, unsigned int exp, unsigned int mod)
{
    if (mod==0)
        throw std::invalid_argument("0 is not legal mod value");
    
    constexpr unsigned char ByteSize = 8;
    constexpr unsigned char LeastSignificantBitMask = 0x00000001;

    unsigned long long x = 1;
    unsigned long long power = base % mod;

    for (IndexType i = 0; i < sizeof(int) * ByteSize; i++) {
        unsigned char leastSignificantBit = LeastSignificantBitMask & (exp >> i);
        if (leastSignificantBit)
            x = (x * power) % mod;
        power = (power * power) % mod;
    }

    return (unsigned long) x;
}

namespace
{

// Finds next lexicographic permutation following given one in DataVct using given comparer functor
// Comparer is a binary predicate and returns true if first elem is following the second in the requested order
// Returns false if no permutation can be generated, true if succeeded
template <typename Comparer>
bool nextLexicographicPermutation(GADS::DataVct& permutation, Comparer compare)
{
    if (permutation.size() < 2) // It is not possible to permute less than 2 elements
        return false;

    const GADS::IndexType BiggestIndex = permutation.size() - 1;

    // Find last pair in increasing order
    GADS::IndexType lastPair = BiggestIndex - 1;
    for (; compare(permutation[lastPair], permutation[lastPair + 1]); lastPair--)
    {
        assert(lastPair >= 0);
        if (lastPair == 0)
            return false; // There is no such a pair hence no next permutation
    }

    // Find smallest value bigger than permutation[lastPair]
    GADS::IndexType smallestBiggerValueIdx = BiggestIndex;
    while (compare(permutation[lastPair], permutation[smallestBiggerValueIdx]))
    {
        // With a pair in order found this may fail only due to a programmimg error
        smallestBiggerValueIdx--;
        assert(smallestBiggerValueIdx > lastPair);
    }

    // Swap those values
    std::swap(permutation[lastPair], permutation[smallestBiggerValueIdx]);

    // Reverse trailing values to put them in lexicographic order
    GADS::IndexType lastPairSecondElem = lastPair + 1;
    GADS::reverseArray(permutation, lastPairSecondElem, BiggestIndex);

    return true;
}

}

// Finds next increasing lexicographic permutation in DataVct using std::greater_equal comparer functor
bool  GADS::nextIncreasingLexicographicPermutation(DataVct& permutation)
{
    return nextLexicographicPermutation(permutation, std::greater_equal<ValueType>());
}

// Finds next decreasing lexicographic permutation in DataVct using std::less_equal comparer functor
bool  GADS::nextDecreasingLexicographicPermutation(DataVct& permutation)
{
    return nextLexicographicPermutation(permutation, std::less_equal<ValueType>());
}

// Generatates next r combination in lexicographic order from range [0,maxVal]
// Returns false if next combination cannot be calculated
bool GADS::nextLexicographicRCombination(IndexType maxVal, IndexVct& rCombination)
{
    if (rCombination.empty())
        return false;
   
    IndexType r = rCombination.size() - 1;

    if (maxVal <= r)
        return false;

    IndexType i = r;
    for (; rCombination[i] == maxVal - r + i; i--)
    {
        if (i == 0)
            return false; // No more combinations can be generated
    }

    rCombination[i]++;
    for (IndexType j = i + 1; j <= r; j++)
        rCombination[j] = rCombination[i] + j - i;

    return true;
}

namespace 
{
    
// Generates combinations with r elements
GADS::GeneratedSequences generateRCombinationsRecursive(const GADS::DataVct& inputSet, GADS::IndexType startingIndex, GADS::IndexType r)
{
    assert(r <= inputSet.size());
    
    if (r == 0)
    {
        // Create a sequence with an empty combination
        GADS::GeneratedSequences seq; // Sequence of combinations
        seq.emplace_front(GADS::GeneratedSequences::value_type()); // Insert empty combination
        return seq;
    }
    else
    {
        // Every element from input set is either present or not in generated sequences
        
        // Sequences with startingIndex
        GADS::GeneratedSequences seq = generateRCombinationsRecursive(inputSet, startingIndex + 1, r-1);
        for (auto& s : seq) // Add leading inputSet[startingIndex] value to sequences of size r-1
            s.push_front(inputSet[startingIndex]);
            
        // Sequences without startingIndex
        if (inputSet.size() - startingIndex > r) // There is enough of elements to generate sequences of length r
        {
            GADS::GeneratedSequences seq2 = generateRCombinationsRecursive(inputSet, startingIndex + 1, r);
            seq.splice(seq.end(), seq2);
        }

        return seq;
    }
}
}


GADS::GeneratedSequences GADS::generateRCombinations(const DataVct& inputSet, IndexType r)
{
    if (r > inputSet.size())
        return GeneratedSequences();
    else
        return generateRCombinationsRecursive(inputSet, 0, r);
}

// Returns next subset
GADS::DataVct GADS::SubsetGenerator::nextSubset()
{
    nextSubsetMask();
    
    GADS::IndexType resultSize = std::count(m_subsetMask.cbegin(), m_subsetMask.cend(), true);
    
    GADS::DataVct result;
    result.reserve(resultSize);

    for (GADS::IndexType i = 0; i < m_subsetMask.size(); ++i)
    {
        if (m_subsetMask[i])
            result.emplace_back(m_totalSet[i]);
    }

    assert (result.size() == resultSize);

    return result;
}

void GADS::SubsetGenerator::nextSubsetMask()
{
    // Simulates adding 1 to the mask
    for (auto it = m_subsetMask.begin(); it < m_subsetMask.end(); ++it)
    {
        if ((*it = !*it)) // Flip bits, break when 1 is set
            break;
    }
}


namespace // longestCommonSubsequence related
{

typedef GADS::GenericVector<GADS::IndexType> Row;
typedef GADS::GenericVector <Row> Table;

typedef GADS::GenericSet <std::string> Sequences;


// Recovers the longest subsequence from the tracking table
Sequences backtrackAllLongestSubsequences(const Table& t, const std::string& s1, const std::string& s2, GADS::IndexType i, GADS::IndexType j, bool matchingCharsFound = false)
{
    Sequences result;

    if ((i == 0) || (j == 0))
    {
        if (matchingCharsFound)
            result.insert(std::string());
    }
    else
    {
        if (s1[i - 1] == s2[j - 1]) // Matching characters
        {
            const Sequences& prefixes = backtrackAllLongestSubsequences(t, s1, s2, i - 1, j - 1, true);

            for (Sequences::iterator it = prefixes.begin(); it != prefixes.end(); ++it)
            {
                result.insert (*it + s1[i - 1]); // Append matching character to collected prefix strings
            }
        }
        
        else
        {
            if (t[i][j - 1] >= t[i - 1][j]) // Insert element [i, j - 1]
            {
                const Sequences& prefixes = backtrackAllLongestSubsequences(t, s1, s2, i, j - 1, matchingCharsFound);
                result.insert(prefixes.begin(), prefixes.end());
            }

            if (t[i - 1][j] >= t[i][j - 1]) // Insert element [i - 1, j]
            {
                const Sequences& prefixes = backtrackAllLongestSubsequences(t, s1, s2, i - 1, j, matchingCharsFound);
                result.insert(prefixes.begin(), prefixes.end());
            }
        }
    }

    return result;
}

}

// Finds the length 'and the sequences if container provided) of the longeset common subsequences of strings s1 and s2
GADS::IndexType GADS::longestCommonSubsequence(const std::string& s1, const std::string& s2, GenericSet < std::string >* sequences)
{

    // Input check
    if (( s1.length() == 0) || ( s2.length() == 0))
        return 0;

    // Table dimensions
    GADS::IndexType s1Length = s1.length() + 1; // Empty 0th element
    GADS::IndexType s2Length = s2.length() + 1; // Empty 0th element

    // Table initialization
    Table t(s1Length);
    for (auto& row: t)
        row.resize(s2Length);
 
    // All table elements are filled with 0

    // Main loop
    for (GADS::IndexType i = 1; i < s1Length; i++) // For every char in s1
    {
        for (GADS::IndexType j = 1; j < s2Length; j++) // Compare with a char in s2
        {
            if (s1[i - 1] == s2[j - 1]) // Matching case, char in s2 already was present in s1
                t[i][j] = t[i - 1][j - 1] + 1; // Matching score at given position is increased
            else
                t[i][j] = std::max(t[i][j - 1], t[i - 1][j]); // Matching score at given position is copied from the last highest score
        }
    }

    GADS::IndexType longestCommonSubsequenceLength = t[s1Length - 1][s2Length - 1];

    if ( (sequences != nullptr) && (longestCommonSubsequenceLength > 0) ) // Store discovered subsequences
    {
        *sequences = backtrackAllLongestSubsequences(t, s1, s2, s1Length - 1, s2Length - 1);
    }

    return longestCommonSubsequenceLength;

}

// Implementation of MatchConfigGenericImpl


// Creates and inits table of matching results
Table GADS::MatchConfigGenericImpl::createTable(GADS::IndexType tableD1, GADS::IndexType tableD2) const
{
    Table t(tableD1);

    for (GADS::IndexType i = 0; i < tableD1; i++)
    {
        t[i].resize(tableD2);

        if (i==0)
            std::iota(t[0].begin(), t[0].end(), 0);
        else
        {
            t[i].front() = i;
            std::fill(t[i].begin() + 1, t[i].end(), std::numeric_limits<IndexType>::max());
        }

    }

    return t;
}

// Cost of matching 2 characters
GADS::ValueType GADS::MatchConfigGenericImpl::matchCost(char c1, char c2) const
{
    ValueType result = 1;

    if (c1==c2)
        result = 0;

    return result;
}

// Cost of inserting given character
GADS::ValueType GADS::MatchConfigGenericImpl::insertCost(char c) const
{
    return 1;
}

// Cost of deleting given character
GADS::ValueType GADS::MatchConfigGenericImpl::deleteCost(char c) const
{
    return 1;
}

// Find the final result
GADS::IndexType GADS::MatchConfigGenericImpl::getResult(const Table& t) const
{
    return t.back().back();
}



// Finds best match of strings s1 and s2 parametrized by object MatchConfig
// If container was passed as a parameter it will contain matching subsequence or an empty string if there is no one
GADS::IndexType GADS::approximativeStringMatching(const std::string& s1, const std::string& s2,
    const MatchConfig& matchConfig, GenericSet < std::string >* sequences)
{
    // Table dimensions
    GADS::IndexType s1Length = s1.length() + 1; // Empty 0th element
    GADS::IndexType s2Length = s2.length() + 1; // Empty 0th element

    MatchConfig::Table t = matchConfig.createTable(s1Length, s2Length);

    // Main loop
    for (GADS::IndexType i = 1; i < s1Length; i++) // For every char in s1
    {
        for (GADS::IndexType j = 1; j < s2Length; j++) // Compare with a char in s2
        {
            // Getting costs of all operations in current context
            GADS::GenericVector<IndexType> operCost;
            operCost.reserve(matchConfig.CompareOperCount);
            operCost.push_back(t[i - 1][j - 1] + matchConfig.matchCost(s1[i - 1], s2[j - 1])); // matching
            operCost.push_back(t[i][j - 1] + matchConfig.insertCost(s1[i - 1])); // inserting
            operCost.push_back(t[i - 1][j] + matchConfig.deleteCost(s1[i - 1])); // deleting
            assert(operCost.size() == matchConfig.CompareOperCount);

            // Choose and store the smallest cost
            for (GADS::IndexType k = 0; k < operCost.size(); k++)
            {
                if (operCost[k] < t[i][j])
                    t[i][j] = operCost[k];
            }
        }
    }

    return matchConfig.getResult(t);

}


// Partition sequential list of data values into ranges using at most given number of dividers and minimizing the sum of the biggest range
// Returns list of dividers of the original sequence
GADS::IndexSeq GADS::partitionRange(const DataVct& data, IndexType maxDividers)
{
    GADS::IndexSeq result; // Return value
    
    // Input data refinement and validation
   
    GADS::IndexType dataCount = data.size();
    GADS::IndexType rangeCount = maxDividers + 1;

    if ( dataCount == 0 ) // You cannot divide sequence of 0 elements
        return result; // Return empty list

    
    // Data structures for computation

    // Stores range cost (sum)
    GADS::GenericVector<DataVct> partitionCost(dataCount);
    for (auto& values: partitionCost)
        values.resize(rangeCount);


    // Stores end of indentified range (divider)
    GADS::GenericVector<IndexVct> rangeEnd(dataCount);
    for (auto& values: rangeEnd)
        values.resize(rangeCount);


    // Helper with precalculated sums of prefixes
    GADS::DataVct prefixSum(dataCount);
    prefixSum[0] = data[0];
    for (GADS::IndexType i = 1; i < prefixSum.size(); i++)
    {
        prefixSum[i] = prefixSum[i-1] + data[i];
    }

    
    // Setting initial values of data structures
    
    for (GADS::IndexType i = 0; i < dataCount; i++)
        for (GADS::IndexType j = 0; j < rangeCount; j++)
        {
            if (i==0) // Boundary condition
                partitionCost[0][j] = data[0];

            else if (j==0) // Boundary condition
                partitionCost[i][0] = prefixSum[i];

            else // Other cases
                partitionCost[i][j] = std::numeric_limits<ValueType>::max();
        }


    // Main algorithm loop

    for (GADS::IndexType i = 1; i < dataCount; i++) // Iterate over number of elements
    {
        for (GADS::IndexType j = 1; j < rangeCount; j++) // Iterate over number of ranges
        {
            for (GADS::IndexType k = 0; k < i; k++) // Check last range starting at [0,i)
            {
                // cost of range j starting at elem k
                GADS::ValueType cost = std::max( partitionCost[k][j-1], prefixSum[i] - prefixSum[k] );

                if (cost < partitionCost[i][j]) // Register the smallest one
                {
                    partitionCost[i][j] = cost; // Store cost
                    rangeEnd[i][j] = k; // Store range end
                }
            }
        }
    }

    // Set dividers in the final result

    // Find all relevant range dividers
    for (GADS::IndexType k = rangeCount - 1, n = dataCount - 1;
            (k > 0) && (n > 0); // Stop if all dividers or items were used
            k--  // Number of dividers decreases with each iteration
        )
    {
        GADS::IndexType previousRangeEnd = rangeEnd[n][k];
        result.push_front(previousRangeEnd); // Add divider
        n = previousRangeEnd; // Now consider last range ending at previousRangeEnd
    }

    return result;
}

