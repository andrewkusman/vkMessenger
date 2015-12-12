#include "VK_API.h"
#include "LongPollSession.h"
#include "User.h"
#include "Me.h"
#include "ncurses.h"

std::mutex test;

int row, col;
int current_row;
int col_to_move;
bool input_mode_command = 1;
User current_user;

//std::string readString(WINDOW* scr)
//{
//    int x, y;
//    getyx(scr, y, x);
//    wmove(scr, 0, 0);
//    std::string output;
//    output.clear();
//    for(int i = col_to_move; i < scr->_maxx; i++)
//    {
//        output.push_back(winch(scr));
//        wmove(scr, 0, i);
//        if(winch(scr) == ' ' && output.back() == ' ')
//        {
//            break;
//        }
//    }
//    if(output.empty())
//    {
//        return "";
//    }
//    else
//    {
//        output.pop_back();
//        wmove(scr, y, x);
//        return output;
//    }
//}

std::string getstring(bool t, WINDOW* scr)
{
    std::string input;
    int x, y;

    // let the terminal do the line editing
    nocbreak();
    if(t)
        echo();
    else
        noecho();

    // this reads from buffer after <ENTER>, not "raw"
    // so any backspacing etc. has already been taken care of
    int ch = wgetch(scr);
    getyx(scr, y, x);
    while ( ch != '\n' )
    {
        input.push_back( ch );
        wrefresh(scr);
        ch = wgetch(scr);
    }
    // restore your cbreak / echo settings here

    return input;
}

//std::string input; // Global variable to hold current input. String type because UNIX philosophy.

//std::string getinput () // Gets one unit of input at a time. Whether that is one line or one character is dependent on the input mode.
//{
//    std::string input;
//    char ch;
//    input.clear();
//    if (input_mode_command)
//    {
//        while (1) // Assumes (default) position of cursor is at printing location.
//        {
//            ch = getch();
//            addch(ch);
//            if (ch == '\n') // Keeps buffering input until end of line. Check done after acquiring input char (thus if inside while) to prevent it from being discarded automatically.
//            {
//                return "";
//            }
//            if (ch == '\a' || ch == '\b') // Ensure normal attempts at backspace are caught.
//            {
//                if (!input.empty()) // pop_back will cause a crash when attempting to remove stuff from an empty vector.
//                {
//                    input.pop_back(); // Removes previously entered character from buffered input.
//                    addch('\b'); // Actual backspacing from user's perspective in these 5 lines.
//                    addch(' ');
//                    int y, x;
//                    getyx (stdscr, y, x);
//                    move (y, x-1); // More than one line of user input deemed unlikely/useless.
//                }
//            }
//            else
//            {
//                input.push_back(ch);
//                addch(ch);
//            }
//        }
//    }
//    else
//    {
//        ch = getch();
//        input.push_back(ch);
//    }
//    return input;
//}

void Send(Me* usr)
{
//    wmove(stdscr, row/2, col/2);

}

void PrintNewMessages(WINDOW* windowTest) {
    int x, y;
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    if (current_user.loaded_history)
    {
        for (int j = current_user.list_of_messages.size()-1; j >= 0; j--)
        {
            if (current_user.list_of_messages[j].is_new == true)
            {
                getyx(windowTest, y, x);
//                        buffer = readString(windowTest);
                if (current_user.list_of_messages[j].fromMe)
                {
                    attron(COLOR_PAIR(1));
                    wscrl(stdscr, 1);
                    mvwprintw(stdscr, stdscr->_maxy - 2, 0,
                              "%s >> %s :: %d",
                              current_user.GetFirstName().c_str(),
                              current_user.list_of_messages[j].text.c_str(),
                              current_user.list_of_messages[j].fromMe);
                    attroff(COLOR_PAIR(1));
                    wrefresh(stdscr);
                }
                else
                {
                    attron(COLOR_PAIR(2));
                    wscrl(stdscr, 1);
                    wmove(stdscr, stdscr->_maxy - 2, 0);
                    wprintw(stdscr, "%s >> %s :: %d",
                            current_user.GetFirstName().c_str(),
                            current_user.list_of_messages[j].text.c_str(),
                            current_user.list_of_messages[j].fromMe);
                    wrefresh(stdscr);
                    attroff(COLOR_PAIR(2));
                }
                wmove(windowTest, 0, 0);
                wprintw(windowTest, "%s >> ", current_user.GetFirstName().c_str());
                wrefresh(windowTest);
                wmove(windowTest, y, x);
                current_user.list_of_messages[j].is_new = false;
            }
        }
        current_user.SetNewMessages(false);
        current_user.loaded_history = true;
    }
}


