// ------------------------------------
// projects/allocator/TestAllocator.c++
// Copyright (C) 2013
// Glenn P. Downing
// ------------------------------------

/*
To test the program:
    % ls -al /usr/include/gtest/
    ...
    gtest.h
    ...

    % locate libgtest.a
    /usr/lib/libgtest.a

    % locate libpthread.a
    /usr/lib/x86_64-linux-gnu/libpthread.a
    /usr/lib32/libpthread.a

    % locate libgtest_main.a
    /usr/lib/libgtest_main.a

    % g++ -pedantic -std=c++0x -Wall TestAllocator.c++ -o TestAllocator -lgtest -lpthread -lgtest_main

    % valgrind TestAllocator > TestAllocator.out
*/

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator

#include "gtest/gtest.h"

#include "Allocator.h"

// -------------
// TestAllocator
// -------------

template <typename A>
struct TestAllocator : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A                  allocator_type;
    typedef typename A::value_type      value_type;
    typedef typename A::difference_type difference_type;
    typedef typename A::pointer         pointer;};

typedef testing::Types<
            std::allocator<int>,
            std::allocator<double>,
            Allocator<int, 100>,
            Allocator<double, 100> >
        my_types;

TYPED_TEST_CASE(TestAllocator, my_types);

TYPED_TEST(TestAllocator, One) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

    allocator_type x;
    const difference_type s = 1;
    const value_type      v = 2;
    const pointer         p = x.allocate(s);
    if (p != 0) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator, Ten) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

    allocator_type x;
    const difference_type s = 10;
    const value_type      v = 2;
    const pointer         b = x.allocate(s);
    if (b != 0) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}

TYPED_TEST(TestAllocator, Three) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

    allocator_type x;
    const difference_type s = 3;
    const value_type      v = 2;
    const pointer         p = x.allocate(s);
    if (p != 0) {
        x.construct(p, v);
	x.construct(p+1, v);
	x.construct(p+2, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
	x.destroy(p+1);
	x.destroy(p+2);
        x.deallocate(p, s);}}

TEST(TestAllocator, IntSentinelCheck1) {

//Test that only one block is allocated
    Allocator<int, 100> x;
    int s = 1;
    int      v = 2;
    int*         p = x.allocate(s);
    int* end = (p+s);
    int* start = p-1;
    assert(*start==-4);
    assert(*end==-4);
    if (p != 0) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TEST(TestAllocator, IntSentinelCheck2) {

//Test that one free block is left
    Allocator<int, 100> x;
    int s = 20;
    int      v = 2;
    int*         p = x.allocate(s);
    int* end = (p+s);
    int* start = p-1;
    assert(*start==-80);
    assert(*end==-80);
    if (p != 0) {
        x.deallocate(p, s);}}

TEST(TestAllocator, IntSentinelCheck3) {

//Test that if there is no space for a new free block, the whole block is taken up
    Allocator<int, 100> x;
    int s = 21;
    int      v = 2;
    int*         p = x.allocate(s);
    int* start = p-1;
    assert(*start==-92);
    if (p != 0) {
        x.deallocate(p, s);}}

TEST(TestAllocator, DoubleSentinelCheck1) {

//Test that only one block is allocated
    Allocator<double, 100> x;
    int s = 1;
    double      v = 2;
    double*         p = x.allocate(s);
    int* end = (int*)(p+s);
    int* start = (int*)((char*)p-4);
    assert(*start==-8);
    assert(*end==-8);
    if (p != 0) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TEST(TestAllocator, DoubleSentinelCheck2) {

//Test that one free block is left
    Allocator<double, 100> x;
    int s = 9;
    double      v = 2;
    double*         p = x.allocate(s);
    int* end = (int*)(p+s);
    int* start = (int*)((char*)p-4);
    assert(*start==-72);
    assert(*end==-72);
    if (p != 0) {
        x.deallocate(p, s);}}

TEST(TestAllocator, DoubleSentinelCheck3) {

//Test that if there is no space for a new free block, the whole block is taken up
    Allocator<double, 100> x;
    int s = 10;
    double      v = 2;
    double*         p = x.allocate(s);
    int* start = (int*)((char*)p-4);
    double(*start==-92);
    if (p != 0) {
        x.deallocate(p, s);}}


TEST(TestAllocator, AllocateDeallocate5Doubles){

    Allocator<double, 100> x;
    int s=5;
    double v=2;
    double* p[5];
    int i=0;
    for(i=0;i<s;i++)
    {
 	p[i]=x.allocate(1);	
	int* start = (int*)((char*)p[i]-4);
	assert(*start==-8);
    }
    //5 allocated doubles
    for(i=0;i<s;i++)
    {
	x.deallocate(p[i],1);
    }
    
}

TEST(TestAllocator, AllocateDeallocate5Ints){

    Allocator<int, 100> x;
    int s=5;
    int v=2;
    int* p[5];
    int i=0;
    for(i=0;i<s;i++)
    {
 	p[i]=x.allocate(1);
	int* start = (int*)((char*)p[i]-4);
	assert(*start==-4);		
    }
    //5 allocated doubles
    for(i=0;i<s;i++)
    {
	x.deallocate(p[i],1);
    }
    
}
TEST(TestAllocator, AllocateDeallocate7Ints){

    Allocator<int, 100> x;
    int s=7;
    int v=2;
    int* p[7];
    int i=0;
    for(i=0;i<s;i++)
    {
 	p[i]=x.allocate(1);
	int* start = (int*)((char*)p[i]-4);
	assert(*start==-4);	
    }
    //5 allocated doubles
    for(i=0;i<s;i++)
    {
	x.deallocate(p[i],1);
    }
    
}
