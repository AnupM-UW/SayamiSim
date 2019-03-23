#include <iostream>
#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "../../elma_port/elma.h"
#include "../circularbuffer.h"

#ifdef _RPI_
#include <json/nlohmann_json.h>
#else
#include <json/json.h>
#endif

using namespace std;
using namespace elma;
using json = nlohmann::json;

TEST(CircularBuffer, InitialEmpty) {
    CircularBuffer<string> buff = CircularBuffer<string>(5);
    EXPECT_EQ(buff.size(), 0);
    EXPECT_EQ(buff.isEmpty(), true);
    EXPECT_EQ(buff.isFull(), false);
}

TEST(CircularBuffer, AddItems) {
    CircularBuffer<string> buff = CircularBuffer<string>(5);
    buff.insert("1");
    buff.insert("2");
    EXPECT_EQ(buff.size(), 2);
    EXPECT_EQ(buff.isEmpty(), false);
    EXPECT_EQ(buff.isFull(), false);
}

TEST(CircularBuffer, RemoveItems) {
    CircularBuffer<string> buff = CircularBuffer<string>(5);
    buff.insert("1");
    buff.insert("2");
    EXPECT_EQ(buff.size(), 2);
    string ret = buff.removeitem();
    EXPECT_EQ(buff.size(), 1);
    EXPECT_EQ(ret, "2");
    string ret = buff.removeitem();
    EXPECT_EQ(buff.size(), 0);
    EXPECT_EQ(ret, "1");
    EXPECT_EQ(buff.isFull(), false);

}

TEST(CircularBuffer, Filling) {
    CircularBuffer<string> buff = CircularBuffer<string>(5);
    buff.insert("1");
    buff.insert("2");
    buff.insert("3");
    buff.insert("4");
    buff.insert("5");
    EXPECT_EQ(buff.size(), 5);
    EXPECT_EQ(buff.isFull(), true);
    EXPECT_EQ(buff.isEmpty(), false);
    string ret = buff.removeitem();
    EXPECT_EQ(buff.size(), 4);
    EXPECT_EQ(ret, "5");
    EXPECT_EQ(buff.isFull(), false);
    EXPECT_EQ(buff.isEmpty(), false);
}

TEST(CircularBuffer, OverFilling) {
    CircularBuffer<string> buff = CircularBuffer<string>(5);
    buff.insert("1");
    buff.insert("2");
    buff.insert("3");
    buff.insert("4");
    buff.insert("5");
    buff.insert("6");
    buff.insert("7");
    EXPECT_EQ(buff.size(), 5);
    EXPECT_EQ(buff.isFull(), true);
    EXPECT_EQ(buff.isEmpty(), false);
    string ret = buff.removeitem();
    EXPECT_EQ(buff.size(), 4);
    EXPECT_EQ(ret, "7");
    EXPECT_EQ(buff.isFull(), false);
    EXPECT_EQ(buff.isEmpty(), false);
    buff.insert("7");
    buff.insert("8");
    EXPECT_EQ(buff.isFull(), true);
    EXPECT_EQ(buff.isEmpty(), false);
    EXPECT_EQ(buff.size(), 5);
}

TEST(CircularBuffer, OverEmptying) {
    CircularBuffer<string> buff = CircularBuffer<string>(5);
    buff.insert("1");
    buff.insert("2");
    EXPECT_EQ(buff.size(), 2);
    EXPECT_EQ(buff.isFull(), false);
    EXPECT_EQ(buff.isEmpty(), false);
    string ret = buff.removeitem();
    EXPECT_EQ(buff.size(), 1);
    EXPECT_EQ(ret, "2");
    EXPECT_EQ(buff.isFull(), false);
    EXPECT_EQ(buff.isEmpty(), true);
    string ret = buff.removeitem();
    EXPECT_EQ(buff.size(), 0);
    EXPECT_EQ(ret, "1");
    EXPECT_ANY_THROW(buff.removeitem());
}

TEST(CircularBuffer, Emptying) {
    CircularBuffer<string> buff = CircularBuffer<string>(5);
    buff.insert("1");
    buff.insert("2");
    EXPECT_EQ(buff.size(), 2);
    EXPECT_EQ(buff.isFull(), false);
    EXPECT_EQ(buff.isEmpty(), false);
    string ret = buff.removeitem();
    EXPECT_EQ(buff.size(), 1);
    EXPECT_EQ(ret, "2");
    EXPECT_EQ(buff.isFull(), false);
    EXPECT_EQ(buff.isEmpty(), true);
}

GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}