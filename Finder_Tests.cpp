﻿#pragma once
#include "Finder_Tests.h"
#include <fstream>
#include<iostream>
#include<cassert>
#include<functional>
#include<utility>
#include"WinInOut.h"

using namespace std;
using namespace std::literals::chrono_literals;

long long countDigits(long long number) {
    if (number == 0) return 1;
    return log10(abs(number)) + 1;
}

std::string GenerateRandomNumberStr(int length) {
    std::string random_numbers(length, '0');
    std::generate(random_numbers.begin(), random_numbers.end(), []() {
        return (char)(rand() % 9) + 48; });

    return random_numbers;
}

int FilesGenerator(fs::path input_path, 
    int start_count, 
    int end_count, 
    int max_level, 
    int current_level, 
    int files_max) {
    
    static int total_files_count = 0;
    if (total_files_count >= files_max) {
        return total_files_count;
    }
    int folders_count = end_count;
    int files_count = files_max / (pow(folders_count, max_level));
    if (files_max == 0) {
        files_count = end_count;
    }

    if (!fs::directory_entry(input_path).exists()) {
        fs::remove(input_path);
    }
    
    long long digits = countDigits(files_count);
    
    fs::create_directory(input_path);

    for (int folder_num = 1; folder_num <= folders_count; folder_num++) {
        std::string random_number = GenerateRandomNumberStr(digits);
        std::string folder_name = "folder_"s + random_number + "_"s + std::to_string(folder_num);
        fs::path folder_path = input_path / fs::path(folder_name);

        fs::create_directory(folder_path);

        if (current_level < max_level) {
            FilesGenerator(folder_path, start_count, end_count, max_level, current_level + 1, files_max);
        }
    }

    for (int file_num = 1; file_num <= files_count; file_num++) {
        std::string random_number = GenerateRandomNumberStr(digits);
        std::string file_name = "file_"s + random_number + "_"s + std::to_string(file_num) + ".txt"s;
        fs::path file_path = input_path / fs::path(file_name);

        std::ofstream test_file(file_path);
        total_files_count++;
    }

    return total_files_count;
}

void GenerateHardTestFiles() {
    try {
        int new_files = FilesGenerator(HARD_TEST_PATH, 1, 2, 15, 1);
        std::cout << new_files << " files were generated\n";
    }
    catch (fs::filesystem_error e) {
        std::cout << e.what() << '\n';
    }
}

void GenerateBenchmarkTestFiles() {
    try {
        int new_files = FilesGenerator(BENCHMARK_TEST_PATH_MID, 1, 4, 5, 1, 1100000);
        std::cout << new_files << " files were generated\n";
    }
    catch (fs::filesystem_error e) {
        std::cout << e.what() << '\n';
    }
}

#ifdef _PROFILE

void TestDataSize(fs::path path) {
    cout << "Testing...\n\n";

    Finder finder;
    finder.FindAllFilesViaPath(path);
    size_t total_full_path_w = 0;

    cout << "Inserted files: " << finder.inserted_files_count << '\n';
    cout << "Not inserted files: " << finder.not_inserted_files_count << '\n';
    cout << "Not used files: " << finder.not_used_paths_.size() << '\n';
    cout << "Not existing files: " << finder.not_existing_paths_.size() << '\n';
    cout << "Total folders: " << finder.dir_count << "\n\n";
    
    cout << "Origin inserted files total weight:\t" << finder.total_inserted_files_weight << " bytes"
        << " = " << finder.total_inserted_files_weight / 1024 << " kb "
        << " = " << finder.total_inserted_files_weight / 1024 / 1024 << " mb " << '\n';

    cout << "Origin all files total weight:\t\t" << finder.total_files_weight << " bytes"
        << " = " << finder.total_files_weight / 1024 << " kb "
        << " = " << finder.total_files_weight / 1024 / 1024 << " mb " << '\n';

    cout << "Origin all folders total weight:\t" << finder.total_dir_str_weight << " bytes"
        << " = " << finder.total_dir_str_weight / 1024 << " kb "
        << " = " << finder.total_dir_str_weight / 1024 / 1024 << " mb " << '\n';

    //for (const auto& f : finder.files) {
    //    total_full_path_w += MeasurePathMemory(finder.files.at(f.first).full_path_);
    //}

    cout << "Total insered full paths weight:\t" << total_full_path_w << " bytes"
        << " = " << total_full_path_w / 1024 << " kb "
        << " = " << total_full_path_w / 1024 / 1024 << " mb " << '\n';

    cout << "Total full paths weight:\t\t" << finder.total_paths_weight << " bytes"
        << " = " << finder.total_paths_weight / 1024 << " kb "
        << " = " << finder.total_paths_weight / 1024 / 1024 << " mb " << '\n';
}

