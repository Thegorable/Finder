#pragma once
#define NOMINMAX
#include<iostream>
#include<cassert>
#include<string>
#include<map>
#include<filesystem>
#include<stdio.h>
#include<functional>
#include<memory>
#include<Windows.h>
#include"Time.h"
#include"Labels.h"

namespace fs = std::filesystem;
using namespace std::string_literals;

template <typename searcher>
using refresher = std::function<format_file_map(const searcher& object, const std::u16string& str)>;

template <typename searcher>
using opener_file = std::function<void(const searcher& object, const std::u16string& str)>;

const WCHAR ESC = 27;
const WCHAR ENTER = 13;
const WCHAR BACK_SPACE = 8;
const WCHAR NOTHING = '\0';

enum class Dir{Up, Down, Right, Left};

template <typename searcher>
class ConsoleSearcherUI
{
public:
    ConsoleSearcherUI(const searcher& object, Language len = Language::RU);

    void run();
    void SetRefresherFoundList(refresher<searcher> ref_func);
    void SetOpennerFile(opener_file<searcher> func);
    void PrintU16InConsole(const WCHAR* c, DWORD len = 1);
    void PrintU16InConsole(const std::u16string& w_str);
    void PrintU16InConsole(const fs::path& path);

private:
    void Key(INPUT_RECORD& input_event, DWORD& read_num);
    void UpdateScreenBuffer();
    void SetCursorYPosition(SHORT posY = 0);
    void SetCursorXYPosition(SHORT posX = 0, SHORT posY = 0);
    bool Step(Dir dir);
    void Switch(Dir dir);
    void DeleteCurrentCharFromConsole();
    void ReadCurrentConsoleLine(std::u16string& str);
    void RefreshFoundList();
    void ReprintFoundList();
    void ClearConsoleFoundList();
    void UpdateFoundListFromCurrentConsoleLine();
    void StopRunning();
    std::u16string GetMatchedFilesCount();
    void ReprintFoundFilesCount();
    void HighLightLine(SHORT y, DWORD x_len);
    void ResetHighlightLine(SHORT y, DWORD x_len);
    void HighLightCurrentLine(SHORT x_len);

    std::u16string start_instructions = u"Enter file name:";
    std::u16string found_list_label = u"Matched files";
    std::u16string no_found_list_label = u"No Matched Files";
    
    const HANDLE H_CONSOLE_IN;
    const HANDLE H_CONSOLE_OUT;
    const SHORT INPUT_LINE_POS_ = 3;
    const SHORT FOUND_LIST_LABEL_Y_POS_ = 5;
    const SHORT FOUND_LIST_Y_POS_ = 7;
    const SHORT HILIGHT_LINE_LEN_ = 30;

    Language language_;

    CONSOLE_SCREEN_BUFFER_INFO buffer_;
    std::u16string input_line_;
    short size_input_line_;
    bool is_running;
    bool on_input_line_;
    int max_found_list_size_;
    SHORT prev_hilight_num_line_;
    SHORT found_list_on_screen_count_;

    format_file_map found_list_;
    const searcher& searcher_object_;
    refresher<searcher> refresh_found_files_f_;
    opener_file<searcher> open_file_;
};

template <typename searcher>
ConsoleSearcherUI<searcher>::ConsoleSearcherUI(const searcher& object, Language lan) : 
    language_(lan),
    searcher_object_(object),
    is_running(true),
    size_input_line_(0),
    max_found_list_size_(15),
    prev_hilight_num_line_(0),
    found_list_on_screen_count_(0),
    on_input_line_(true),
    H_CONSOLE_IN(GetStdHandle(STD_INPUT_HANDLE)),
    H_CONSOLE_OUT(GetStdHandle(STD_OUTPUT_HANDLE))
{
    UpdateScreenBuffer();
    input_line_.reserve(buffer_.dwSize.X);

    if (H_CONSOLE_IN == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("The getting discriptor input console wasn't successfull");
    }

    if (H_CONSOLE_OUT == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("The getting discriptor output console wasn't successfull");
    }

    if (!SetConsoleOutputCP(65001)) {
        throw std::runtime_error("Error SetConsoleOutputCP");
    }

    if (!SetConsoleCP(65001)) {
        throw std::runtime_error("Error SetConsoleOutputCP");
    }

    DWORD h_in_set = 0;
    DWORD h_out_set = 0;

    if (!GetConsoleMode(H_CONSOLE_IN, &h_in_set)) {
        throw std::runtime_error("Error GetConsoleMode Input");
    }

    if (!GetConsoleMode(H_CONSOLE_OUT, &h_out_set)) {
        throw std::runtime_error("Error GetConsoleMode Output");
    }

    h_in_set &= ~ENABLE_VIRTUAL_TERMINAL_INPUT;
    h_out_set = ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
    
    if (!SetConsoleMode(H_CONSOLE_IN, h_in_set)) {
        throw std::runtime_error("Error SetConsoleMode VIRTUAL_TERMINAL_INPUT");
    }

    if (!SetConsoleMode(H_CONSOLE_OUT, h_out_set)) {
        throw std::runtime_error("Error VIRTUAL_TERMINAL_PROCESSING");
    }

    SetCursorYPosition(0);
    PrintU16InConsole(start_instructions);
    SetCursorYPosition(FOUND_LIST_LABEL_Y_POS_);
    PrintU16InConsole(found_list_label);
    ReprintFoundList();
}

