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

fs::path operator ""_p(const char* c, size_t s) {
    return fs::path(c);
}

fs::path operator ""_p(const char8_t* c, size_t s) {
    return fs::path(c);
}

file::file(const fs::path& path) : full_path_(path),
ui_format_16_str (ConvertPathToUiU16(path, 30, 60)),
path_size_ (path.native().length()) {}

fs::path file::parent_path() const {
    return full_path_.parent_path();
}

file::operator std::u16string() const {
    return full_path_.u16string();
}

file::operator std::string() const {
    return full_path_.string();
}


void Finder::FindAllFilesViaPath(const fs::path& input_path, FinderWarning& w) {

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

#ifdef _PROFILE
    for (const auto& dir : fs::directory_iterator(input_path)) {
        if (dir.is_directory()) {
            FindAllFilesViaPath(dir.path(), w);
            dir_count++;
            total_dir_str_weight += MeasurePathMemory(dir.path().filename());
        }
        else {
            files.insert({ dir.path().filename().u16string(),  dir.path() });
            total_files_weight += MeasurePathMemory(dir.path().filename());
            total_paths_weight += MeasurePathMemory(dir.path());
        }
    }
#endif
#ifndef _PROFILE
    for (const auto& dir : fs::directory_iterator(input_path)) {
        dir.is_directory() ? FindAllFilesViaPath(dir.path(), w) : 
            (void)files.insert( { dir.path().filename().u16string(),  dir.path()});
    }

    //for (const auto& dir : fs::directory_iterator(input_path)) {
    //    dir.is_directory() ? 
    //        FindAllFilesViaPath(dir.path(), w) :
    //        files.insert(dir.path());
    //}
#endif
}

void Finder::FindAllFilesViaPath(const fs::path& input_path) {
    FinderWarning w(FinderWarning::no_warnings);
    FindAllFilesViaPath(input_path, w);
}

format_file_map Finder::FindFilesBySubstring(const std::u16string& substring) const {
    format_file_map found_files;
    for (const auto& f : files) {
        if (f.first.find(substring) != f.first.npos) {
            found_files[&f.first] = &f.second.ui_format_16_str;
        }
    }
    return found_files;
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
            throw runtime_error("The openning directory by path wasn't successfull."s
                                "The error code "s + to_string(reinterpret_cast<intptr_t>(data.hInstApp)) );
        }
        w = FinderWarning::no_warnings;
        
        return;
    }
    
    w = FinderWarning::open_noexists_path;
}

void Finder::OpenDirectoryViaFileName(const std::u16string& file_name, FinderWarning& w) const {
    if (files.count(file_name)) {
        OpenDirectory(files.at(file_name).parent_path(), w);
        return;
    }

    w = FinderWarning::open_noexists_file_name;
}

void Finder::OpenDirectoryViaFileName(const std::u16string& file_name) const {
    FinderWarning w = FinderWarning::no_warnings;
    OpenDirectoryViaFileName(file_name, w);
}

#ifdef _DEBUG
bool Finder::isFileExist(std::u16string str_file) {
    return files.count(str_file);
}
bool Finder::IsFilePath(std::u16string file_name, std::u16string file_full_path) {
    file f = files.at(file_name);
    return f.full_path_ == file_full_path;
}
#endif // _DEBUG

PathsMap::PathsMap() : base_path_(CURRENT_PATH) {
    name_ids_.reserve(500'000);
    id_set_.reserve(500'000);
}

void PathsMap::insert(const fs::path& path) {
    insert(path, distance(path.begin(), path.end()));
}

void PathsMap::insert(const fs::path& path, int8_t path_len) {
    ArrayPtr<int32_t> path_ids(path_len);

    short i = 0;
    int32_t last_used_id = 0;
    for (const auto& name : path) {
        if (!name_ids_.count(name.u16string())) {
            auto it = name_ids_.insert({ name.u16string(), last_id_ });
            names_.push_back(&it.first->first);
            last_used_id = last_id_++;
            path_ids[i++] = last_used_id;
            continue;
        }
        last_used_id = name_ids_.at(name.u16string());
        path_ids[i++] = last_used_id;
    }

    id_set_[*names_.at(last_used_id)] = move(path_ids);
}

fs::path PathsMap::at(const std::u16string& key) const {
    const ArrayPtr<int32_t>& ids_set = id_set_.at(key);
    fs::path result_path = base_path_;
    for (uint8_t i = 0; i < ids_set.size(); i++) {
        result_path /= fs::path(*names_.at(ids_set[i]));
    }

    return result_path;
}

void PathsMap::SetBasePath(const fs::path& path) {
    base_path_ = path;
}

size_t PathsMap::count(const std::u16string str) const {
    return id_set_.count(str);
}

PathsMap::Iterator PathsMap::begin() {
    return Iterator(names_.begin());
}

PathsMap::Iterator PathsMap::begin() const {
    return Iterator(names_.begin());
}

const PathsMap::Iterator PathsMap::cbegin() const {
    return Iterator(names_.cbegin());
}

PathsMap::Iterator PathsMap::end() {
    return Iterator(names_.end());
}

PathsMap::Iterator PathsMap::end() const {
    return Iterator(names_.end());
}

const PathsMap::Iterator PathsMap::cend() const {
    return Iterator(names_.cend());
}

PathsMap::Iterator::Iterator(std::vector<const std::u16string*>::iterator it) : vec_it_(*it) {}

PathsMap::Iterator::Iterator(std::vector<const std::u16string*>::const_iterator it) : vec_it_(*it) {}

PathsMap::Iterator::Iterator(const std::u16string* raw_ptr) : vec_it_(raw_ptr) {}

PathsMap::Iterator PathsMap::Iterator::operator++() { vec_it_++; return *this; }

PathsMap::Iterator PathsMap::Iterator::operator++(int) { 
    Iterator temp = *this; vec_it_++; return temp; 
}

PathsMap::Iterator PathsMap::Iterator::operator--() { vec_it_--; return *this; }

PathsMap::Iterator PathsMap::Iterator::operator--(int) { 
    Iterator temp = *this; vec_it_--; return temp; 
}

//PathsMap::Iterator PathsMap::Iterator::operator+(const Iterator& other) const {
//    Iterator temp(vec_it_ + other.vec_it_); return temp; 
//}
//
//PathsMap::Iterator PathsMap::Iterator::operator-(const Iterator& other) const {
//    Iterator temp(vec_it_ - other.vec_it_); return temp;
//}

bool PathsMap::Iterator::operator==(Iterator other) const { return vec_it_ == other.vec_it_; }

bool PathsMap::Iterator::operator!=(Iterator other) const { return !(*this == other); }

PathsMap::Iterator::reference PathsMap::Iterator::operator*() const { return *vec_it_; }
