#pragma once

#include <vector>
#include <list>
#include <string>
#include <set>
#include <map>
#include <utility>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <functional>
#include <memory>


namespace GeneralAlgorithmsAndDataStructures
{

    // Template based types

    template <typename T> using GenericVector = std::vector<T>;
    template <typename T> using GenericVectorIterator = typename GenericVector<T>::iterator;
    template <typename T> using GenericVectorConstIterator = typename GenericVector<T>::const_iterator;
    template <typename T> using GenericVectorIteratorDistance = typename std::iterator_traits< GenericVectorIterator<T> >::difference_type;

    template <typename T> using GenericSequence = std::list<T>;
    template <typename T> using GenericSequenceIterator = typename GenericSequence<T>::iterator;
    template <typename T> using GenericSequenceConstIterator = typename GenericSequence<T>::const_iterator;

    template <typename T> using GenericSet = std::set<T>;
    template <typename T> using GenericSetIterator = typename GenericSet<T>::iterator;
    template <typename T> using GenericSetConstIterator = typename GenericSet<T>::const_iterator;


    // Aliases

    // Default index type
    using IndexType = size_t;

    // Default value type
    using ValueType = int;

    // Default data vector container
    using DataVct = GenericVector<ValueType>;

    // Default data sequence container
    using DataSeq = GenericSequence<ValueType>;

    // Default index vector container
    using IndexVct = GenericVector<IndexType>;

    // Default index sequence container
    using IndexSeq = GenericSequence<IndexType>;


    // Constants

    constexpr int NotFound = -1;


    // Iterator based implementation of the Lomuto partition algorithm in range [beg, end), takes initial pivot position as parameter 
    // Returns position of the pivot in the partitioned list
    // Returns end if no partitioning could be done
    template <typename T> GenericVectorIterator<T> partitionLomuto(const GenericVectorIterator<T> beg, const GenericVectorIterator<T> end,
        const GenericVectorIterator<T> pivotPos)
    {
        GenericVectorIterator<T> result = end;

        if (beg < end)
        {
            auto insertPos = beg; // Insert smaller elements at beginning
            auto lastElemPos = end - 1;
            if (insertPos < lastElemPos) // More than one element => the elements can be partitioned
            {
                if (pivotPos != lastElemPos) // Put pivot at the end of the list
                    std::swap(*pivotPos, *lastElemPos);
                const auto& pivotVal = *lastElemPos;  // Get pivot value

                // Traverse list comparing to pivot
                for (auto it = beg; it < lastElemPos; ++it)
                {
                    if (*it <= pivotVal)
                    {
                        if (it != insertPos)
                            std::swap(*it, *insertPos);
                        ++insertPos;
                    }
                }

                // Move pivot to the position of the first not smaller element
                if (insertPos != lastElemPos)
                    std::swap(*lastElemPos, *insertPos);

            }

            result = insertPos;
        }

        return result;
    }

    // Use last element as default pivot position
    template <typename T> GenericVectorIterator<T> partitionLomuto(const GenericVectorIterator<T> beg, const GenericVectorIterator<T> end)
    {
        if (beg < end)
            return partitionLomuto<T>(beg, end, end - 1);
        else
            return end;
    }

    // Find value of n-th smallest element using 1 based counting
    template <typename T>  T findNthSmallestElem(GenericVectorIterator<T> beg, GenericVectorIterator<T> end, GenericVectorIteratorDistance<T> n)
    {
        if ((n <= 0) || (std::distance(beg, end) < n))
            throw std::out_of_range("Range does not contain n elements");
        else
        {
            auto foundPos = end; // Not found yet
            auto targetPos = beg + (n - 1); // Convert to 0 based iterator

            while (foundPos != targetPos) // Loop until right element is found
            {
                assert(beg < end);
                foundPos = partitionLomuto<T>(beg, end);
                if (foundPos > targetPos)
                    end = foundPos;
                else if (foundPos < targetPos)
                    beg = foundPos + 1;
            }

            return *foundPos;
        }
    }


    // Implementation of Quicksort of elements in range [beg,end)
    template <typename T> void quickSortInPlace(const GenericVectorIterator<T> beg, const GenericVectorIterator<T> end)
    {
        auto pivotPos = partitionLomuto<T>(beg, end);
        if (pivotPos != end)
        {
            // Element at pivotPos is already at its final destination
            quickSortInPlace<T>(beg, pivotPos);
            quickSortInPlace<T>(pivotPos + 1, end);
        }
    }

