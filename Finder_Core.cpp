#include <iostream>
#include "Finder_Core.h"
#include"Finder_Tests.h"
#include"WinInOut.h"

using namespace std;

u16string ConvertPathToUiU16(const fs::path& path, int file_max_len, int path_max_len) {
    u16string format_str(path.filename().u16string());
    u16string dir_str = path.parent_path().u16string();

    if (format_str.size() > file_max_len) {
        format_str.resize(file_max_len - 3);
        format_str += u"..." + u16string(5, u' ');
    }
    else {
        format_str += u16string(file_max_len + 5 - format_str.size(), u' ');
    }

    if (dir_str.size() > path_max_len) {
        format_str += u":\t" +
            u16string(dir_str.begin(),
                dir_str.begin() + path_max_len / 2 - 5) +
            u" ... " +
            u16string(dir_str.end() - path_max_len / 2 - 5,
                dir_str.end());

        return format_str;
    }

    format_str += u":\t" + dir_str;
    return format_str;
}

wchar_t* GetNativePathFileWStr(const fs::path& path) {
    wchar_t* wchar_ptr = const_cast<wchar_t*>(path.c_str() + path.native().size());
    while (*wchar_ptr != L'\\' && *wchar_ptr != L'/') {
        wchar_ptr--;
    }

    return wchar_ptr;
}

char16_t* GetNativePathFileU16Str(const fs::path& path) {
    return reinterpret_cast<char16_t*>(GetNativePathFileWStr(path));
}

const char16_t* GetNativeDirsToFileU16Str(const fs::path& path) {
    const char16_t* str_ptr = reinterpret_cast<const char16_t*>(path.c_str());
    char16_t* str_ptr_end = const_cast<char16_t*>(str_ptr + path.native().size());
    while (*str_ptr_end != L'\\' && *str_ptr_end != L'/') {
        str_ptr_end--;
    }

    *str_ptr_end = L'\0';

    return str_ptr;
}

fs::path operator ""_p(const char* c, size_t s) {
    return fs::path(c);
}

fs::path operator ""_p(const char8_t* c, size_t s) {
    return fs::path(c);
}

file::file(const fs::path& path) : full_path_(0) {}
file::file(const std::u16string& path) : full_path_(0) {}
file::file(const std::wstring& path) : full_path_(0) {}
file::file(const char16_t* path) : full_path_(0) {}
file::file(const wchar_t* path) : full_path_(0) {}

file::file(uint16_t id) : full_path_(id) {}

fs::path file::parent_path() const {
    return {};
}

const fs::path& file::path() const {
    return {};
}

file::operator std::u16string() const {
    return {};
}

file::operator std::string() const {
    return {};
}


