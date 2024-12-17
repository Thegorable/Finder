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

#define _PROFILE

namespace fs = std::filesystem;

const fs::path CURRENT_PATH = fs::current_path();

fs::path operator ""_p(const char* c, size_t s);
fs::path operator ""_p(const char8_t* c, size_t s);

std::u16string ConvertPathToUiU16(const fs::path& path, int file_max_len, int path_max_len);

struct file {
    file(const fs::path& path);

    fs::path parent_path() const;
    operator std::u16string() const;
    operator std::string() const;

    const fs::path full_path_;
    const std::u16string ui_format_16_str;
    const std::size_t path_size_;
};

typedef std::map<const std::u16string*, const std::u16string*> format_file_map;
typedef std::unordered_map<std::u16string, file> files_map;

enum class FinderWarning {path_limit, paths_noexists, open_noexists_path, open_noexists_file_name, no_warnings};

class PathsMap {
public:
    class Iterator {
        friend class PathsMap;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = std::u16string;
        using pointer = const std::u16string*;
        using reference = const std::u16string&;
        using iterator_category = std::random_access_iterator_tag;

        Iterator(std::vector<const std::u16string*>::iterator it);
        Iterator(std::vector<const std::u16string*>::const_iterator it);
        Iterator(const std::u16string* raw_ptr);
        Iterator operator++();
        Iterator operator++(int);
        Iterator operator--();
        Iterator operator--(int);
        //Iterator operator+(const Iterator& other) const;
        //Iterator operator-(const Iterator& other) const;
        bool operator==(Iterator other) const;
        bool operator!=(Iterator other) const;
        reference operator*() const;

    private:
        pointer vec_it_;
    };
    
    PathsMap();

    void insert(const fs::path& path);
    void insert(const fs::path& path, int8_t path_len);
    fs::path at(const std::u16string& key) const;
    void SetBasePath(const fs::path& path);
    inline size_t size() const noexcept { return id_set_.size(); }
    size_t count(const std::u16string str) const;
    
    Iterator begin();
    Iterator begin() const;
    const Iterator cbegin() const;
    Iterator end();
    Iterator end() const;
    const Iterator cend() const;

private:
    fs::path base_path_;
    std::vector<const std::u16string*> names_;
    std::unordered_map <std::u16string, int32_t> name_ids_;
    std::unordered_map <std::u16string_view, ArrayPtr<int32_t> > id_set_;

    int32_t last_id_ = 0;

#ifdef _DEBUG
    friend void TestPathsMapInsert();
#endif // _DEBUG

};

class Finder {
public:
    void FindAllFilesViaPath(const fs::path& input_path, FinderWarning& w);
    void FindAllFilesViaPath(const fs::path& input_path);
    format_file_map FindFilesBySubstring(const std::u16string& str) const;
    inline size_t files_count() const noexcept { return files.size(); }
    std::u16string GetWarning(FinderWarning w) const;
    void OpenDirectory(const fs::path& path) const;
    void OpenDirectory(const fs::path& path, FinderWarning& w) const;
    void OpenDirectoryViaFileName(const std::u16string& file_name, FinderWarning& w) const;
    void OpenDirectoryViaFileName(const std::u16string& file_name) const;

private:  
    files_map files;
    //PathsMap files;
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

    bool isFileExist(std::u16string str_file);
    bool IsFilePath(std::u16string file_name, std::u16string file_full_path);

    friend void TestSimpleFindAllFilesViaPath();
    friend void TestHardFindAllFilesViaPath();
    friend void TestSimpleRusFindAllFilesViaPath();
    friend void TestDataSize(fs::path path);
#endif
};

void FindFilesViaConsoleTest(const fs::path current_path = CURRENT_PATH);