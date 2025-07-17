#include "test_tools.h"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <assert.h>

#include "../src/filesystem.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace easyzip;

bool IsFileExists(const std::wstring &filename) {
  std::error_code ec;
  return filesystem::exists(filename, ec);
}

bool MakeDir(const std::wstring &newdir) {
  std::error_code ec;
  return filesystem::create_directories(newdir, ec);
}

void RemoveDir(const std::wstring &foldername) {
  std::error_code ec;
  filesystem::remove_all(foldername, ec);
}

bool IsDirectory(const std::wstring &path) {
  std::error_code ec;
  return filesystem::is_directory(path, ec);
}

std::wstring GetParentDir(const std::wstring &filepath) {
  filesystem::path p(filepath);
  return p.parent_path().wstring();
}

std::wstring GetCurrentPath() { return filesystem::current_path().wstring(); }

std::vector<std::wstring> GetFilesFromDir(const std::wstring &path) {
  std::vector<std::wstring> v;
  filesystem::recursive_directory_iterator itr(path);
  for (itr = filesystem::begin(itr); itr != filesystem::end(itr); itr++) {
    if (!filesystem::is_directory(itr->path())) {
      v.push_back(itr->path().wstring());
    }
  }

  return v;
}

std::wstring GetFileNameFromPath(const std::wstring &fullPath) {
  filesystem::path p(fullPath);
  return p.filename().wstring();
}

std::wstring GetLastDirNameFromPath(const std::wstring &path) {
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