    // Quick sort of an entire generic container
    template <typename VT> void quickSortInPlace(VT&  vct)
    {
        quickSortInPlace <typename VT::value_type>(vct.begin(), vct.end());
    }

    // Used by Merge Sort
    template <typename T> void concatenateSortedContainers(const T& src1, const T& src2, T& dest)
    {
        dest.clear();
        auto totalSize = src1.size() + src2.size();
        dest.reserve(totalSize);

        auto src1It = src1.cbegin();
        auto src2It = src2.cbegin();

        for (size_t i = 0; i < totalSize; i++)
        {
            if (src1It == src1.end())
            {
                assert(src2It != src2.end());
                dest.emplace_back(*src2It++);
            }
            else if (src2It == src2.end())
            {
                assert(src1It != src1.end());
                dest.emplace_back(*src1It++);
            }
            else if (*src2It < *src1It)
                dest.emplace_back(*src2It++);
            else
                dest.emplace_back(*src1It++);
        }

        assert(src1It == src1.end() && src2It == src2.end()); // All elements were copied
    }

    // Implementation of Merge Sort of an entire generic container
    template <typename T> void mergeSort(T& vct)
    {
        if (vct.size() < 2)
            return;

        auto mid = vct.size() / 2; // Index of middle element

        // Create auxiliary containers
        T vct1(mid);
        T vct2(vct.size() - mid);
        assert(vct1.size() + vct2.size() == vct.size());

        // Fill auxiliary containers
        std::copy_n(vct.begin(), vct1.size(), vct1.begin());
        std::copy_n(vct.begin() + mid, vct2.size(), vct2.begin());

        // Sort auxiliary containers recursively
        mergeSort(vct1);
        mergeSort(vct2);

        //  Concatenate containers
        concatenateSortedContainers(vct1, vct2, vct);

    }


    // Binary search in the range [beg, end]
    // Returns index of the first element found
    // Returns a value > end if searched value could not be located
    template <typename Container, typename Target = typename Container::const_reference,
        typename EqualComp = std::equal_to<typename Container::value_type>,
        typename LessComp = std::less<typename Container::value_type> >
    IndexType binSearch(const Container& vct, const Target& target, 
        IndexType beg, IndexType end, EqualComp isEqual = EqualComp(), LessComp isSmaller = LessComp())
    {
        assert(beg >= 0);
        assert(end < vct.size());

        while (beg < end)
        {
            auto middle = (beg + end) / 2;

            if (isSmaller(vct[middle], target))
                beg = middle + 1;
            else
                end = middle;
        }

        if ( (beg == end) && (isEqual(vct[beg], target)) )
            return beg;
        else
            return vct.size(); // Not found
    }

    // Binary search in full container
    // using default comparers for data type
    template <typename Container>
        IndexType binSearch(const Container& vct, typename Container::const_reference target)
    {
        return binSearch(vct, target, 0, vct.size() - 1);
    }

    // Lower bound search (smallest value bigger than target) in the range [beg, end]
    // Returns index of the lower bound
    // Returns a value > end if lower bound does not exist
    template <typename T> IndexType lowerBound(const GenericVector<T>& vct, const T& target, IndexType beg, IndexType end)
    {
        assert(beg >= 0);
        assert(end < vct.size());

        if (beg > end)
            return vct.size(); // Invalid range
        if (target >= vct[end])
            return vct.size(); // Invalid target
        
        while (beg < end) // Advance until lower bound is reached
        {
            auto middle = (beg + end) / 2;

            if (vct[middle] > target)
                end = middle;
            else
                beg = middle + 1;

        }

        return beg;
    }

    // lower bound search in full container
    template <typename T> IndexType lowerBound(const GenericVector<T>& vct, const T& target)
    {
        return lowerBound(vct, target, 0, vct.size() - 1);
    }


