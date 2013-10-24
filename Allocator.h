// ------------------------------
// projects/allocator/Allocator.h
// Copyright (C) 2013
// Glenn P. Downing
// ------------------------------

#ifndef Allocator_h
#define Allocator_h

// --------
// includes
// --------

#include <cassert> // assert
#include <cstddef> // ptrdiff_t, size_t
#include <new>     // new

// ---------
// Allocator
// ---------

template <typename T, int N>
class Allocator {
    public:
        // --------
        // typedefs
        // --------

        typedef T                 value_type;

        typedef std::size_t       size_type;
        typedef std::ptrdiff_t    difference_type;

        typedef       value_type*       pointer;
        typedef const value_type* const_pointer;

        typedef       value_type&       reference;
        typedef const value_type& const_reference;

    public:
        // -----------
        // operator ==
        // -----------

        friend bool operator == (const Allocator&, const Allocator&) {
            return true;}                                              // this is correct

        // -----------
        // operator !=
        // -----------

        friend bool operator != (const Allocator& lhs, const Allocator& rhs) {
            return !(lhs == rhs);}

    private:
	
	
	int get_int_val(char* ptr)
	{
		return *reinterpret_cast<const int*>(ptr);
	}

	void set_int_val(char* ptr, int a)
	{
		int* new_ptr = (int*)(ptr);
		*new_ptr=a;
	}

        // ----
        // data
        // ----

        char a[N];

        // -----
        // valid
        // -----

        /**
         * O(1) in space
         * O(n) in time
         * Checks if all sentinels match up
         */
        bool valid () const {
            int* ptr=(int*)a;
            while(ptr<(int*)(&a[N-4]))
            {
                printf("[%d][",*ptr);
                int size = abs(*ptr);
                ptr =(int*)( (char* )ptr +size +4);

                for(int i=0;i<size;i++)
                {
                        printf(".");
                }
                printf("][%d]",*ptr);

                if(abs(*ptr) != size)
                {
                        return false;
                }
                ptr=(int*)((char*)ptr+4);
            }
            printf("\n");
            return true;}


        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         */
        int& view (int i) {
            return *reinterpret_cast<int*>(&a[i]);}

    public:
        // ------------
        // constructors
        // ------------

        /**
         * O(1) in space
         * O(1) in time
         * Sets up start and end sentinel
         */
        Allocator () {
            int* ptr = (int*)a;
            *ptr = N-8;
            ptr= (int*) (&a[N-1]-3);
            *ptr = N-8;
            //a= [N-8][........][N-8]
            assert(valid());}

        // Default copy, destructor, and copy assignment
        // Allocator  (const Allocator&);
        // ~Allocator ();
        // Allocator& operator = (const Allocator&);

        // --------
        // allocate
        // --------



        /**
         * O(1) in space
         * O(n) in time
         * Finds the first free block and allocates it for size n
         * after allocation there must be enough space left for a valid block
         * the smallest allowable block is sizeof(T) + (2 * sizeof(int))
         * choose the first block that fits
         */

        pointer allocate (size_type n) {
	char* ptr=a;
	int minblock =sizeof(T) + (2 * sizeof(int));
    	int n_bytes = n*sizeof(T);

	//search until empty sentinel
	while(ptr<&a[N-4])
	{
		//ptr is pointing to left sentinel
		//check if empty
		int sent_val = get_int_val(ptr);
		if(sent_val>=n_bytes+8)
		{	
			//check if free space left is enough to form another free block
			if(sent_val>n_bytes+8+minblock)
			{
				int free_space = sent_val-n_bytes-8;
				char* end = ptr+ sent_val+4;

				//allocate block
				//TODO create allocate_consec(ptr,n);
				assert(n_bytes==sizeof(T)*n);
				set_int_val(ptr,-n_bytes);
				ptr=ptr+n_bytes+4;
				set_int_val(ptr,-n_bytes);


				//rest of the free space
				set_int_val(end,free_space);
				end=end-free_space-4;
				set_int_val(end,free_space);
				

				return (pointer)(ptr-n_bytes);
				
			}
			else
			{
				//allocate whole block
				//TODO create a function to handle more than 1 obj
				//allocate_consec(ptr,n);
				int size =get_int_val(ptr);
				set_int_val(ptr, 0-size);
				set_int_val(ptr+size+4, 0-size);
				return ((pointer)(ptr+4));
			}
		}
		else
		{
			//block taken or block too small,move on to next block
			ptr+=*ptr+8;
		}
		
	}
	//return NULL if no space
	return NULL;
            assert(valid());
            return 0;}                   // replace!

        // ---------
        // construct
        // ---------

        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         */
        void construct (pointer p, const_reference v) {
            new (p) T(v);                               // this is correct and exempt
            assert(valid());}                           // from the prohibition of new

        // ----------
        // deallocate
        // ----------

        /**
         * O(1) in space
         * O(1) in time
	 * @param p: pointer to used block
         * Deallocates used blocks and coalesces free blocks
         */
        void deallocate (pointer p, size_type ) {

		//save start position, negate start sentinel
                char* start = (char*)(p)-4;
                assert(get_int_val(start)<0);
                set_int_val(start,0-get_int_val(start));
                assert(get_int_val(start)>0);

		//save end position, negate end sentinel
                char* end = (char*)(p)+get_int_val(start);
                assert(get_int_val(end)<0);
                set_int_val(end,0-get_int_val(end));
                assert(get_int_val(end)>0);

		//check if left block is within array bounds
                if(start>&a[3])
                {
                        char* left = start-4;
                        if(get_int_val(left)>0)
                                coalesce(left,start);
                }

		//check if right block is within array bounds
                if(end<&a[N-4])
                {
                        char* right = end +4;
                        assert(get_int_val(end)>0); 
                        if(get_int_val(right)>0)
                                coalesce(end,right);
                }
            assert(valid());}
        // ----------
        // coalesce
        // ----------

        /**
	 * @param right: pointer pointing to start sentinel of right block
         * @param left:  pointer pointing to end sentinel of left block
         * joins 2 free blocks
         */
        void coalesce (char* left, char* right)
        {
                char* ptr = left;
                assert(get_int_val(left)>0);
                assert(get_int_val(right)>0);
                //store new coalesced size
                int new_size = get_int_val(right) + 8 + get_int_val(left);

                //go to left start sentinel and change it to new size
                ptr = ptr - get_int_val(left) -4;
                set_int_val(ptr, new_size);

                //go to right end sentinel and change it to new size
                ptr = ptr+ get_int_val(ptr) +4;
                assert(ptr==right+4+get_int_val(right));
                set_int_val(ptr, new_size);
        }
        // -------
        // destroy
        // -------

        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         */
        void destroy (pointer p) {
            p->~T();               // this is correct
            assert(valid());}

        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         */
        const int& view (int i) const {
            return *reinterpret_cast<const int*>(&a[i]);}};

#endif // Allocator_h

