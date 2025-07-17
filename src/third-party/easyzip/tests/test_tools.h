#pragma once

#include <istream>
#include <string>
#include <vector>


bool IsFileExists(const std::wstring &filename);
bool MakeDir(const std::wstring &newdir);
void RemoveDir(const std::wstring &foldername);
std::wstring GetParentDir(const std::wstring &filepath);
std::wstring GetCurrentPath();
bool IsDirectory(const std::wstring &path);
std::vector<std::wstring> GetFilesFromDir(const std::wstring &path);
std::wstring GetFileNameFromPath(const std::wstring &path);
std::wstring GetLastDirNameFromPath(const std::wstring &path); // the path must a dir