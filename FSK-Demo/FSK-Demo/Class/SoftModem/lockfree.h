
#pragma once


#ifdef __APPLE__

#include <libkern/OSAtomic.h>

#endif


// This is a collection of lock-free data structures that use atomic operations
// to ensure thread safety. Based on the C# examples from a series of articles
// by Julian M. Bucknall. The first article can be found here:
//
// http://www.boyet.com/Articles/LockfreeStack.html

namespace lock_free
{

	// The linked list node for lock-free data structures
	template <typename T>
	struct node
	{
		node<T>* volatile next;
		T data;

		node():next(NULL),data(){}
	};

	// This is the classic atomic compare-and-swap operation. Pseudocode:
	// if (dest == oldValue){
	//     dest = newValue;
	//     return true;
	// }else{
	//     return false;
	// }
	template <typename T>
	inline static bool atomic_cas(node<T>* volatile &dest, node<T>* oldValue, node<T>* newValue)
	{
#ifdef WIN32
		return oldValue == (node<T>*)InterlockedCompareExchangePointer((volatile PVOID*)&dest, newValue, oldValue);
#endif
#ifdef __APPLE__
		return OSAtomicCompareAndSwapPtr(oldValue, newValue, (void* volatile *)&dest);
#endif
	}



	// Basic functionality of a lock-free stack implemented as a linked list
	template <typename T>
	class node_stack
	{
	public:
		node_stack()
		{
			head = NULL;
		}

		bool empty() const
		{
			return head;
		}

	protected:
		// Add a node to the head of the list
		void node_add(node<T>* pNewHead)
		{
			// Just keep trying to push until it succeeds
			do{
				pNewHead->next = head;
			}while(!atomic_cas(head, pNewHead->next, pNewHead));
		}

		// Pop a node from the head of the list
		node<T>* node_remove()
		{
			node<T>* pOldHead;

			// Keep trying to pop until it succeeds or the list is empty
			do{
				// If the head is NULL then the list is empty, so return NULL
				if(!(pOldHead = head))
					return NULL;
			}while(!atomic_cas(head, pOldHead, pOldHead->next));
			return pOldHead;
		}

		// Pointer to the head of the node list
		node<T>* volatile head;
	};



	// Basic functionality of a lock-free queue implemented as a linked list
	template <typename T>
	class node_queue
	{
	public:
		node_queue():tail(NULL),dummy(NULL){}

		bool empty() const
		{
			return tail == dummy;
		}

	protected:
		// Add a node to the tail of the list
		void node_add(node<T>* pNewTail)
		{
			node<T> *pOldTail = nullptr, *pOldNext;

			// Loop until we have managed to update the tail's Next link
			// to point to our new node
			bool done = false;
			while(!done)
			{
				pOldTail = tail;
				pOldNext = pOldTail->next;

				// A lot could have changed between those statements, so check
				// if things still look the same
				if(tail != pOldTail)
					continue;

				if(pOldNext)
				{
					// If the tail's Next field was non-null, then another thread
					// has pushed a new tail, so try to advance the tail pointer
					atomic_cas(tail, pOldTail, pOldNext);
					continue;
				}

				// Make our new node the tail's Next node
				done = atomic_cas(pOldTail->next, pOldNext, pNewTail);
			}

			// Try to advance the tail pointer to point to our node; if this fails,
			// that means another thread already took care of it
			atomic_cas(tail, pOldTail, pNewTail);
		}

		// Pop a node from the head of the list
		node<T>* node_remove()
		{
			T tempData = T();
			node<T> *pOldDummy = nullptr, *pOldHead;

			// Keep trying to pop until it succeeds or the list is empty
			bool done = false;
			while(!done)
			{
				pOldDummy = dummy;
				pOldHead = pOldDummy->next;
				node<T>* pOldTail = tail;

				// A lot could have changed between those statements, so check
				// if things still look the same
				if(dummy != pOldDummy)
					continue;

				// If the head is NULL then the list is empty, so return NULL
				if(!pOldHead)
					return NULL;

				// If the tail is pointed at the dummy but the list is not empty,
				// then we should try to advance the tail
				if(pOldTail == pOldDummy)
				{
					atomic_cas(tail, pOldTail, pOldHead);
					continue;
				}

				// Store the data in case this succeeds
				tempData = pOldHead->data;

				// Try to advance the dummy so the head becomes the new dummy
				done = atomic_cas(dummy, pOldDummy, pOldHead);
			}

			pOldDummy->data = tempData;
			return pOldDummy;
		}

		// Pointer to the dummy node at the head of the list
		node<T>* volatile dummy;

