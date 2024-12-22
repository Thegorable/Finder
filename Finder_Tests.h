#pragma once
#include <string>
#include"Finder_Core.h"
#include<cmath>

namespace fs = std::filesystem;
using std::string_literals::operator""s;

const fs::path MAIN_TEST_PATH = "E:\\Programming\\Training_projects\\C++\\Finder"_p;
const fs::path SIMPLE_TEST_PATH = MAIN_TEST_PATH / "findFiles_test"_p;
const fs::path SIMPLE_TEST_RUS_PATH = MAIN_TEST_PATH / "rus_findFiles_test"_p;
const fs::path HARD_TEST_PATH = MAIN_TEST_PATH / "hard_findFiles_test"_p;
const fs::path HARD_TEST_PATH_REL = HARD_TEST_PATH.lexically_relative(CURRENT_PATH);
const fs::path RUS_TEST_PATH = MAIN_TEST_PATH / "rus_findFiles_test"_p;
const fs::path COPY_TEST_PATH = MAIN_TEST_PATH / "findCopyFiles_test"_p;
const fs::path BENCHMARK_TEST_PATH_LOW = MAIN_TEST_PATH / "findFiles_Benchmark_test_low"_p;
const fs::path BENCHMARK_TEST_PATH_MID = MAIN_TEST_PATH / "findFiles_Benchmark_test_mid"_p;
const fs::path BENCHMARK_TEST_PATH_MAX = MAIN_TEST_PATH / "findFiles_Benchmark_test_max"_p;
const fs::path REAL_PATH_CICADA = R"(E:\Cicada Adventure\Assets_export)"_p;
const fs::path REAL_PATH_FORT_MID = R"(D:\DEV_FN_33\Engine\Binaries)"_p;
const fs::path REAL_PATH_FORT_MAX = R"(D:\DEV_FN_33)"_p;

long long countDigits(long long number);
std::string GenerateRandomNumberStr(int length);

int FilesGenerator(fs::path input_path, int start_count, int end_count, int max_level, int current_level, int files_max = 0);
void GenerateHardTestFiles();
void GenerateBenchmarkTestFiles();

#ifdef _PROFILE
void TestDataSize(fs::path path);
size_t MeasurePathMemory(const fs::path& p);
#endif // _PROFILE

#ifdef _DEBUG
void TestSimpleFindAllFilesViaPath();
void TestSimpleRusFindAllFilesViaPath();
void TestHardFindAllFilesViaPath();
void Test_Copy_FindAllFilesViaPath();

void Profile_FindAllFilesViaPath(fs::path path);

void TestFindFilesBySubstring();
void TestFindRusFilesBySubstring();
void TestOpenDirectory();
void TestOpenDirectoryViaFileName();

void FindFilesViaConsoleTest(const fs::path& current_path = CURRENT_PATH);

void TestPathsMapInsert();
void TestPathsMapAt();
#endif // _DEBUG