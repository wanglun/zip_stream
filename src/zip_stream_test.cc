#include <sstream>
#include <gtest/gtest.h>

#include "zip_stream.h"

TEST(zip_stream, unzip) {
  // file not exist
  {
    UnzipStream s;
    s.open("testdata/zip_stream.zip");
    ASSERT_TRUE(s.good());
    s.select("1.TXT");
    ASSERT_FALSE(s.good());
  }
  // copy to ostream
  {
    UnzipStream s;
    s.open("testdata/zip_stream.zip");
    ASSERT_TRUE(s.good());
    s.select("1.txt");
    ASSERT_TRUE(s.good());
    std::stringstream ss;
    ss << s.rdbuf();
    ASSERT_EQ("12345\nabcde\n", ss.str());
    ASSERT_TRUE(s.good());
    s.get();
    ASSERT_FALSE(s.good());
    ASSERT_TRUE(s.eof());
  }
  // getline
  {
    UnzipStream s;
    s.open("testdata/zip_stream.zip");
    ASSERT_TRUE(s.good());
    s.select("1.txt");
    ASSERT_TRUE(s.good());
    char buf[10];
    s.getline(buf, 10);
    ASSERT_TRUE(s.good());
    ASSERT_STREQ("12345", buf);
    s.getline(buf, 10);
    ASSERT_TRUE(s.good());
    ASSERT_STREQ("abcde", buf);
    ASSERT_TRUE(s.good());
    s.getline(buf, 10);
    ASSERT_TRUE(s.eof());
  }
  // file with password
  {
    UnzipStream s;
    s.open("testdata/zip_stream.zip");
    ASSERT_TRUE(s.good());
    s.select("2.txt");
    ASSERT_FALSE(s.good());

    s.select("2.txt", "wrong");
    ASSERT_FALSE(s.good());

    s.select("2.txt", "abc");
    ASSERT_TRUE(s.good());

    std::stringstream ss;
    ss << s.rdbuf();
    ASSERT_EQ("encrypted\n", ss.str());
    ASSERT_TRUE(s.good());
    s.get();
    ASSERT_FALSE(s.good());
    ASSERT_TRUE(s.eof());
  }
  // file in dir
  {
    UnzipStream s;
    s.open("testdata/zip_stream.zip");
    ASSERT_TRUE(s.good());
    s.select("dir/1.txt");
    ASSERT_TRUE(s.good());
    std::stringstream ss;
    ss << s.rdbuf();
    ASSERT_EQ("67890\nhijkl\n", ss.str());
    ASSERT_TRUE(s.good());
    s.get();
    ASSERT_FALSE(s.good());
    ASSERT_TRUE(s.eof());
  }
  // re select
  {
    UnzipStream s;
    s.open("testdata/zip_stream.zip");
    ASSERT_TRUE(s.good());
    {
      s.select("1.txt");
      ASSERT_TRUE(s.good());
      std::stringstream ss;
      ss << s.rdbuf();
      ASSERT_EQ("12345\nabcde\n", ss.str());
      ASSERT_TRUE(s.good());
    }

    {
      s.select("dir/1.txt");
      ASSERT_TRUE(s.good());
      std::stringstream ss;
      ss << s.rdbuf();
      ASSERT_EQ("67890\nhijkl\n", ss.str());
      ASSERT_TRUE(s.good());
    }
  }
  // re select the same file
  {
    UnzipStream s;
    s.open("testdata/zip_stream.zip");
    ASSERT_TRUE(s.good());
    {
      s.select("1.txt");
      ASSERT_TRUE(s.good());
      std::stringstream ss;
      ss << s.rdbuf();
      ASSERT_EQ("12345\nabcde\n", ss.str());
      ASSERT_TRUE(s.good());
    }

    {
      s.select("1.txt");
      ASSERT_TRUE(s.good());
      std::stringstream ss;
      ss << s.rdbuf();
      ASSERT_EQ("12345\nabcde\n", ss.str());
      ASSERT_TRUE(s.good());
    }
  }
  // select when open
  {
    UnzipStream s;
    s.open("testdata/zip_stream.zip", "1.txt");
    ASSERT_TRUE(s.good());
    std::stringstream ss;
    ss << s.rdbuf();
    ASSERT_EQ("12345\nabcde\n", ss.str());
    ASSERT_TRUE(s.good());
    s.get();
    ASSERT_FALSE(s.good());
    ASSERT_TRUE(s.eof());
  }
  // select with password when open
  {
    UnzipStream s;
    s.open("testdata/zip_stream.zip", "2.txt", "abc");
    ASSERT_TRUE(s.good());
    std::stringstream ss;
    ss << s.rdbuf();
    ASSERT_EQ("encrypted\n", ss.str());
    ASSERT_TRUE(s.good());
    s.get();
    ASSERT_FALSE(s.good());
    ASSERT_TRUE(s.eof());
  }
}
