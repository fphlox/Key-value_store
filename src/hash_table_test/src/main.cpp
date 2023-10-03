#include <gtest/gtest.h>

#include <optional>
#include <thread>

#include "hash_table/hash_table.hpp"

TEST(HASH, SET) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100";

  EXPECT_EQ(table.set(input), "OK");
}

TEST(HASH, GET) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100";
  s21::t_data ret;

  table.set(input);
  ret = *table.get("Key1");
  EXPECT_EQ(ret.last_name, "Doe");
  EXPECT_EQ(ret.first_name, "John");
  EXPECT_EQ(ret.year_of_birth, 1990);
  EXPECT_EQ(ret.city, "NewYork");
  EXPECT_EQ(ret.coins, 100);
}

TEST(HASH, EXISTS) {
  s21::HashTable table;
  std::string file_name = "src/hash_table_test/data/test.txt";
  std::string ret;

  ret = table.load(file_name);
  EXPECT_EQ(table.exists("key1"), true);
  EXPECT_EQ(table.exists("key2"), true);
  EXPECT_EQ(table.exists("key1"), true);
  EXPECT_EQ(table.exists("key101"), true);
  EXPECT_EQ(table.exists("AAA"), true);
  EXPECT_EQ(table.exists("AAAA"), false);
}

TEST(HASH, DEL) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100";
  std::string ret;

  ret = table.set(input);
  EXPECT_EQ(ret, "OK");
  EXPECT_EQ(table.del("Key1"), true);
  EXPECT_EQ(table.exists("Key1"), false);
  EXPECT_EQ(table.del("Key1"), false);
}

TEST(HASH, UPDATE) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100";
  s21::t_data ret;

  table.set(input);
  ret = *table.get("Key1");
  EXPECT_EQ(ret.last_name, "Doe");
  EXPECT_EQ(ret.first_name, "John");
  EXPECT_EQ(ret.year_of_birth, 1990);
  EXPECT_EQ(ret.city, "NewYork");
  EXPECT_EQ(ret.coins, 100);

  EXPECT_EQ(table.update("Key1 - LAL - NewHren 100000"), "OK");
  ret = *table.get("Key1");
  EXPECT_EQ(ret.last_name, "Doe");
  EXPECT_EQ(ret.first_name, "LAL");
  EXPECT_EQ(ret.year_of_birth, 1990);
  EXPECT_EQ(ret.city, "NewHren");
  EXPECT_EQ(ret.coins, 100000);
}

TEST(HASH, KEYS) {
  s21::HashTable table;
  std::string file_name = "src/hash_table_test/data/test.txt";
  std::vector<std::string> ret;

  table.load(file_name);
  ret = table.keys();
  EXPECT_EQ(ret.size(), 4);
  EXPECT_EQ("AAA", ret[3]);
  EXPECT_EQ("key1", ret[0]);
  EXPECT_EQ("key2", ret[1]);
  EXPECT_EQ("key101", ret[2]);
}

TEST(HASH, RENAME) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100";
  s21::t_data ret;

  EXPECT_EQ(table.set(input), "OK");
  table.rename("Key1", "Key2");
  ret = *table.get("Key2");
  EXPECT_EQ(ret.last_name, "Doe");
  EXPECT_EQ(ret.first_name, "John");
  EXPECT_EQ(ret.year_of_birth, 1990);
  EXPECT_EQ(ret.city, "NewYork");
  EXPECT_EQ(ret.coins, 100);
  EXPECT_EQ(table.exists("Key1"), false);
}

TEST(HASH, TTL) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100";

  EXPECT_EQ(table.set(input), "OK");
  EXPECT_EQ(table.ttl("Key1"), (std::numeric_limits<int>::infinity()));
  input = "Key2 Human Man 190 Rome 10 EX 0";
  EXPECT_EQ(table.set(input), "OK");
  EXPECT_EQ(table.ttl("Key2"), 0);
  EXPECT_EQ(table.exists("Key2"), false);
  input = "Key2 Human Man 190 Rome 10 EX 100";
  EXPECT_EQ(table.set(input), "OK");
  EXPECT_EQ(table.ttl("Key2"), 100);
}

TEST(HASH, FIND) {
  s21::HashTable table;
  std::string file_name = "src/hash_table_test/data/test.txt";
  std::vector<std::string> ret;
  std::string sub;

  EXPECT_EQ(table.load(file_name), "OK 4");
  ret = table.find("- - - - 55");
  EXPECT_EQ(ret.size(), 2);
  EXPECT_EQ(ret[1], "key1");
  EXPECT_EQ(ret[0], "key2");
}

