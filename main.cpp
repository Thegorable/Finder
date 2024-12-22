#include<iostream>
#include <windows.h>
#include"Finder_Core.h"
#include"WinInOut.h"

namespace fs = std::filesystem;
using namespace std;
using namespace std::chrono_literals;
#include"Finder_Tests.h"

void RunFinder(const fs::path& cur_path = fs::current_path()) {
    Finder finder;
    ConsoleSearcherUI<Finder> ui(finder, Language::RU);
    
    auto data_profiler = [&ui]() { ui.PrintProfiling(25);};
    thread profiler_thread(data_profiler);
    
    FinderWarning w = FinderWarning::no_warnings;
    finder.FindAllFilesViaPath(cur_path, w);
    ui.StopPrintProfiling();
    profiler_thread.join();
    //ui.PrintProfilingOnce(27);

    refresher<Finder> f_refresher = [](const Finder& f, const u16string& str)
        {return f.FindFilesBySubstring(str, 30); };
    ui.SetRefresherFoundList(f_refresher);

    opener_file<Finder> f_opener = [](const Finder& f, const fs::path& path)
        { f.OpenDirectory(path); };
    ui.SetOpennerFile(f_opener);

    ui.SetCurrentPath(cur_path);

    ui.run();
}

int main() {
    try {

#ifdef _DEBUG
        //TestSimpleFindAllFilesViaPath();
        //TestHardFindAllFilesViaPath();
        //TestSimpleRusFindAllFilesViaPath();
        //Test_Copy_FindAllFilesViaPath();

        //TestDataSize(BENCHMARK_TEST_PATH_LOW);

        //Profile_FindAllFilesViaPath(BENCHMARK_TEST_PATH_LOW);
        
        RunFinder(BENCHMARK_TEST_PATH_MID);
        //TestOpenDirectory();
        //TestOpenDirectoryViaFileName();

#endif

#ifndef _DEBUG
        RunFinder();
#ifdef _PROFILE
        //Finder finder;
        //Profiler profiler;
        //finder.FindAllFilesViaPath(BENCHMARK_TEST_PATH_MAX);
        //profiler.PrintResults<std::chrono::milliseconds>("BENCHMARK test FindAllFilesViaPath: ");
        //system("pause");
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