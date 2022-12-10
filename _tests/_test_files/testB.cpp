#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>

#include "../../includes/sql/sql.h"

using namespace std;

bool test_stub(bool debug = false)
{
  // SQL sql("_!select.txt");
  SQL sql;

  string cmd = "make table student fields  fname,          lname,    major,    age";
  sql.command(cmd);
  cmd = "insert into student values Flo,            Yao, 	CS, 	20";
  sql.command(cmd);
  cmd = "insert into student values Bo, 		     Yang, 	CS, 		28";
  sql.command(cmd);
  cmd = "insert into student values \"Sammuel L.\", Jackson, 	CS, 		40";
  sql.command(cmd);

  cmd = "select * from student where lname = Yang or major = CS and age < 23 or lname = Jackson";
  sql.command(cmd);

  return true;
}

TEST(TEST_STUB, TestStub) {
  
  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_stub(false));
}




int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}

