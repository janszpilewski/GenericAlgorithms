#pragma once

#include "GeneralAlgorithmsAndDataStructures.h"

namespace GeneralAlgorithmsAndDataStructures
{
	
	// Basic operations for generic heap data structure and heap sort
	// Does not control lifecycle of underlying vector
	// Comparer - comparer functor class with two params
	// true if first value goes before second following implemented comparing rules
	template <typename T, typename Comparer> class HeapBase
	{
	public:

		// Build Heap from vector rvalue (with moving)
		HeapBase(GenericVector<T>& heapData) : m_heapData(heapData) {}

		// Return number of elements in the heap
		virtual IndexType size() { return m_heapData.size(); }

		// Return the front element
		const T& top() { return m_heapData.front(); }


		// Remove and return the front element, update the heap
		T extractTop()
		{
			T top = std::move(m_heapData[TopIdx]);
			removeItemFromHeap(TopIdx);
			return top;
		}

		// Delete item with given value, return true if succeeded
		// size() is virtual and may not refer to the end of the array
		bool deleteItem(const T& val)
		{
			auto iter = std::find(m_heapData.begin(), m_heapData.begin() + size(), val);
			bool elementFound = iter < m_heapData.begin() + size();
			if (elementFound) // Element found, delete it
				removeItemFromHeap(iter - m_heapData.begin());

			return elementFound;
		}

	protected:

		constexpr static IndexType TopIdx = 0; // First index in the array

		// Storage of the heap data, it is provided by a client or a derived class
		GenericVector<T>& m_heapData;

		// Build Heap from initially unordered data in the storage vector
		// Must be called by a derived class usually in constructor once the storage was filled 
		void heapify()
		{
			// Bubble down starting with n-1 row , complexity m*lg n - lg m!  => O(n)
			// Do not decrease potentially unsigned parentIdx below zero (TopIdx may be zero)
			for (auto parentIdx = getParentIdx(getLastIndex()); parentIdx > TopIdx; parentIdx--)
				bubbleDown(parentIdx);
			bubbleDown(TopIdx);
		}

		// Bubbles up starting from element with given index
		void bubbleUp(IndexType itemIdx)
		{
			assert(itemIdx >= TopIdx && itemIdx < size());

			// Create comparer object
			Comparer compare;

			// Order is not set yet
			bool orderSet = false;

			while (!orderSet && itemIdx != TopIdx)
			{
				T& item = m_heapData[itemIdx];

				// Get parent idx and reference
				IndexType parentIdx = getParentIdx(itemIdx);
				T& parent = m_heapData[parentIdx];

				if (compare(item, parent))
				{
					std::swap(item, parent);
					itemIdx = parentIdx;
				}
				else
					orderSet = true; // No need to run further iterations
			}

		}

		// Bubbles down starting from element with given index
		void bubbleDown(IndexType itemIdx)
		{
			// Preliminary checks			
			if (itemIdx == getLastIndex()) // Item already at last position
				return;
			else if (itemIdx > getLastIndex())
				assert(false); // Illegal index value

			// Select child node for swap, initially left
			IndexType topChildIdx = getLeftChildIdx(itemIdx);

			// Order is not set yet
			bool orderSet = false;
			Comparer compare;

			while (!orderSet && topChildIdx <= getLastIndex()) // Bubbling down loop
			{
				// Now consider the right child
				IndexType rightChildIdx = getRightChildIdx(itemIdx); // May be bigger then vector size if child not present!

				// Check conditions for selecting right child node for swapping with parent
				if (
					(rightChildIdx <= getLastIndex()) // Right child exists 
					&& (compare(m_heapData[rightChildIdx], m_heapData[topChildIdx])) // Right child compares before left child
					)
				{
					topChildIdx = rightChildIdx;
				}

				// Compare item to smaller child
				if (compare(m_heapData[topChildIdx], m_heapData[itemIdx])) // If swap is needed
				{
					std::swap(m_heapData[itemIdx], m_heapData[topChildIdx]);
					topChildIdx = getLeftChildIdx(topChildIdx);
				}
				else
					orderSet = true; // No need to run further iterations
			}

		} // End of func bubbleDown()


		// Last item is no longer used
		virtual void dropLastElem() { m_heapData.pop_back(); }

		// Removes item with given index and rebalances the heap
		void removeItemFromHeap(IndexType targetIdx)
		{
			if (targetIdx < getLastIndex())
			{
				m_heapData[targetIdx] = std::move(m_heapData[getLastIndex()]);
				dropLastElem();
				bubbleDown(targetIdx);
			}
			else if (targetIdx == getLastIndex())
				dropLastElem();
			else
				assert(false); // Illegal index
		}

		// Getter utilities

		IndexType getParentIdx(IndexType itemIdx)
		{
			assert(itemIdx > TopIdx);
			return (itemIdx - 1) / 2;
		}

		IndexType getLastIndex()
		{
			assert(size() > 0);
			return size() - 1;
		}


		IndexType getLeftChildIdx(IndexType itemIdx) { return (itemIdx * 2) + 1; } // Only math, may not fit the Heap size
		IndexType getRightChildIdx(IndexType itemIdx) { return (itemIdx * 2) + 2; } // Only math, may not fit the Heap size


	}; // End of class HeapBase
	
	   
	// Heap data structure implementation, provides storage for HeapBase and reuses its implementation 
	template <typename T, typename Comparer> class Heap : public HeapBase<T, Comparer>
	{
	public:
		// Create empty Heap
		Heap(int initialCapacity = 16) : HeapBase<T, Comparer>(m_heapDataStorage) { m_heapDataStorage.reserve(initialCapacity); }

		// Build Heap from vector reference (with copy)
		Heap(const GenericVector<T>& heapData) : HeapBase<T, Comparer>(m_heapDataStorage), m_heapDataStorage(heapData) { HeapBase<T, Comparer>::heapify(); }

		// Build Heap from vector rvalue (with moving)
		Heap(GenericVector<T>&& heapData) : HeapBase<T, Comparer>(m_heapDataStorage), m_heapDataStorage(std::move(heapData)) { HeapBase<T, Comparer>::heapify(); }

		// Build Heap from rvalue initializer list (with moving)
		Heap(std::initializer_list<T>&& heapData) : HeapBase<T, Comparer>(m_heapDataStorage), m_heapDataStorage(std::move(heapData)) { HeapBase<T, Comparer>::heapify(); }

		// Add new element (possibly building it from its constructor parameters through emplace_back())
		// and update Heap, variadic function template using universal reference to pass constructor params
		template<typename... Args> void insert(Args&&... item)
		{
			// Add new element at the end and bubble it up to its proper position
			m_heapDataStorage.emplace_back(std::forward<Args>(item)...);
			HeapBase<T, Comparer>::bubbleUp(HeapBase<T, Comparer>::getLastIndex());
		}


	private:

		// Storage of the heap data
		GenericVector<T> m_heapDataStorage;



	}; // End of class Heap