template <typename searcher>
inline void ConsoleSearcherUI<searcher>::run() {
    is_running = true;
    MaxFPS fps_restrictor(30);
    SetCursorYPosition(INPUT_LINE_POS_);

    INPUT_RECORD input_event;
    DWORD read_num;

    while (is_running) {
        fps_restrictor.StartFunction();
        Key(input_event, read_num);
        fps_restrictor.EndFunction();
    }
}

template<typename searcher>
inline void ConsoleSearcherUI<searcher>::SetRefresherFoundList(refresher<searcher> ref_func) {
    refresh_found_files_f_ = ref_func;
}

template<typename searcher>
inline void ConsoleSearcherUI<searcher>::SetOpennerFile(opener_file<searcher> func) {
    open_file_ = func;
}

template <typename searcher>
inline void ConsoleSearcherUI<searcher>::Key(INPUT_RECORD& input_event, DWORD& read_num) {
    bool read_result = ReadConsoleInput(H_CONSOLE_IN, &input_event, 5, &read_num);
    assert(read_result);
    
    if (input_event.EventType == KEY_EVENT && input_event.Event.KeyEvent.bKeyDown) {
        switch (input_event.Event.KeyEvent.uChar.UnicodeChar) {
        case ENTER:
            if (!on_input_line_) {
                UpdateScreenBuffer();
                auto it = next(found_list_.begin(), buffer_.dwCursorPosition.Y - FOUND_LIST_Y_POS_);
                open_file_(searcher_object_, *it->first);
                StopRunning();
            }
            break;

        case ESC:
            StopRunning();
            break;

        case NOTHING:
            switch (input_event.Event.KeyEvent.wVirtualKeyCode)
            {
            case VK_LEFT:
                Step(Dir::Left);
                break;

            case VK_RIGHT:
                Step(Dir::Right);
                break;

            case VK_UP:
                Switch(Dir::Up);
                break;

            case VK_DOWN:
                Switch(Dir::Down);
                break;

            case VK_DELETE:
                DeleteCurrentCharFromConsole();
                break;
            };
            break;

        case BACK_SPACE:
            if (Step(Dir::Left)) {
                DeleteCurrentCharFromConsole();
            }
            break;

        default:
            if (on_input_line_) {
                PrintU16InConsole(&input_event.Event.KeyEvent.uChar.UnicodeChar);
                size_input_line_++;
                UpdateFoundListFromCurrentConsoleLine();
            }
        }
    }
}

template <typename searcher>
inline void ConsoleSearcherUI<searcher>::UpdateScreenBuffer() {
    GetConsoleScreenBufferInfo(H_CONSOLE_OUT, &buffer_);
}

template <typename searcher>
inline void ConsoleSearcherUI<searcher>::SetCursorYPosition(SHORT posY) {
    COORD new_cursor_pos;
    new_cursor_pos.X = 0;
    new_cursor_pos.Y = posY;

    bool result = SetConsoleCursorPosition(H_CONSOLE_OUT, new_cursor_pos);
    assert(result);
}

template<typename searcher>
inline void ConsoleSearcherUI<searcher>::SetCursorXYPosition(SHORT posX, SHORT posY) {
    bool result = SetConsoleCursorPosition(H_CONSOLE_OUT, { posX, posY });
    assert(result);
}