  // Upper bound search (biggest value smaller than target) in the range [beg, end]
 // Returns index of the upper bound
 // Returns a value > end if upper bound does not exist
    template <typename T> IndexType upperBound(const GenericVector<T>& vct, const T& target, IndexType beg, IndexType end)
    {
        assert(beg >= 0);
        assert(end < vct.size());

        if (beg > end)
            return vct.size(); // Invalid range
        if (target <= vct[beg])
            return vct.size(); // Invalid target

        // Neeed for upper bound as integer division converges to left
        // hence the rightmost elem cannot be reached by dividing ranges ( (n + n+1)/2 = n )
        if (target >= vct[end])
            return end; // rightmost elem is upper bound

        while (beg < end) // Advance until upper bound is reached
        {
            auto middle = (beg + end) / 2;

            if (vct[middle] < target)
                beg = middle;
            else
                end = middle - 1;
        }

        return beg;
    }

    // Upper bound search in full container
    template <typename T> IndexType upperBound(const GenericVector<T>& vct, const T& target)
    {
        return upperBound(vct, target, 0, vct.size() - 1);
    }

 
    // Using template data types
	   
    // Reverses DataVct array in range [startIdx, endIdx]
    void reverseArray(DataVct& array, IndexType startIdx, IndexType endIdx);
    // Reverses entire DataVct array
    inline void reverseArray(DataVct& array) { return reverseArray(array, 0, array.size() - 1); }

    // Finds next increasing lexicographic permutation in DataVct using std::greater_equal comparer functor
    bool nextIncreasingLexicographicPermutation(DataVct& permutation);

    // Finds next decreasing lexicographic permutation in DataVct using std::less_equal comparer functor
    bool nextDecreasingLexicographicPermutation(DataVct& permutation);

    // Generatates next r combination in lexical order from range [0,maxVal]
    // Returns false if next combination cannot be calculated
    bool nextLexicographicRCombination(IndexType maxVal, IndexVct& rCombination);

    using GeneratedSequences = GenericSequence< DataSeq >;

    // Generates combinations with r elements
    GeneratedSequences generateRCombinations(const DataVct& inputSet, IndexType r);

    // Generates subsets of given data set
    // Empty subset is returned as the last one
    class SubsetGenerator
    {
    public:
        SubsetGenerator(DataVct& totalSet): m_totalSet(totalSet), m_subsetMask(totalSet.size()) {}
        DataVct nextSubset();

    private:
        const DataVct& m_totalSet;
        
        GenericVector<bool> m_subsetMask;
        void nextSubsetMask();
    };


    // Finds result of operation: base^exp % mod
    unsigned long modularExp(int base, unsigned int exp, unsigned int mod);

    // Finds length of the longest common subsequences of strings s1 and s2
    // If container was passed as a parameter it will contain matching subsequence or an empty string if there is no one
    IndexType longestCommonSubsequence(const std::string& s1, const std::string& s2, GenericSet < std::string >* sequences = nullptr);


    // Interface for configuring matching in func approximativeStringMatching
    class MatchConfig
    {
    public:
        typedef GenericVector<IndexType> Row;
        typedef GenericVector <Row> Table;

        static constexpr IndexType CompareOperCount = 3; // match, insert, delete

        // Creates and inits table of matching results
        virtual Table createTable(IndexType tableD1, IndexType tableD2) const = 0;

        // Cost of matching 2 characters
        virtual ValueType matchCost(char c1, char c2) const = 0;

        // Cost of inserting given character
        virtual ValueType insertCost(char c) const = 0;

        // Cost of deleting given character
        virtual ValueType deleteCost(char c) const = 0;

        // Find the final result
        virtual IndexType getResult(const Table& t) const = 0;

    };

    class MatchConfigGenericImpl : public MatchConfig
    {
    public:
        Table createTable(IndexType tableD1, IndexType tableD2) const override;
        ValueType matchCost(char c1, char c2) const override;
        ValueType insertCost(char c) const override;
        ValueType deleteCost(char c) const override;
        IndexType getResult(const Table& t) const override;

    };

    // Finds best match of strings s1 and s2 parametrized by object MatchConfig
    // If container was passed as a parameter it will contain matching subsequence or an empty string if there is no one
    IndexType approximativeStringMatching(const std::string& s1, const std::string& s2, const MatchConfig& matchConfig, GenericSet < std::string >* sequences = nullptr);


    // Partition sequential list of data values into ranges using at most given number of dividers and minimizing the sum of the biggest range
    // Returns list of dividers of the original sequence
    IndexSeq partitionRange(const DataVct& data, IndexType maxDividers);


} // End of namaspace GeneralAlgorithmsAndDataStructures
