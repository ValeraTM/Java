#include "gtest/gtest.h"
#include <iostream>
#include "flat_map.h"

//gcovr -r.
//gcovr -r. --html --html-details -o flat_map.html
//gcovr -r. --html -o flat_map.html

class flat_mapTest : public testing::Test {
protected:
	flat_map<int, int> a;
	flat_map<int, int> empty;

	flat_mapTest() {
		// Здесь вы можете выполнить настройку для каждого теста.
	}

	~flat_mapTest() override {
		// Вы можете выполнять очистку, которая не генерирует здесь исключения.
	}

	// Если конструктора и деструктора недостаточно для настройки 
	// и очистки каждого теста, вы можете определить следующие методы:

	void  SetUp() override {
		a.insert(1, 1);
		a.insert(2, 2);
		a.insert(3, 3);
		a.insert(5, 5);
		a.insert(4, 4);
		a.insert(11, 11);
		a.insert(9, 9);
		a.insert(8, 8);
		a.insert(7, 7);
		a.insert(10, 10);
		a.insert(6, 6);
		a.insert(12, 12);
		// Код здесь вызывается сразу после конструктора(справа перед каждым тестом).
	}

	void  TearDown() override {
		// Здесь код будет вызываться сразу после каждого теста (справа перед деструктором).
	}
};

TEST_F(flat_mapTest, Method_Contains) {
	EXPECT_TRUE(a.contains(1));
	EXPECT_TRUE(a.contains(2));
	EXPECT_TRUE(a.contains(3));
	EXPECT_TRUE(a.contains(4));
	EXPECT_TRUE(a.contains(5));
	EXPECT_TRUE(a.contains(6));
	EXPECT_TRUE(a.contains(7));
	EXPECT_TRUE(a.contains(8));
	EXPECT_TRUE(a.contains(9));
	EXPECT_TRUE(a.contains(10));
	EXPECT_TRUE(a.contains(11));
	EXPECT_TRUE(a.contains(12));
	EXPECT_FALSE(a.contains(111));
	EXPECT_FALSE(empty.contains(1));
}

TEST_F(flat_mapTest, Iterator) {
	EXPECT_TRUE(a.begin() <= a.end());
	EXPECT_TRUE(a.begin() <= a.begin());
	EXPECT_EQ(a.begin(), a.end() - a.size());
}

TEST_F(flat_mapTest, Constructor) {
	//Copy constructor
	flat_map<int, int> c = a;
	EXPECT_EQ(a, c);
	//Move constructor
	flat_map<int, int> d = std::move(a);
	EXPECT_EQ(c, d);
	EXPECT_EQ(empty, a);
}

TEST_F(flat_mapTest, Method_Swap) {
	flat_map<int, int> b(2);
	b.insert(2, 2);
	flat_map<int, int> c = a;
	flat_map<int, int> d = b;
	a.swap(b);
	EXPECT_EQ(d, a);
	EXPECT_EQ(c, b);
	a.swap(a);
	EXPECT_EQ(d, a);
}

TEST_F(flat_mapTest, Method_Clear) {
	a.clear();
	EXPECT_EQ(empty, a);
	a.clear();
	EXPECT_EQ(empty, a);
}

TEST_F(flat_mapTest, Method_At) {
	const flat_map<int, int> b = a;
	const int c = b.at(1);
	EXPECT_EQ(1, c);
	EXPECT_ANY_THROW(b.at(31));
	EXPECT_ANY_THROW(a.at(31));
	EXPECT_NO_THROW(a.at(1));
	EXPECT_EQ(1, a.at(1));
}

TEST_F(flat_mapTest, Method_Erase) {
	EXPECT_FALSE(a.erase(31));
	EXPECT_TRUE(a.erase(1));
	EXPECT_FALSE(a.contains(1));
	EXPECT_TRUE(a.insert(1, 1));
	EXPECT_TRUE(a.contains(1));
}

TEST_F(flat_mapTest, Method_Insert) {
	EXPECT_TRUE(a.insert(31, 31));
	EXPECT_FALSE(a.insert(1, 2));
	EXPECT_TRUE(a.contains(1));
}

TEST_F(flat_mapTest, Operator_Equality) {
	flat_map<int, int> b = a;
	flat_map<int, int> c = b;
	EXPECT_TRUE(a == c);
	EXPECT_TRUE(a == b);
	EXPECT_TRUE(b == a);
	a.erase(1);
	EXPECT_FALSE(a == b);
	a.insert(30, 30);
	EXPECT_FALSE(a == b);
	EXPECT_TRUE(b != a);
}

TEST_F(flat_mapTest, Operator_Square_Brackets) {
	EXPECT_EQ(2, a[2]);
	EXPECT_EQ(1, a[1]);
	EXPECT_EQ(3, a[3]);
	EXPECT_EQ(4, a[4]);
	EXPECT_EQ(int(), a[15]);
}

TEST_F(flat_mapTest, Copy_Assigment_Operator) {
	empty = a;
	EXPECT_EQ(a, empty);
	EXPECT_TRUE(a == empty);
	a = a;
	EXPECT_EQ(a, empty);
	EXPECT_TRUE(a == empty);
}

TEST_F(flat_mapTest, Move_Assigment_Operator) {
	flat_map<int, int> d = a;
	flat_map<int, int> c;
	c = std::move(a);
	EXPECT_EQ(d, c);
	EXPECT_EQ(empty, a);
	EXPECT_TRUE(c.contains(1));
	EXPECT_TRUE(c.contains(2));
}

TEST_F(flat_mapTest, Other) {
	EXPECT_TRUE(empty.empty());
}

int main(int argc, char * argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}