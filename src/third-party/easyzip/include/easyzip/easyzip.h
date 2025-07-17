#ifndef EASYZIP_H_
#define EASYZIP_H_
#pragma once

#include <ctime>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <istream>
#include <map>

#ifdef EASYZIP_STATIC
#define EASYZIP_API
#else
#if defined(EASYZIP_EXPORTS)
#if defined(_MSC_VER)
#define EASYZIP_API __declspec(dllexport)
#define EXPIMP_TEMPLATE
#else
#define EASYZIP_API
#endif
#else
#if defined(_MSC_VER)
#define EASYZIP_API __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#else
#define EASYZIP_API
#endif
#endif
#endif


namespace easyzip {
class EASYZIP_API Zipper {
 public:
  enum zipFlags {
    Overwrite = 0x01,  // -o
    Append = 0x02,     // -a
    Store = 0x04,      // -0
    Faster = 0x08,     // -1
    Better = 0x10,     // -9
    NoPaths = 0x20,    // -j
    SaveHierarchy = 0x40,
  };
  Zipper(std::iostream& buffer);
  Zipper(std::vector<unsigned char>& buffer);
  Zipper(const std::wstring& zipname);
  Zipper(const std::wstring& zipname, const std::wstring& password);

  ~Zipper();

  bool add(std::istream& source,
           const std::tm& timestamp,
           const std::wstring& nameInZip,
           zipFlags flags = Better);
  bool add(std::istream& source, const std::wstring& nameInZip, zipFlags flags = Better);
  bool add(const std::wstring& fileOrFolderPath, zipFlags flags = Better, bool unixSeparator = false);

  void open();
  void close();

 private:
  void release();

  std::wstring m_password;
  std::wstring m_zipname;
  std::iostream& m_obuffer;
  std::vector<unsigned char>& m_vecbuffer;
  bool m_usingMemoryVector;
  bool m_usingStream;
  bool m_open;

  struct Impl;
  Impl* m_impl;
};

class ZipEntry;

class EASYZIP_API Unzipper {
 public:
  Unzipper(std::istream& buffer);
  Unzipper(std::vector<unsigned char>& buffer);
  Unzipper(const std::wstring& zipname);
  Unzipper(const std::wstring& zipname, const std::wstring& password);

  ~Unzipper();

  std::vector<ZipEntry> entries();

  bool extract(const std::wstring& destination,
               const std::map<std::wstring, std::wstring>& alternativeNames);
  bool extract(const std::wstring& destination = std::wstring());
  bool extractEntry(const std::wstring& name, const std::wstring& destination = std::wstring());
  bool extractEntryToStream(const std::wstring& name, std::ostream& stream);
  bool extractEntryToMemory(const std::wstring& name, std::vector<unsigned char>& vec);

  void close();

 private:
  void release();
  std::wstring m_password;
  std::wstring m_zipname;
  std::istream& m_ibuffer;
  std::vector<unsigned char>& m_vecbuffer;
  bool m_usingMemoryVector;
  bool m_usingStream;
  bool m_open;

  struct Impl;
  Impl* m_impl;
};

class EASYZIP_API ZipEntry {
 private:
  typedef struct {
    unsigned int tm_sec;
    unsigned int tm_min;
    unsigned int tm_hour;
    unsigned int tm_mday;
    unsigned int tm_mon;
    unsigned int tm_year;
  } tm_s;

 public:
  ZipEntry(const std::wstring& name,
           unsigned long long int compressed_size,
           unsigned long long int uncompressed_size,
           int year,
           int month,
           int day,
           int hour,
           int minute,
           int second,
           unsigned long dosdate)
      : name(name)
      , compressedSize(compressed_size)
      , uncompressedSize(uncompressed_size)
      , dosdate(dosdate) {
    // timestamp YYYY-MM-DD HH:MM:SS
    std::wstringstream str;
    str << year << L"-" << month << L"-" << day << L" " << hour << L":" << minute << L":" << second;
    timestamp = str.str();

    unixdate.tm_year = year;
    unixdate.tm_mon = month;
    unixdate.tm_mday = day;
    unixdate.tm_hour = hour;
    unixdate.tm_min = minute;
    unixdate.tm_sec = second;
  }

  bool valid() { return !name.empty(); }

  std::wstring name, timestamp;
  unsigned long long int compressedSize, uncompressedSize;
  unsigned long dosdate;
  tm_s unixdate;
};
}  // namespace easyzip

#endif  // !EASYZIP_H_