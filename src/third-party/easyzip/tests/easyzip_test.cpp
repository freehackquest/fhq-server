#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "test_tools.h"
#include "easyzip/easyzip.h"
using namespace easyzip;

TEST(StringZipTest, Test1) {
  if (IsFileExists(L"zip测试.zip"))
    std::remove("zip测试.zip");

  Zipper zipper(L"zip测试.zip");

  std::stringstream strdata;
  strdata << "test string data compression";

  zipper.add(strdata, L"strdata");
  zipper.close();

  Unzipper unzipper(L"zip测试.zip");

  EXPECT_TRUE(unzipper.entries().size() == 1);
  EXPECT_TRUE(unzipper.entries().front().name == L"strdata");

  unzipper.extract();

  EXPECT_TRUE(IsFileExists(L"strdata"));

  std::ifstream testfile("strdata");
  EXPECT_TRUE(testfile.good());

  std::string test((std::istreambuf_iterator<char>(testfile)), std::istreambuf_iterator<char>());
  testfile.close();
  EXPECT_TRUE(test == "test string data compression");

  std::map<std::wstring, std::wstring> alt_names;
  alt_names[L"strdata"] = L"alternative_strdata.dat";

  unzipper.extract(L"", alt_names);

  EXPECT_TRUE(IsFileExists(L"alternative_strdata.dat"));

  std::ifstream testfile2("alternative_strdata.dat");
  EXPECT_TRUE(testfile2.good());

  std::string test2((std::istreambuf_iterator<char>(testfile2)), std::istreambuf_iterator<char>());
  testfile2.close();
  EXPECT_TRUE(test2 == "test string data compression");

  EXPECT_TRUE(false == unzipper.extractEntry(L"fake.dat"));

  unzipper.close();

  std::remove("strdata");
  std::remove("alternative_strdata.dat");
  std::remove("zip测试.zip");
}

TEST(FileZipTest, Test1) {
  if (IsFileExists(L"zip测试.zip"))
    std::remove("zip测试.zip");

  Zipper zipper(L"zip测试.zip");

  std::ofstream test1("test文件1.txt");
  test1 << "test file compression";
  test1.flush();
  test1.close();

  std::ifstream test1stream("test文件1.txt");

  // add test文件1.txt
  zipper.add(test1stream, L"test文件1.txt");

  test1stream.close();
  zipper.close();

  std::remove("test文件1.txt");

  Unzipper unzipper(L"zip测试.zip");

  std::vector<ZipEntry> entries = unzipper.entries();
  EXPECT_TRUE(entries.size() == 1);
  EXPECT_TRUE(entries.front().name == L"test文件1.txt");

  unzipper.extractEntry(L"test文件1.txt");
  // due to sections forking or creating different stacks we need to make
  // sure the local instance is closed to prevent mixing the closing when
  // both instances are freed at the end of the scope
  unzipper.close();

  EXPECT_TRUE(IsFileExists(L"test文件1.txt"));

  std::ifstream testfile("test文件1.txt");
  EXPECT_TRUE(testfile.good());

  std::string test((std::istreambuf_iterator<char>(testfile)), std::istreambuf_iterator<char>());
  testfile.close();
  EXPECT_TRUE(test == "test file compression");

  std::ofstream test2("test文件2.dat");
  test2 << "other data to compression test";
  test2.flush();
  test2.close();

  std::ifstream test2stream("test文件2.dat");

  zipper.open();
  // add 测试Folder/test文件2.dat
  zipper.add(test2stream, L"测试Folder/test文件2.dat");
  zipper.close();

  test2stream.close();
  std::remove("test文件2.dat");

  do {
    Unzipper unzipper(L"zip测试.zip");

    EXPECT_TRUE(unzipper.entries().size() == 2);
    EXPECT_TRUE(unzipper.entries().front().name == L"test文件1.txt");
    EXPECT_TRUE(unzipper.entries()[1].name == L"测试Folder/test文件2.dat");

    unzipper.extract();
    unzipper.close();

    EXPECT_TRUE(IsFileExists(L"测试Folder/test文件2.dat"));

    std::ifstream testfile("测试Folder/test文件2.dat");
    EXPECT_TRUE(testfile.good());

    std::string file_content((std::istreambuf_iterator<char>(testfile)),
                             std::istreambuf_iterator<char>());
    testfile.close();
    EXPECT_TRUE(file_content == "other data to compression test");

    MakeDir(GetCurrentPath() + L"/测试Files/子文件夹");
    std::ofstream test("测试Files/test文件1.txt");
    test << "test file compression";
    test.flush();
    test.close();

    std::ofstream test1("测试Files/test2.pdf");
    test1 << "test file compression";
    test1.flush();
    test1.close();

    std::ofstream test2("测试Files/子文件夹/test-sub.txt");
    test2 << "test file compression";
    test2.flush();
    test2.close();

    zipper.open();
    // add 测试Files folder
    zipper.add(L"测试Files\\");
    zipper.close();

    do {
      Unzipper unzipper(L"zip测试.zip");
      EXPECT_TRUE(unzipper.entries().size() == 5);

      MakeDir(GetCurrentPath() + L"/新建文件夹");

      unzipper.extract(GetCurrentPath() + L"/新建文件夹");

      std::vector<std::wstring> files = GetFilesFromDir(GetCurrentPath() + L"/新建文件夹");

      EXPECT_TRUE(IsFileExists(L"新建文件夹/测试Files/test文件1.txt"));
      EXPECT_TRUE(IsFileExists(L"新建文件夹/测试Files/test2.pdf"));
      EXPECT_TRUE(IsFileExists(L"新建文件夹/测试Files/子文件夹/test-sub.txt"));

      unzipper.close();
    } while (false);

    RemoveDir(L"测试Folder");
    RemoveDir(L"测试Files");
    RemoveDir(L"新建文件夹");
    std::remove("test文件1.txt");
  } while (false);

  std::remove("zip测试.zip");
}

TEST(MemoryZipTest, Test1) {
  std::stringstream output;
  Zipper zipper(output);

  std::stringstream strdata;
  strdata << "test string data compression";

  EXPECT_TRUE(zipper.add(strdata, L"str"));
  zipper.close();

  std::stringstream strdata2;
  Unzipper unzipper(output);
  EXPECT_TRUE(unzipper.extractEntryToStream(L"str", strdata2));
  unzipper.close();

  EXPECT_TRUE(strdata.str() == strdata2.str());
}

//TEST(FileUnzipTest, Test1) {
//  std::string s = "D:\\新建文件夹\\GogoWallapperResource\\937\\2000133704.zip";
//  Unzipper unzipper(s);
//  if (!unzipper.extract("D:\\123\\")) {
//
//  }
//}