#include<iostream>
#include <windows.h>
#include"Finder_Core.h"
#include"WinInOut.h"

namespace fs = std::filesystem;
using namespace std;
using namespace std::chrono_literals;
#include"Finder_Tests.h"

void RunFinder() {
    Finder finder;
    FinderWarning w = FinderWarning::no_warnings;
    finder.FindAllFilesViaPath(fs::current_path(), w);

    ConsoleSearcherUI<Finder> ui(finder, Language::RU);

    refresher<Finder> f_refresher = [](const Finder& f, const u16string& str)
        {return f.FindFilesBySubstring(str); };
    ui.SetRefresherFoundList(f_refresher);

    opener_file<Finder> f_opener = [](const Finder& f, const u16string& str)
        { f.OpenDirectoryViaFileName(str); };
    ui.SetOpennerFile(f_opener);

    ui.run();
}

int main() {
    try {

#ifdef _DEBUG
        //TestPathsMapInsert();
        //TestPathsMapAt();
        TestSimpleFindAllFilesViaPath();
        TestHardFindAllFilesViaPath();
        TestSimpleRusFindAllFilesViaPath();
        //Test_Copy_FindAllFilesViaPath();

        //TestDataSize(BENCHMARK_TEST_PATH_LOW);

        //Profile_FindAllFilesViaPath(BENCHMARK_TEST_PATH_LOW);
        
        FindFilesViaConsoleTest(HARD_TEST_PATH);
        //TestOpenDirectory();
        //TestOpenDirectoryViaFileName();

#endif

#ifndef _DEBUG
        //RunFinder();
#ifdef _PROFILE
        Finder finder;
        //
        Profiler profiler;
        finder.FindAllFilesViaPath(BENCHMARK_TEST_PATH_MAX);
        profiler.PrintResults<std::chrono::milliseconds>("BENCHMARK Low FindAllFilesViaPath: ");
        system("pause");
#endif // _PROFILE

#endif // !_DEBUG

    }
    catch (fs::filesystem_error ex) {
        cout << ex.what() << '\n';
    }
    catch (exception ex) {
        cout << ex.what() << '\n';
    }
    catch (...) {
        cout << "Some exeption was thrown\n";
    }
    
    return 0;
}