	// Min Heap
	template <typename T> using MinHeap = Heap< T, std::less<T> >;

	// Max Heap
	template <typename T> using MaxHeap = Heap< T, std::greater<T> >;

	// Inplace heapsort helper 
	template <typename T> class HeapSorter : private HeapBase< T, std::greater<T> >
	{
	public:
		// Build Heap using provided vector with data to sort
		HeapSorter(GenericVector<T>& heapData) : HeapBase< T, std::greater<T> >(heapData), m_nbOfElementsInHeap(heapData.size()) {}

		void sort()
		{
			HeapBase< T, std::greater<T> >::heapify();
			
			while (m_nbOfElementsInHeap > 1)
			{
				T biggest = HeapBase< T, std::greater<T> >::extractTop(); // Extract the biggest element, calls dropLastElem()
				HeapBase< T, std::greater<T> >::m_heapData[m_nbOfElementsInHeap] = std::move(biggest); // Put it outside of current heap range
			}
		}

	private:
		IndexType m_nbOfElementsInHeap;

		// Redefine virtual functions to match heapsort requirements
		IndexType size() override { return m_nbOfElementsInHeap; }
		void dropLastElem() override { m_nbOfElementsInHeap--; }
	};

	// Inplace heap sort function
	template <typename T> void heapsort(GenericVector<T>& data)
	{
		HeapSorter<T> sorter(data);
		sorter.sort();
	}
}