TEST(HASH, SHOWALL) {
  s21::HashTable table;
  std::string file_name = "src/hash_table_test/data/test.txt";
  std::vector<s21::t_data> ret;
  std::string sub;

  EXPECT_EQ(table.load(file_name), "OK 4");
  ret = table.showall();
  EXPECT_EQ(ret.size(), 4);
  EXPECT_EQ(ret[1].last_name, "Sidorov");
  EXPECT_EQ(ret[1].first_name, "Sergei");
  EXPECT_EQ(ret[1].year_of_birth, 1847);
  EXPECT_EQ(ret[1].city, "Suzdal");
  EXPECT_EQ(ret[1].coins, 12312313);

  EXPECT_EQ(ret[2].last_name, "Ivanov");
  EXPECT_EQ(ret[2].first_name, "Vasiliy");
  EXPECT_EQ(ret[2].year_of_birth, 2000);
  EXPECT_EQ(ret[2].city, "Москва");
  EXPECT_EQ(ret[2].coins, 55);

  EXPECT_EQ(ret[3].last_name, "Vasilev");
  EXPECT_EQ(ret[3].first_name, "Ivan");
  EXPECT_EQ(ret[3].year_of_birth, 2001);
  EXPECT_EQ(ret[3].city, "Rostov");
  EXPECT_EQ(ret[3].coins, 55);

  EXPECT_EQ(ret[0].last_name, "Lalka");
  EXPECT_EQ(ret[0].first_name, "Lolnya");
  EXPECT_EQ(ret[0].year_of_birth, 3000);
  EXPECT_EQ(ret[0].city, "Hrenograd");
  EXPECT_EQ(ret[0].coins, 0);
}

TEST(HASH, LOAD) {
  s21::HashTable table;
  std::string file_name = "src/hash_table_test/data/test.txt";
  std::string ret;

  ret = table.load(file_name);
  EXPECT_EQ(ret, "OK 4");
}

TEST(HASH, FEXPORT) {
  s21::HashTable table;
  std::string file_name = "src/hash_table_test/data/test.txt";
  std::string ret;

  ret = table.fexport("bin/coverage/hash_table_test/__output__/out.txt");
  EXPECT_EQ(ret, "Table is empty");
  ret = table.load(file_name);
  EXPECT_EQ(ret, "OK 4");
  ret = table.fexport("bin/coverage/hash_table_test/__output__/out.txt");
  EXPECT_EQ(ret, "OK 4");
}

//              TIME    TIME    TIME        TIME        TIME

TEST(HASH, SET_TIME) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100 EX 1";
  s21::t_data ret;

  EXPECT_EQ(table.set(input), "OK");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  input = "Key1 LOL MAN 1990 NewYork 100 ";
  EXPECT_EQ(table.set(input), "OK");
  ret = *table.get("Key1");
  EXPECT_EQ(ret.last_name, "LOL");
  EXPECT_EQ(ret.first_name, "MAN");
  EXPECT_EQ(ret.year_of_birth, 1990);
  EXPECT_EQ(ret.city, "NewYork");
  EXPECT_EQ(ret.coins, 100);
}

TEST(HASH, GET_TIME) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100 EX 1";

  table.set(input);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  auto ret = table.get("Key1");
  EXPECT_EQ(ret, std::nullopt);
}

TEST(HASH, EXISTS_TIME) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100 EX 1";

  table.set(input);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(table.exists("Key1"), false);
}

TEST(HASH, DEL_TIME) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100 EX 1";
  std::string ret;

  ret = table.set(input);
  EXPECT_EQ(ret, "OK");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(table.del("Key1"), false);
}

TEST(HASH, UPDATE_TIME) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100 EX 1";

  table.set(input);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(table.update("Key1 - LAL - NewHren 100000"),
            "ERROR: update value not found");
}

TEST(HASH, KEYS_TIME) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100 EX 1";
  std::vector<std::string> ret;

  table.set(input);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  ret = table.keys();
  EXPECT_EQ(ret.size(), 0);
}

TEST(HASH, RENAME_TIME) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100 EX 1";

  EXPECT_EQ(table.set(input), "OK");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(table.rename("Key1", "Key2"), "ERROR: renamemy value not found");
}

TEST(HASH, TTL_TIME) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100 EX 1";

  EXPECT_EQ(table.set(input), "OK");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(table.ttl("Key1"), 0);
}

TEST(HASH, FIND_TIME) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100 EX 1";
  std::vector<std::string> ret;

  EXPECT_EQ(table.set(input), "OK");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  ret = table.find("Doe - - - 100");
  EXPECT_EQ(ret.size(), 0);
}

TEST(HASH, SHOWALL_TIME) {
  s21::HashTable table;
  std::string input = "Key1 Doe John 1990 NewYork 100 EX 1";
  std::vector<s21::t_data> ret;

  EXPECT_EQ(table.set(input), "OK");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  ret = table.showall();
  EXPECT_EQ(ret.size(), 0);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