		// Pointer to the tail of the list
		node<T>* volatile tail;
	};



	// A simple new/delete allocator for lock-free nodes
	template <typename T>
	class node_allocator
	{
	public:
		node<T>* acquire()
		{
			return new node<T>;
		}

		void release(node<T>* pNode)
		{
			delete pNode;
		}
	};



	// A free-node list used to store and allocate nodes for lock-free structures
	template <typename T>
	class cached_node_allocator: public node_stack<T>
	{
		using node_stack<T>::node_remove;

	public:
		cached_node_allocator(int nPreAllocate = 0)
		{
			// Preallocate some nodes and add them to the free node list
			for(int i=0; i<nPreAllocate; ++i)
			{
				release(new node<T>);
			}
		}

		~cached_node_allocator()
		{
			// Pop and delete all nodes from the list
			node<T>* pNode;
			while((pNode = node_remove()))
				delete pNode;
		}


		// Get a free node
		node<T>* acquire()
		{
			// Try to get one from the head of the list
			node<T>* pOldHead = node_remove();

			// If the free node list is empty, allocate a new one
			if(!pOldHead)
				pOldHead = new node<T>;

			// Clear the Next pointer
			pOldHead->next = NULL;
			return pOldHead;
		}

		// Add a node to the head of the free node list
		void release(node<T>* pNode)
		{
			this->node_add(pNode);
		}
	};



	// A lock-free collection implementation layer with node allocator support
	template <template <typename> class impl_base, typename T, class Allocator>
	class collection_impl: public impl_base<T>
	{
		using impl_base<T>::node_remove;

	public:
		collection_impl(Allocator* pAlloc = NULL)
		{
			if(ownedAllocator == !pAlloc)
				alloc = new Allocator;
			else
				alloc = pAlloc;
		}

		collection_impl(int preallocated)
		{
			ownedAllocator = true;
			alloc = new Allocator(preallocated);
		}

		~collection_impl(void)
		{
			// Remove all nodes from the list
			node<T>* pNode;
			while((pNode = node_remove()))
				alloc->release(pNode);

			if(ownedAllocator)
				delete alloc;
			alloc = NULL;
		}

		// Add a data item to the collection
		void add(T data)
		{
			// Get a free node and populate it
			node<T>* pNewNode = alloc->acquire();
			pNewNode->data = data;

			// Add the node to the collection
			this->node_add(pNewNode);
		}

		// Remove a data item from the collection
		bool remove(T& data)
		{
			// Remove the node
			node<T>* pOldNode = node_remove();
			if(!pOldNode)
				return false;

			// Output the data
			data = pOldNode->data;

			// Release the node
			alloc->release(pOldNode);
			return true;
		}

	protected:
		// Pointer to the node allocator
		Allocator* alloc;

		// True if the allocator should be deleted upon destruction
		bool ownedAllocator;
	};



	// A lock-free stack implemented as a linked list
	template <typename T, class Allocator = cached_node_allocator<T> >
	class stack: public collection_impl<node_stack, T, Allocator>
	{
	public:
		stack(Allocator* pAlloc = NULL): collection_impl<node_stack, T, Allocator>(pAlloc){}

		stack(int preallocated): collection_impl<node_stack, T, Allocator>(preallocated){}

		// Push a data item onto the top of the stack
		void push(T data)
		{
			add(data);
		}

		// Pop a data item off the top of the stack
		bool pop(T& data)
		{
			return remove(data);
		}
	};



	// A lock-free queue implemented as a linked list
	template <typename T, class Allocator = cached_node_allocator<T> >
	class queue: public collection_impl<node_queue, T, Allocator>
	{
		using collection_impl<node_queue, T, Allocator>::tail;
		using collection_impl<node_queue, T, Allocator>::dummy;
		using collection_impl<node_queue, T, Allocator>::alloc;

	public:
		queue(Allocator* pAlloc = NULL): collection_impl<node_queue, T, Allocator>(pAlloc)
		{
			// Allocate the dummy node
			tail = dummy = alloc->acquire();
		}

		queue(int preallocated): collection_impl<node_queue, T, Allocator>(preallocated)
		{
			// Allocate the dummy node
			tail = dummy = alloc->acquire();
		}

		// Pop a data item off the head of the list
		bool get(T& data)
		{
			return this->remove(data);
		}

		// Peek at the head of the queue without removing it
		// Only truly safe in single-consumer scenarios!
		bool peek(T& data)
		{
			node<T> *head = dummy->next;
			if(!head)
				return false;
			data = head->data;
			return true;
		}

		// Push a data item onto the tail of the list
		void put(T data)
		{
			this->add(data);
		}
	};

};