size_t MeasurePathMemory(const fs::path& p) {
    size_t total_size = sizeof(p);
    const auto& native_str = p.native();

    if constexpr (std::is_same_v<decltype(native_str), const std::string&>) {
        total_size += native_str.capacity();
    }
    else if constexpr (std::is_same_v<decltype(native_str), const std::wstring&>) {
        total_size += native_str.capacity() * sizeof(wchar_t);
    }

    return total_size;
}

#endif // _PROFILE

#ifdef _DEBUG
void TestSimpleFindAllFilesViaPath() {
    Finder finder;
    finder.FindAllFilesViaPath(SIMPLE_TEST_PATH);

    assert(finder.files_count() == 11);

    assert(finder.isFileExist(u"root_file_1.txt"));
    assert(finder.IsFilePath(u"root_file_1.txt", 
        uR"(E:\Programming\Training_projects\C++\Finder\findFiles_test\root_file_1.txt)"));
    assert(finder.isFileExist(u"folder_1_1_file_2.txt"));
    assert(finder.IsFilePath(u"folder_1_1_file_2.txt",
        uR"(E:\Programming\Training_projects\C++\Finder\findFiles_test\folder_1\folder_1_1\folder_1_1_file_2.txt)"));
    assert(finder.isFileExist(u"folder_1_file_2.txt"));
    assert(finder.IsFilePath(u"folder_1_file_2.txt",
        uR"(E:\Programming\Training_projects\C++\Finder\findFiles_test\folder_1\folder_1_file_2.txt)"));
    assert(finder.isFileExist(u"folder_3_file_1.txt"));
    assert(finder.IsFilePath(u"folder_3_file_1.txt",
        uR"(E:\Programming\Training_projects\C++\Finder\findFiles_test\folder_3\folder_3_file_1.txt)"));
    assert(finder.isFileExist(u"folder_1_2_1_file_1.txt"));
    assert(finder.IsFilePath(u"folder_1_2_1_file_1.txt",
        uR"(E:\Programming\Training_projects\C++\Finder\findFiles_test\folder_1\folder_1_2\folder_1_2_1\folder_1_2_1_file_1.txt)"));
    
    cout << "FindAllFilesViaPath - Simple Test is passed!\n";
}

void TestSimpleRusFindAllFilesViaPath() {
    Finder finder;
    finder.FindAllFilesViaPath(SIMPLE_TEST_RUS_PATH);

    assert(finder.files_count() == 11);

    assert(finder.isFileExist(u"корень_файл_1.txt"));
    assert(finder.IsFilePath(u"корень_файл_1.txt",
        uR"(E:\\Programming\\Training_projects\\C++)"
        uR"(\\Finder\\rus_findFiles_test\\корень_файл_1.txt)"));
    
    assert(finder.isFileExist(u"папка_1_1_файл_1.txt"));
    assert(finder.IsFilePath(u"папка_1_1_файл_1.txt",
        uR"(E:\\Programming\\Training_projects\\C++)"
        uR"(\\Finder\\rus_findFiles_test\\папка_1\\папка_1_1\\папка_1_1_файл_1.txt)"));
    
    assert(finder.isFileExist(u"папка_1_файл_2.txt"));
    assert(finder.IsFilePath(u"папка_1_файл_2.txt",
        uR"(E:\\Programming\\Training_projects\\C++)"
        uR"(\\Finder\\rus_findFiles_test\\папка_1\\папка_1_файл_2.txt)"));
    
    assert(finder.isFileExist(u"папка_1_3_файл_1.txt"));
    assert(finder.IsFilePath(u"папка_1_3_файл_1.txt",
        uR"(E:\\Programming\\Training_projects\\C++)"
        uR"(\\Finder\\rus_findFiles_test\\папка_1\\папка_1_3\\папка_1_3_файл_1.txt)"));
    
    assert(finder.isFileExist(u"папка_1_2_1_файл_1.txt"));
    assert(finder.IsFilePath(u"папка_1_2_1_файл_1.txt",
        uR"(E:\\Programming\\Training_projects\\C++)"
        uR"(\\Finder\\rus_findFiles_test\\папка_1\\папка_1_2)"
        uR"(\\папка_1_2_1\\папка_1_2_1_файл_1.txt)"));

    cout << "FindAllFilesViaPath - Rus Files Test is passed!\n";
}

