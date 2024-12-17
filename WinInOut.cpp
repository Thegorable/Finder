//#include "WinInOut.h"
//#include<iostream>
//#include"Time.h"
//#include<cassert>
//#include<algorithm>
//#include<memory>

//template <typename refresher>
//ConsoleSearcherUI<refresher>::ConsoleSearcherUI(const std::vector<const std::wstring*>* ptr, Language lan) :
//    found_list_(ptr),
//    language_(lan)
//{
//    UpdateScreenBuffer();
//    input_line_.reserve(buffer_.dwSize.X);
//
//    if (H_CONSOLE_IN == INVALID_HANDLE_VALUE) {
//        throw std::runtime_error("The getting discriptor input console wasn't successfull");
//    }
//
//    if (H_CONSOLE_OUT == INVALID_HANDLE_VALUE) {
//        throw std::runtime_error("The getting discriptor output console wasn't successfull");
//    }
//
//    if (!SetConsoleOutputCP(65001)) {
//        cout << "Error SetConsoleOutputCP: Error Code: " << GetLastError();
//    }
//
//    if (!SetConsoleCP(65001)) {
//        cout << "Error SetConsoleOutputCP: Error Code: " << GetLastError();
//    }
//
//    SetCursorYPosition(0);
//    switch (lan)
//    {
//    case Language::RU:
//        PrintU16InConsole(start_instructions_ru);
//        SetCursorYPosition(FOUND_LIST_LABEL_Y_POS_);
//        PrintU16InConsole(found_list_lablel_ru);
//        break;
//    case Language::EN:
//        printf(start_instructions_en.c_str());
//        SetCursorYPosition(FOUND_LIST_LABEL_Y_POS_);
//        printf(found_list_label_en.c_str());
//        break;
//    default:
//        break;
//    }
//    ReprintFoundList();
//}
//
//template <typename refresher>
//void ConsoleSearcherUI<refresher>::run() {
//    is_running = true;
//    MaxFPS fps_restrictor(30);
//    SetCursorYPosition(INPUT_LINE_POS_);
//
//    INPUT_RECORD input_event;
//    DWORD read_num;
//    
//    while (is_running) {
//        fps_restrictor.StartFunction();
//
//        PrintInputLine(input_event, read_num);
//        
//        fps_restrictor.EndFunction();
//    }
//}
//
//template <typename refresher>
//const std::u16string* ConsoleSearcherUI<refresher>::GetInputLine() {
//    return &input_line_;
//}
//
//template <typename refresher>
//void ConsoleSearcherUI<refresher>::SetFoundListPtr(const std::vector<const std::wstring*>* ptr) {
//    found_list_ = ptr;
//    ReprintFoundList();
//}
//
//template <typename refresher>
//void ConsoleSearcherUI<refresher>::PrintInputLine(INPUT_RECORD& input_event, DWORD& read_num) {
//    if (!ReadConsoleInput(H_CONSOLE_IN, &input_event, 5, &read_num)) {
//        assert("ReadConsoleInput function wasn't successful!");
//    }
//    if (input_event.EventType == KEY_EVENT && input_event.Event.KeyEvent.bKeyDown) {
//        switch (input_event.Event.KeyEvent.uChar.UnicodeChar)
//        {
//        case ENTER:
//            break;
//
//        case ESC:
//            StopRunning();
//            break;
//            
//        case NOTHING:
//            switch (input_event.Event.KeyEvent.wVirtualKeyCode)
//            {
//            case VK_LEFT:
//                printf("\x1b[1D");
//                break;
//
//            case VK_RIGHT:
//                printf("\x1b[1C");
//                break;
//
//            case VK_DELETE:
//                printf("\x1b[1P");
//                size_input_line_--;
//                ReprintFoundList();
//                break;
//            };
//                
//            break;
//
//        case BACK_SPACE:
//            printf("\x1b[1D");
//            printf("\x1b[1P");
//            size_input_line_--;
//            ReprintFoundList();
//            break;
//
//        default:
//            PrintU16InConsole(&input_event.Event.KeyEvent.uChar.UnicodeChar);
//            size_input_line_++;
//            ReadCurrentConsoleLine(input_line_);
//            ReprintFoundList();
//        }
//    }
//}
//
//template <typename refresher>
//void ConsoleSearcherUI<refresher>::UpdateScreenBuffer() {
//    GetConsoleScreenBufferInfo(H_CONSOLE_OUT, &buffer_);
//}
//
//template <typename refresher>
//void ConsoleSearcherUI<refresher>::SetCursorYPosition(SHORT posY) {
//    COORD new_cursor_pos;
//    new_cursor_pos.X = 0;
//    new_cursor_pos.Y = posY;
//
//    bool result = SetConsoleCursorPosition(H_CONSOLE_OUT, new_cursor_pos);
//    assert(result);
//}
//
//template <typename refresher>
//void ConsoleSearcherUI<refresher>::StopRunning() {
//    cout << "\nTerminate programm\n";
//    is_running = false;
//}
//
//template <typename refresher>
//void ConsoleSearcherUI<refresher>::PrintU16InConsole(const WCHAR* in_char, DWORD len) {
//    DWORD num_printed = 0;
//    switch (*in_char) {
//
//    case BACK_SPACE: ESC: NOTHING:
//        break;
//    
//    case ENTER:
//    {
//        WCHAR enter_c = '\n';
//        bool result = WriteConsole(H_CONSOLE_OUT, &enter_c, 1, &num_printed, NULL);
//        assert(result);
//        break;
//    }
//
//    default:
//        printf("\x1b[1@");
//        bool result = WriteConsole(H_CONSOLE_OUT, in_char, len, &num_printed, NULL);
//        assert(result);
//    }
//
//}
//
//template <typename refresher>
//void ConsoleSearcherUI<refresher>::PrintU16InConsole(const std::wstring w_str) {
//    PrintU16InConsole(w_str.c_str(), w_str.size());
//}
//
//template <typename refresher>
//void ConsoleSearcherUI<refresher>::ReadCurrentConsoleLine(std::u16string& str) {
//    UpdateScreenBuffer();
//    shared_ptr<WCHAR[]> in(new WCHAR[size_input_line_]);
//    DWORD is_read = 0;
//    
//    bool result = ReadConsoleOutputCharacter(H_CONSOLE_OUT,
//                                    in.get(),
//                                    size_input_line_, 
//                                    { 0, buffer_.dwCursorPosition.Y }, 
//                                    &is_read);
//    assert(result);
//
//    for (short i = 0; i < size_input_line_; i++) {
//        str.push_back(in.get()[i]);
//    }
//}
//
//template <typename refresher>
//void ConsoleSearcherUI<refresher>::ReprintFoundList() {
//    SHORT raw = 0;
//    UpdateScreenBuffer();
//    for (const auto& path_str : *found_list_) {
//        SetCursorYPosition(FOUND_LIST_Y_POS_ + (raw++));
//        printf("\x1b[%dX", buffer_.dwSize.X);
//        PrintU16InConsole(path_str->c_str(), path_str->size());
//    }
//}