void ShowMessages(Me* usr, LongPollSession* longPollSession, WINDOW* windowTest)
{
    int x, y;
    std::string buffer;
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);

    while(true)
    {
        if (!longPollSession->queueOfMessages.empty())
        {
            test.lock();
            usr->IncMessagesSort(longPollSession->queueOfMessages.front());
            longPollSession->queueOfMessages.pop();
            test.unlock();
        }
        for(int k = 0; k < usr->list_of_user.size(); k++)
        {
            if (usr->list_of_user[k].NewMessages() && usr->list_of_user[k].GetFullName().compare(current_user.GetFullName()) == 0)
            {
                for (int j = 0; j < usr->list_of_user[k].list_of_messages.size(); j++)
                {
                    if (usr->list_of_user[k].list_of_messages[j].is_new == true)
                    {
                        getyx(windowTest,y, x);
//                        buffer = readString(windowTest);
                        if (current_user.list_of_messages[j].fromMe) {
                            attron(COLOR_PAIR(1));
                            wscrl(stdscr, 1);
                            mvwprintw(stdscr, stdscr->_maxy-2, 0,
                                      "%s >> %s :: %d",
                                      usr->list_of_user[k].GetFirstName().c_str(),
                                      usr->list_of_user[k].list_of_messages[j].text.c_str(),
                                      usr->list_of_user[k].list_of_messages[j].fromMe);
                            attroff(COLOR_PAIR(1));
                            wrefresh(stdscr);
                        }
                        else {
                            attron(COLOR_PAIR(2));
                            wscrl(stdscr, 1);
                            wmove(stdscr, stdscr->_maxy-2, 0);
                            wprintw(stdscr, "%s >> %s :: %d",
                                    usr->list_of_user[k].GetFirstName().c_str(),
                                    usr->list_of_user[k].list_of_messages[j].text.c_str(),
                                    usr->list_of_user[k].list_of_messages[j].fromMe);
                            wrefresh(stdscr);
                            attroff(COLOR_PAIR(2));
                        }
                        wmove(windowTest, 0, 0);
                        wprintw(windowTest, "%s >> ", usr->list_of_user[k].GetFirstName().c_str());
                        wrefresh(windowTest);
                        wmove(windowTest, y, x);
                        usr->list_of_user[k].list_of_messages[j].is_new = false;
                    }
                }
                usr->list_of_user[k].SetNewMessages(false);
            }
        }
    }
}


