#pragma once

#include <istream>
#include <string>
#include <vector>

namespace easyzip {
void GetFileCrc(std::istream& input_stream, std::vector<char>& buff, unsigned long& result_crc);
bool IsLargeFile(std::istream& input_stream);
bool IsFileExists(const std::wstring& filename);
bool MakeDir(const std::wstring& newdir);
void RemoveDir(const std::wstring& foldername);
std::wstring GetParentDir(const std::wstring& filepath);
std::wstring GetCurrentPath();
bool IsDirectory(const std::wstring& path);
std::vector<std::wstring> GetFilesFromDir(const std::wstring& path);
std::wstring GetFileNameFromPath(const std::wstring& path);
std::wstring GetLastDirNameFromPath(const std::wstring& path); // the path must a dir

std::wstring TryToUnicode(const std::string& src);
std::string TryFromUnicode(const std::wstring& src);
}  // namespace easyzip