template<typename searcher>
inline bool ConsoleSearcherUI<searcher>::Step(Dir dir) {
    UpdateScreenBuffer();
    if (on_input_line_) {
        switch (dir)
        {
        case Dir::Right:
            if (buffer_.dwCursorPosition.X < input_line_.size()) {
                printf("\x1b[1C");
                return true;
            }

        case Dir::Left:
            if (buffer_.dwCursorPosition.X > 0) {
                printf("\x1b[1D");
                return true;
            }
        }
    }
    return false;
}

template<typename searcher>
inline void ConsoleSearcherUI<searcher>::Switch(Dir dir) {
    if (found_list_on_screen_count_) {
        UpdateScreenBuffer();

        switch (dir)
        {
        case Dir::Up:
            if (buffer_.dwCursorPosition.Y == INPUT_LINE_POS_) {
                SetCursorYPosition(found_list_on_screen_count_ + FOUND_LIST_Y_POS_ - 1);
                HighLightCurrentLine(HILIGHT_LINE_LEN_);
                on_input_line_ = false;
                break;
            }
            else if (buffer_.dwCursorPosition.Y == FOUND_LIST_Y_POS_) {
                SetCursorYPosition(INPUT_LINE_POS_);
                ResetHighlightLine(prev_hilight_num_line_, HILIGHT_LINE_LEN_);
                on_input_line_ = true;
                break;
            }
            printf("\x1b[1A");
            HighLightCurrentLine(HILIGHT_LINE_LEN_);
            break;

        case Dir::Down:
            if (buffer_.dwCursorPosition.Y == found_list_on_screen_count_ + FOUND_LIST_Y_POS_ - 1) {
                SetCursorYPosition(INPUT_LINE_POS_);
                ResetHighlightLine(prev_hilight_num_line_, HILIGHT_LINE_LEN_);
                on_input_line_ = true;
                break;
            }
            else if (buffer_.dwCursorPosition.Y == INPUT_LINE_POS_) {
                SetCursorYPosition(FOUND_LIST_Y_POS_);
                HighLightCurrentLine(HILIGHT_LINE_LEN_);
                on_input_line_ = false;
                break;
            }
            printf("\x1b[1B");
            HighLightCurrentLine(HILIGHT_LINE_LEN_);
            break;
        }
    }
}

template<typename searcher>
inline void ConsoleSearcherUI<searcher>::DeleteCurrentCharFromConsole() {
    if (on_input_line_) {
        UpdateScreenBuffer();
        if (buffer_.dwCursorPosition.X < input_line_.size()) {
            size_input_line_--;
            printf("\x1b[1P");
            UpdateFoundListFromCurrentConsoleLine();
        }
    }
}

template <typename searcher>
inline void ConsoleSearcherUI<searcher>::StopRunning() {
    std::cout << "\nTerminate programm\n";
    is_running = false;
}

template<typename searcher>
inline std::u16string ConsoleSearcherUI<searcher>::GetMatchedFilesCount() {
    return u'(' + std::u16string(found_list_.size()) + u')';
}

template<typename searcher>
inline void ConsoleSearcherUI<searcher>::ReprintFoundFilesCount() {
    UpdateScreenBuffer();
    WCHAR space = ' ';
    COORD space_to_fill = { 0, FOUND_LIST_Y_POS_ };
    DWORD num_written = 0;

    bool result = FillConsoleOutputCharacter(H_CONSOLE_OUT,
                                            space,
                                            buffer_.dwSize.X * max_found_list_size_,
                                            space_to_fill,
                                            &num_written);
    assert(result);
}

template<typename searcher>
inline void ConsoleSearcherUI<searcher>::HighLightLine(SHORT y, DWORD x_len) {
    DWORD num_writen = 0;
    bool result = FillConsoleOutputAttribute(H_CONSOLE_OUT,
        BACKGROUND_INTENSITY,
        x_len,
        { 0, y },
        &num_writen);
    assert(result);
}

template<typename searcher>
inline void ConsoleSearcherUI<searcher>::ResetHighlightLine(SHORT y, DWORD x_len) {
    DWORD num_writen = 0;
    bool result = FillConsoleOutputAttribute(H_CONSOLE_OUT,
        FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
        x_len,
        { 0, y },
        &num_writen);
    assert(result);
}