int main()
{
    setlocale(LC_ALL, "");
    initscr();
//    wresize(stdscr, 30, 60);
    start_color();
    scrollok(stdscr, true);
    std::string password;
    cbreak();
    getmaxyx(stdscr, row, col);
    mvwprintw(stdscr, row / 2, (col - strlen("Введите пароль: ")) / 2, "%s", "Введите пароль: ");
    wmove(stdscr, row / 2, (col - strlen("Введите пароль: ")) / 2 + 16);
    wrefresh(stdscr);
    password = getstring(false, stdscr);
    VK_API vk_api = VK_API("89998132952", password);
    while(true)
    {
        if (vk_api.Authorize())
        {
            vk_api.needCaptcha = false;
            vk_api.has_error = false;
            mvwprintw(stdscr, row / 2 + 1, (col - strlen("Введите пароль: ")) / 2, "%s", "Пароль верный");
            wrefresh(stdscr);
            getch();
            clear();
            wrefresh(stdscr);
            WINDOW* windowTest = newwin(1, col, row-1, 0);
            scrollok(windowTest, false);
            wrefresh(windowTest);
            Me usr = Me(vk_api);
            LongPollSession longPollSession = LongPollSession(vk_api);
            longPollSession.StartThread();
            std::thread thr(ShowMessages, &usr, &longPollSession, std::ref(windowTest));
            thr.detach();
            current_user = usr.GetUserByFullName(usr.GetNameById(usr.GetId()));
            current_user.GetMessageHistory(0, 200);
            PrintNewMessages(windowTest);
            int id = 0;
            std::string tmpStr;
            col_to_move = current_user.GetFirstName().length() / 2 + 4; // Кусачев Андрей >>
            wprintw(windowTest, "%s >> ", current_user.GetFirstName().c_str());
            while(true)
            {
//                wrefresh(windowTest);
//                wmove(windowTest,0, 0);
                wmove(windowTest,0, col_to_move);
                wclrtoeol(windowTest);
                wrefresh(windowTest);
                tmpStr = getstring(true, windowTest);
                wrefresh(windowTest);
                if(tmpStr.compare("изменить адресата") == 0)
                {
                    wmove(windowTest, 0, 0);
                    wclrtoeol(windowTest);
                    wrefresh(windowTest);
                    wprintw(windowTest, "Укажите нового адресата: ");
                    wrefresh(windowTest);
                    std::string tmp = getstring(true, windowTest);
                    wrefresh(windowTest);
                    for(int i = 0; i < usr.list_of_user.size(); i++)
                    {
                        if (usr.list_of_user[i].GetFullName().compare(tmp) == 0)
                        {
                            current_user = usr.list_of_user[i];
                        }
                    }
                    wclear(stdscr);
                    wmove(windowTest,0, 0);
                    wclrtoeol(windowTest);
                    wprintw(windowTest, "%s >> ", current_user.GetFirstName().c_str());
                    if(!current_user.loaded_history)
                    {
                        current_user.GetMessageHistory(0, 200);
                    }
                        PrintNewMessages(stdscr);
                    wrefresh(windowTest);
                    col_to_move = current_user.GetFirstName().length() / 2 + 4;
                }
                else
                {
//            std::cout << "Sent " << std::endl;
                    if(usr.MessageSent(current_user.GetId(), tmpStr))
                    {
                        wmove(windowTest, 0, col_to_move);
                    }
                    else
                        std::cout << "Pizdec" << std::endl;
                }
            }
        }
        else {
            wclear(stdscr);
            wmove(stdscr, row/2-1, col/2 - 20);
            wprintw(stdscr, "ERROR: %s :: %s \n", vk_api.error.c_str(), vk_api.error_description.c_str());
            wrefresh(stdscr);
//            std::cout << "ERROR: " + vk_api.error + " :: " + vk_api.error_description << std::endl;
            if (vk_api.needCaptcha) {
                wmove(stdscr, row/2, col/2 - 20);
                wprintw(stdscr, "Captcha sid :: %s ", vk_api.captcha.captcha_sid.c_str());
                wmove(stdscr, row/2+1, col/2 - 20);
                wprintw(stdscr, "Captcha img :: %s ", vk_api.captcha.captcha_img.c_str());
                wmove(stdscr, row/2+2, col/2 - 20);
                wprintw(stdscr, "Input Captcha key: ");
                wrefresh(stdscr);
                vk_api.captcha.captcha_key = getstring(true, stdscr);
            }
        }
    }
    return 0;
}

//g++ -std=c++11 FunctionsFile.cpp Me.cpp LongPollSession.cpp main.cpp Messages.cpp User.cpp VK_API.cpp -o main -pthread -lcurl -lcurlpp -lncursesw