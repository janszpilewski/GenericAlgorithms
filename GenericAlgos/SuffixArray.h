#pragma once

#include "GeneralAlgorithmsAndDataStructures.h"

namespace GeneralAlgorithmsAndDataStructures
{

    // Builds suffix array for given text
    class SuffixArray
    {
    public:
        // Create suffix array from given text
        SuffixArray(std::string text) : m_text(std::move(text)), m_suffixArray(m_text.size())
        {
            buildSuffixArray();
        }

        // Receive input text
        const std::string& getInputText() const
        {
            return m_text;
        }

        // Receive result
        const IndexVct& get() const&
        {
            return m_suffixArray;
        }

        // Receive copy (possibly rvalue) of result
        IndexVct&& get() &&
        {
            return std::move(m_suffixArray);
        }

        // Verifies if given index is valid inside of the suffix array
        bool indexValid(IndexType idx) const
        {
            return idx < m_suffixArray.size();
        }


        // Algorithms
        
        // Get 1-based n-th suffix in lexicographic order
        // Throws std::out_of_range if n is not valid
        std::string getNthSuffix(IndexType n) const
        {
            return m_text.substr(m_suffixArray.at(n - 1));
        }


        // Finds length of longest common prefix of given suffixes in O(log n) time
        IndexType lcp(IndexType x, IndexType y) const;

        // Checks if text contains a substring in time O(log (size(text)) * size(subsr) )
        IndexType searchSubstring(const std::string& substr) const;

        // Returns minimal lexicographic rotation of the initial text in O(1) time
        std::string minLexRotation() const;


    private:

        enum class CompareResult { Equal, Smaller, Bigger, Undefined };

        std::string m_text; // Input text

        IndexVct m_suffixArray; // Output array

        // N * (1 + log(ceil(N))) matrix storing in every line order of sorted prefixes (of suffixes) of length 2^k (0<=k<=ceil(N))
        GenericVector < IndexVct > m_prefixOrderMatrix;

        void buildSuffixArray();

        // Compares suffix to a given substring
        // They are equal when substring is a prefix of suffix starting from index idx
        CompareResult compareSubstring(IndexType idx, const std::string& substr) const;

        bool isLexEqual(IndexType idx, const std::string& substr) const
        {
            return (compareSubstring(idx, substr) == CompareResult::Equal);
        }

        bool isLexSmaller(IndexType idx, const std::string& substr) const
        {
            return (compareSubstring(idx, substr) == CompareResult::Smaller);
        }

        IndexType outOfRangeVal() const { return m_text.size(); }

    }; // End of class SuffixArray


}