void TestHardFindAllFilesViaPath() {
    Finder finder;
    finder.FindAllFilesViaPath(HARD_TEST_PATH);

    assert(finder.files_count() == 2317);
    assert(finder.folders_count() == 2133);

    assert(finder.isFileExist(u"file_008_133.txt"));
    assert(finder.IsFilePath(u"file_008_133.txt",
        uR"(E:\\Programming\\Training_projects\\C++)"
        uR"(\Finder\hard_findFiles_test\\file_008_133.txt)"));

    assert(finder.isFileExist(u"file_413_2.txt"));
    assert(finder.IsFilePath(u"file_413_2.txt",
        uR"(E:\\Programming\\Training_projects\\C++)"
        uR"(\Finder\hard_findFiles_test\folder_70_2\)"
        uR"(folder_56_1\folder_13_1\folder_11_1\folder_42_1)"
        uR"(\folder_86_1\folder_05_2\folder_52_1\)"
        uR"(folder_01_1\folder_48_1\folder_22_1\folder_52_1\folder_10_2\folder_34_1\)"
        uR"(file_413_2.txt)"));

    assert(finder.isFileExist(u"fileToFind_Acva.txt"));
    assert(finder.IsFilePath(u"fileToFind_Acva.txt",
        uR"(E:\\Programming\\Training_projects\\C++)"
        uR"(\Finder\hard_findFiles_test\folder_74_1)"
        uR"(\folder_85_2\folder_43_2\folder_63_2\folder_64_2)"
        uR"(\fileToFind_Acva.txt)"));

    cout << "FindAllFilesViaPath - Hard Test is passed!\n";
}

void Test_Copy_FindAllFilesViaPath() {
    Finder finder;
    finder.FindAllFilesViaPath(COPY_TEST_PATH);

    assert(finder.files_count() == 11);

    assert(finder.isFileExist(u"copy_1.txt"));
    assert(finder.IsFilePath(u"copy_1.txt",
        uR"(E:\\Programming\Training_projects\C++)"
        uR"(\Finder\findCopyFiles_test\copy_1.txt)"));

    assert(finder.isFileExist(u"copy_1.txt"));
    assert(finder.IsFilePath(u"copy_1.txt",
        uR"(E:\\Programming\Training_projects\C++)"
        uR"(\Finder\findCopyFiles_test\folder_3\copy_1.txt)"));

    assert(finder.isFileExist(u"copy_2.txt"));
    assert(finder.IsFilePath(u"copy_2.txt",
        uR"(E:\\Programming\Training_projects\C++)"
        uR"(\Finder\findCopyFiles_test\folder_1\copy_2.txt)"));

    assert(finder.isFileExist(u"copy_2.txt"));
    assert(finder.IsFilePath(u"copy_2.txt",
        uR"(E:\\Programming\Training_projects\C++)"
        uR"(\Finder\findCopyFiles_test\folder_1\folder_1_2\folder_1_2_1\copy_2.txt)"));
}

void Profile_FindAllFilesViaPath(fs::path path) {
    Finder finder;

    Profiler profiler;
    finder.FindAllFilesViaPath(path);
    profiler.PrintResults<std::chrono::milliseconds>("All files were found for: "s);
    cout << "Files count: " << finder.files_count() << '\n';
}

void TestFindFilesBySubstring() {
}

void TestFindRusFilesBySubstring() {
}

void TestOpenDirectory() {
    Finder finder;
    auto w = FinderWarning::no_warnings;
    
    finder.OpenDirectory(HARD_TEST_PATH, w);
    assert(w == FinderWarning::no_warnings);

    cerr << "TestOpenDirectory was succsessull.\n";
}

void TestOpenDirectoryViaFileName() {
    Finder finder;
    finder.FindAllFilesViaPath(SIMPLE_TEST_PATH);
    auto w = FinderWarning::no_warnings;

    finder.OpenDirectoryViaFileName(u"folder_1_2_1_file_1.txt", w);
    assert(w == FinderWarning::no_warnings);

    cerr << "TestOpenDirectoryViaFileName was succsessull.\n";
}

void FindFilesViaConsoleTest(const fs::path& current_path) {
    Finder finder;
    FinderWarning w = FinderWarning::no_warnings;
    finder.FindAllFilesViaPath(current_path, w);

    ConsoleSearcherUI<Finder> ui(finder, Language::RU);
    
    refresher<Finder> f_refresher = [](const Finder& f, const u16string& str)
        {return f.FindFilesBySubstring(str, 30); };
    ui.SetRefresherFoundList(f_refresher);
    
    opener_file<Finder> f_opener = [](const Finder& f, const fs::path str)
        { f.OpenDirectory(str); };
    ui.SetOpennerFile(f_opener);

    ui.SetCurrentPath(current_path);

    ui.run();
}
#endif // _DEBUG