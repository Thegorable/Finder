#pragma once
#include<filesystem>
#include<map>
#include<unordered_map>
#include<set>
#include<string>
#include<utility>
#include<Windows.h>
#include"Time.h"
#include"array_ptr.h"

//#define _PROFILE
#define _DEBUG_INFO

namespace fs = std::filesystem;

const fs::path CURRENT_PATH = fs::current_path();

fs::path operator ""_p(const char* c, size_t s);
fs::path operator ""_p(const char8_t* c, size_t s);

std::u16string ConvertPathToUiU16(const fs::path& path, int file_max_len, int path_max_len);
wchar_t* GetNativePathFileWStr(const fs::path& path);
char16_t* GetNativePathFileU16Str(const fs::path& path);
const char16_t* GetNativeDirsToFileU16Str(const fs::path& path);

template<typename it, typename value>
std::vector<it> find_all(it l, it r, value v) {
    std::vector<it> vec;

    while (l != r) {
        l = std::find(l, r, v);
        if (l == r) {
            break;
        }
        vec.push_back(l);
        l++;
    }

    return vec;
}

struct file {
    file(const fs::path& path);
    file(const std::u16string& path);
    file(const std::wstring& path);
    file(const char16_t* path);
    file(const wchar_t* path);
    file(uint16_t id);

    fs::path parent_path() const;
    const fs::path& path() const;
    operator std::u16string() const;
    operator std::string() const;

private:
    uint16_t full_path_;
};

typedef std::map<const std::u16string*, const fs::path*> format_file_map;
typedef std::unordered_map<uint32_t, file> files_map;

enum class FinderWarning {path_limit, paths_noexists, open_noexists_path, open_noexists_file_name, no_warnings};

class Finder {
public:
    Finder();

    void FindAllFilesViaPath(const fs::path& input_path, FinderWarning& w);
    void FindAllFilesViaPath(const fs::path& input_path);
    format_file_map FindFilesBySubstring(const std::u16string& str, size_t count) const;
    format_file_map FindFilesBySubstring(const std::u16string& str) const;
    inline size_t files_count() const noexcept { return file_names_.size(); }
    inline size_t folders_count() const noexcept { return paths_.size(); }
    std::u16string GetWarning(FinderWarning w) const;
    void OpenDirectory(const fs::path& path) const;
    void OpenDirectory(const fs::path& path, FinderWarning& w) const;
    void OpenDirectoryViaFileName(const std::u16string& file_name, FinderWarning& w) const;
    void OpenDirectoryViaFileName(const std::u16string& file_name) const;

private:  
    void FindAllFilesViaPath(const fs::path& input_path, FinderWarning& w, uint16_t path_id);
    inline void PushBackFileName(const std::u16string& name, uint16_t path_id) {
        file_names_.push_back(name);
        files_paths_id_.push_back(static_cast<uint16_t>(path_id));
    }
    inline void PushBackFileName(const fs::path& full_path, uint16_t path_id) {
        file_names_.push_back(full_path.filename().u16string());
        files_paths_id_.push_back(static_cast<uint16_t>(path_id));
    }

    const fs::path& GetPathByFileId(uint32_t file_id) const;
    
    fs::path base_path_;
    files_map files;
    std::vector<fs::path> paths_;
    std::vector<std::u16string> file_names_;
    std::vector<uint32_t> files_paths_id_;
    std::vector<fs::path> not_used_paths_;
    std::vector<fs::path> not_existing_paths_;

#ifdef _PROFILE
public:
    size_t dir_count = 0;
    size_t inserted_files_count = 0;
    size_t not_inserted_files_count = 0;
    size_t total_dir_str_weight = 0;
    size_t total_files_weight = 0;
    size_t total_inserted_files_weight = 0;
    size_t total_paths_weight = 0;
#endif

#ifdef _DEBUG
public:
    bool isFileExist(const std::u16string& str_file);
    bool IsFilePath(const std::u16string& file_name, const std::u16string& file_full_path);

    friend void TestSimpleFindAllFilesViaPath();
    friend void TestHardFindAllFilesViaPath();
    friend void TestSimpleRusFindAllFilesViaPath();
    friend void TestDataSize(fs::path path);
#endif
};