template<typename searcher>
inline void ConsoleSearcherUI<searcher>::HighLightCurrentLine(SHORT x_len) {
    UpdateScreenBuffer();
    ResetHighlightLine(prev_hilight_num_line_, x_len);
    HighLightLine(buffer_.dwCursorPosition.Y, x_len);
    prev_hilight_num_line_ = buffer_.dwCursorPosition.Y;
}

template <typename searcher>
inline void ConsoleSearcherUI<searcher>::PrintU16InConsole(const WCHAR* in_char, DWORD len) {
    DWORD num_printed = 0;
    switch (*in_char) {

    case BACK_SPACE: ESC: NOTHING:
    break;

    case ENTER:
    {
        WCHAR enter_c = '\n';
        bool result = WriteConsole(H_CONSOLE_OUT, &enter_c, 1, &num_printed, NULL);
        assert(result);
        break;
    }

    default:
        printf("\x1b[1@");
        bool result = WriteConsole(H_CONSOLE_OUT, in_char, len, &num_printed, NULL);
        assert(result);
    }

}

template <typename searcher>
inline void ConsoleSearcherUI<searcher>::PrintU16InConsole(const std::u16string& u16str) {
    PrintU16InConsole(reinterpret_cast<const WCHAR*> (u16str.c_str()), static_cast<DWORD>(u16str.size()));
}

template<typename searcher>
inline void ConsoleSearcherUI<searcher>::PrintU16InConsole(const fs::path& path) {
    PrintU16InConsole(path.filename().u16string());
    PrintU16InConsole(std::u16string(u"\t\t:\t\t..."));
    std::wstring last_str = path.wstring();
    size_t file_name_size = path.filename().wstring().size();
    PrintU16InConsole(last_str.c_str() + last_str.size() - 60 - file_name_size - 1, 60);
}

template <typename searcher>
inline void ConsoleSearcherUI<searcher>::ReadCurrentConsoleLine(std::u16string& str) {
    UpdateScreenBuffer();
    std::shared_ptr<WCHAR[]> in(new WCHAR[size_input_line_]);
    DWORD is_read = 0;

    bool result = ReadConsoleOutputCharacter(H_CONSOLE_OUT,
        in.get(),
        size_input_line_,
        { 0, buffer_.dwCursorPosition.Y },
        &is_read);
    assert(result);

    str.resize(0);
    for (short i = 0; i < size_input_line_; i++) {
        str.push_back(in.get()[i]);
    }
}

template<typename searcher>
inline void ConsoleSearcherUI<searcher>::RefreshFoundList() {
    found_list_ = refresh_found_files_f_(searcher_object_, input_line_);
}

template <typename searcher>
inline void ConsoleSearcherUI<searcher>::ReprintFoundList() {
    SHORT raw = 0;
    UpdateScreenBuffer();
    ClearConsoleFoundList();
    COORD prev_cursor_pos = buffer_.dwCursorPosition;
    int num = 0;
    
    if (found_list_.empty()) {
        SetCursorYPosition(FOUND_LIST_Y_POS_);
        PrintU16InConsole(no_found_list_label);   
        SetCursorXYPosition(prev_cursor_pos.X, prev_cursor_pos.Y);
        found_list_on_screen_count_ = 0;
        return;
    }
    
    for (const auto& path_str : found_list_) {
        SetCursorYPosition(FOUND_LIST_Y_POS_ + (raw++));
        PrintU16InConsole(*path_str.second);
        num++;
        if (num >= max_found_list_size_) {
            break;
        }
    }
    found_list_on_screen_count_ = num;
    SetCursorXYPosition(prev_cursor_pos.X, prev_cursor_pos.Y);
}

template<typename searcher>
inline void ConsoleSearcherUI<searcher>::ClearConsoleFoundList() {
    UpdateScreenBuffer();
    WCHAR space = ' ';
    COORD space_to_fill = { 0, FOUND_LIST_Y_POS_ };
    DWORD num_written = 0;
    
    bool result = FillConsoleOutputCharacter(H_CONSOLE_OUT,
                                            space,
                                            buffer_.dwSize.X * max_found_list_size_,
                                            space_to_fill,
                                            &num_written);
    assert(result);
}

template<typename searcher>
inline void ConsoleSearcherUI<searcher>::UpdateFoundListFromCurrentConsoleLine() {
    ReadCurrentConsoleLine(input_line_);
    RefreshFoundList();
    ReprintFoundList();
}