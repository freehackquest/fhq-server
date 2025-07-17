#include "tools.h"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <assert.h>
#include <locale>
#include <codecvt>
#include "defs.h"
#include "filesystem.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

namespace easyzip {
/* calculate the CRC32 of a file, because to encrypt a file, we need known the CRC32 of the file before */
void GetFileCrc(std::istream& input_stream, std::vector<char>& buff, unsigned long& result_crc) {
  unsigned long calculate_crc = 0;
  unsigned long size_read = 0;
  unsigned long total_read = 0;

  do {
    input_stream.read(buff.data(), buff.size());
    size_read = (unsigned long)input_stream.gcount();

    if (size_read > 0)
      calculate_crc = crc32(calculate_crc, (const unsigned char*)buff.data(), size_read);

    total_read += size_read;

  } while (size_read > 0);

  input_stream.clear();
  input_stream.seekg(0, std::ios_base::beg);
  result_crc = calculate_crc;
}

bool IsLargeFile(std::istream& input_stream) {
  ZPOS64_T pos = 0;
  input_stream.seekg(0, std::ios::end);
  pos = input_stream.tellg();
  input_stream.seekg(0);

  return pos >= 0xffffffff;
}

bool IsFileExists(const std::wstring& filename) {
  std::error_code ec;
  return filesystem::exists(filename, ec);
}

bool MakeDir(const std::wstring& newdir) {
  std::error_code ec;
  return filesystem::create_directories(newdir, ec);
}

void RemoveDir(const std::wstring& foldername) {
  std::error_code ec;
  filesystem::remove_all(foldername, ec);
}

bool IsDirectory(const std::wstring& path) {
  std::error_code ec;
  return filesystem::is_directory(path, ec);
}

std::wstring GetParentDir(const std::wstring& filepath) {
  filesystem::path p(filepath);
  return p.parent_path().wstring();
}

std::wstring GetCurrentPath() {
  return filesystem::current_path().wstring();
}

std::vector<std::wstring> GetFilesFromDir(const std::wstring& path) {
  std::vector<std::wstring> v;
  filesystem::recursive_directory_iterator itr(path);
  for (itr = filesystem::begin(itr); itr != filesystem::end(itr); itr++) {
    if (!filesystem::is_directory(itr->path())) {
      v.push_back(itr->path().wstring());
    }
  }

  return v;
}

std::wstring GetFileNameFromPath(const std::wstring& fullPath) {
  filesystem::path p(fullPath);
  return p.filename().wstring();
}

std::wstring GetLastDirNameFromPath(const std::wstring& path) {
  if (path.length() == 0)
    return L"";
  if (path.length() == 1 && path == L".")
    return L"";
  if (path.length() == 2 && path == L"..")
    return L"";

  assert(IsDirectory(path));

  std::wstring path2 = path;

#ifdef _WIN32
  if (path2[path2.length() - 1] == L'\\') {
    path2 = path2.substr(0, path2.length() - 1);
  }
#else
  if (path2[path2.length() - 1] == L'/') {
    path2 = path2.substr(0, path2.length() - 1);
  }
#endif

  filesystem::path p(path2);
  return p.filename().wstring();
}

#ifdef _WIN32
std::string UnicodeToAnsi(const std::wstring& str, unsigned int code_page /*= 0*/) {
  std::string strRes;
  int iSize = ::WideCharToMultiByte(code_page, 0, str.c_str(), -1, NULL, 0, NULL, NULL);

  if (iSize == 0)
    return strRes;

  char* szBuf = new (std::nothrow) char[iSize];

  if (!szBuf)
    return strRes;

  memset(szBuf, 0, iSize);

  ::WideCharToMultiByte(code_page, 0, str.c_str(), -1, szBuf, iSize, NULL, NULL);

  strRes = szBuf;
  delete[] szBuf;

  return strRes;
}

std::wstring AnsiToUnicode(const std::string& str, unsigned int code_page /*= 0*/) {
  std::wstring strRes;

  int iSize = ::MultiByteToWideChar(code_page, 0, str.c_str(), -1, NULL, 0);

  if (iSize == 0)
    return strRes;

  wchar_t* szBuf = new (std::nothrow) wchar_t[iSize];

  if (!szBuf)
    return strRes;

  memset(szBuf, 0, iSize * sizeof(wchar_t));

  ::MultiByteToWideChar(code_page, 0, str.c_str(), -1, szBuf, iSize);

  strRes = szBuf;
  delete[] szBuf;

  return strRes;
}

std::string UnicodeToUtf8(const std::wstring& str) {
  std::string strRes;

  int iSize = ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);

  if (iSize == 0)
    return strRes;

  char* szBuf = new (std::nothrow) char[iSize];

  if (!szBuf)
    return strRes;

  memset(szBuf, 0, iSize);

  ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, szBuf, iSize, NULL, NULL);

  strRes = szBuf;
  delete[] szBuf;

  return strRes;
}

std::wstring Utf8ToUnicode(const std::string& str) {
  std::wstring strRes;
  int iSize = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

  if (iSize == 0)
    return strRes;

  wchar_t* szBuf = new (std::nothrow) wchar_t[iSize];

  if (!szBuf)
    return strRes;

  memset(szBuf, 0, iSize * sizeof(wchar_t));
  ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, szBuf, iSize);

  strRes = szBuf;
  delete[] szBuf;

  return strRes;
}

std::string AnsiToUtf8(const std::string& str, unsigned int code_page /*= 0*/) {
  return UnicodeToUtf8(AnsiToUnicode(str, code_page));
}

std::string Utf8ToAnsi(const std::string& str, unsigned int code_page /*= 0*/) {
  return UnicodeToAnsi(Utf8ToUnicode(str), code_page);
}

#endif

#if _MSC_VER >= 1900

std::string utf16_to_utf8(std::wstring utf16_string) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
  auto p = reinterpret_cast<const wchar_t*>(utf16_string.data());
  return convert.to_bytes(p, p + utf16_string.size());
}

std::wstring utf8_to_utf16(std::string utf8_string) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
  auto p = reinterpret_cast<const char*>(utf8_string.data());
  auto str = convert.from_bytes(p, p + utf8_string.size());
  std::wstring u16_str(str.begin(), str.end());
  return u16_str;
}
#else

std::string utf16_to_utf8(std::wstring utf16_string) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
  return convert.to_bytes(utf16_string);
}

std::wstring utf8_to_utf16(std::string utf8_string) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
  return convert.from_bytes(utf8_string);
}
#endif

std::wstring TryToUnicode(const std::string& src) {
#ifdef _WIN32
  return AnsiToUnicode(src, 0);
#else
  return utf8_to_utf16(src);
#endif
}

std::string TryFromUnicode(const std::wstring& src) {
#ifdef _WIN32
  return UnicodeToAnsi(src, 0);
#else
  return utf16_to_utf8(src);
#endif
}

}  // namespace easyzip