Finder::Finder() : base_path_(fs::current_path()) {
    files_paths_id_.reserve(4'000'000);
    file_names_.reserve(4'000'000);
    paths_.reserve(1'500);
}

void Finder::FindAllFilesViaPath(const fs::path& input_path, FinderWarning& w, uint16_t path_id) {

    if (input_path.native().length() >= MAX_PATH) {
        w = FinderWarning::path_limit;
        not_used_paths_.push_back(input_path);
        return;
    }

    if (!fs::directory_entry(input_path).exists()) {
        w = FinderWarning::paths_noexists;
        not_existing_paths_.push_back(input_path);
        return;
    }

#ifndef _PROFILER
    for (const auto& dir : fs::directory_iterator(input_path)) {
        if (dir.is_directory()) {
            paths_.push_back(dir.path());
            FindAllFilesViaPath(dir.path(), w, paths_.size() - 1);
            continue;
        }
        PushBackFileName(dir.path(), path_id);
    }
#endif
}

void Finder::FindAllFilesViaPath(const fs::path& input_path, FinderWarning& w) {
    paths_.push_back(input_path);
    FindAllFilesViaPath(input_path, w, 0);
}

void Finder::FindAllFilesViaPath(const fs::path& input_path) {
    FinderWarning w(FinderWarning::no_warnings);
    paths_.push_back(input_path);
    FindAllFilesViaPath(input_path, w, 0);
}

format_file_map Finder::FindFilesBySubstring(const std::u16string& substring, size_t count) const {
    format_file_map found_files;

    uint32_t found_count = 0;
    for (size_t id = 0; id < file_names_.size(); id++) {
        if (file_names_[id].find(substring) != file_names_[id].npos) {
            found_files[&file_names_[id]] = &GetPathByFileId(id);
            found_count++;
        }
        if (found_count >= count) { break; }
    }

    return found_files;
}

format_file_map Finder::FindFilesBySubstring(const std::u16string& substring) const {
    return FindFilesBySubstring(substring, files_count());
}

u16string Finder::GetWarning(FinderWarning w) const {
    switch (w) {
    case FinderWarning::path_limit:
    {
        u16string w_str = u"Warning! The maximum path length was exceeded! That paths weren't use:\n\n"s;
        for (const auto& path : not_used_paths_) {
            w_str += path.u16string() + u'\n';
        }
        return w_str;
    }
    case FinderWarning::paths_noexists:
    {
        u16string w_str = u"Warning! The next paths are not exists and were skipped:\n\n"s;
        for (const auto& path : not_existing_paths_) {
            w_str += path.u16string() + u'\n';
        }
        return w_str;
    }
    case FinderWarning::open_noexists_path:
    {
        return u"Warning! This path is not exists! Opening is not executed.\n\n"s;
    }
    case FinderWarning::open_noexists_file_name:
    {
        return u"Warning! File with this name is not exists! Opening is not executed.\n\n"s;
    }
    default:
        return u"No warnings";
    }
}

void Finder::OpenDirectory(const fs::path& path) const {
    FinderWarning w = FinderWarning::no_warnings;
    OpenDirectory(path, w);
}

void Finder::OpenDirectory(const fs::path& path, FinderWarning& w) const {
    if (fs::directory_entry(path).exists()) {
        auto path_str = path.wstring();
        
        SHELLEXECUTEINFOW data;
        data.cbSize = sizeof(SHELLEXECUTEINFOW);
        data.fMask = SEE_MASK_FLAG_NO_UI;
        data.hwnd = nullptr;
        data.lpVerb = L"explore";
        data.lpFile = path_str.c_str();
        data.lpParameters = nullptr;
        data.lpDirectory = nullptr;
        data.nShow = SW_SHOWDEFAULT;
        
        bool result = ShellExecuteExW(&data);
        
        if (!result) {
            throw runtime_error("The openning directory by path wasn't successfull. "s
                                "Error code "s + to_string(reinterpret_cast<intptr_t>(data.hInstApp)) );
        }
        w = FinderWarning::no_warnings;
        
        return;
    }
    
    w = FinderWarning::open_noexists_path;
}

void Finder::OpenDirectoryViaFileName(const std::u16string& file_name, FinderWarning& w) const {
    auto it = find(file_names_.begin(), file_names_.end(), file_name);
    if (it != file_names_.end()) {
        uint32_t id = distance(file_names_.begin(), it);
        OpenDirectory(GetPathByFileId(id), w);
        return;
    }

    w = FinderWarning::open_noexists_file_name;
}

void Finder::OpenDirectoryViaFileName(const std::u16string& file_name) const {
    FinderWarning w = FinderWarning::no_warnings;
    OpenDirectoryViaFileName(file_name, w);
}

const fs::path& Finder::GetPathByFileId(uint32_t file_id) const {
    return paths_[files_paths_id_[file_id]];
}

#ifdef _DEBUG
bool Finder::isFileExist(const u16string& str_file) {
    auto it = find(file_names_.begin(), file_names_.end(), str_file);
    return it != file_names_.end();
}
bool Finder::IsFilePath(const u16string& file_name, const u16string& file_full_path) {
    auto vec = find_all(file_names_.begin(), file_names_.end(), file_name);
    for (auto& it : vec) {
        uint16_t id = distance(file_names_.begin(), it);
        if ((GetPathByFileId(id) / file_name) == file_full_path) { return true; }
    }
    return false;
}
#endif // _DEBUG