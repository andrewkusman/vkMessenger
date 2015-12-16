#include "VK_API.h"
#include "LongPollSession.h"
#include "User.h"
#include "Me.h"
#include "ncurses.h"

std::mutex test;

int row, col;
int current_row;
int col_to_move;
bool interrupt = false;
User current_user;

void PrintNewMessages(WINDOW* windowTest);

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

void SelectDialog(Me* usr)
{
    initscr();
    bool flag = true;
    unsigned choice = 0; //Выбор пользователя
    scrollok(stdscr, false);
    curs_set(0); //"Убиваем" курсор
    //Включаем режим удобной работы с функциональными клавишами, другими словами KEY_UP и KEY_DOWN без этого не работали бы
    keypad(stdscr, true);
    interrupt = true;
    while ( flag )
    {
        clear();
        for ( unsigned i = 0; i < stdscr->_maxy; i++ ) //Проходим по всем элементам меню
        {
            if ( i == choice )
            {//Если текущий элемент совпадает с выбором пользователя
                addch('>');
            }//Выводим указатель
            else
            {
                addch(' '); //Иначе выводим " ", для равновесия
            }
            printw("%s\n", usr->list_of_user[i].GetFullName().c_str());

        }

        //Получаем нажатие пользователя
        switch ( getch() )
        {
            case KEY_UP:
                if ( choice ) //Если возможно, переводим указатель вверх
                    choice--;
                break;
            case KEY_DOWN:
                if ( choice != stdscr->_maxy - 1) //Если возможно, переводим указатель вниз
                    choice++;
                break;
            case KEY_RIGHT:
                if(!usr->list_of_user[choice].loaded_history)
                {
                    usr->list_of_user[choice].GetMessageHistory(0, 200);
                }
                current_user = usr->list_of_user[choice];
                PrintNewMessages(stdscr);
                PrintNewMessages(stdscr);
                current_user = usr->list_of_user[choice];
                flag = false;
                break;
        }
    }
    interrupt = false;
    nocbreak();
    wclear(stdscr);
}

std::string getstring(bool t, WINDOW* scr)
{
    std::string input;

    // let the terminal do the line editing
    nocbreak();
    if(t)
        echo();
    else
        noecho();

    // this reads from buffer after <ENTER>, not "raw"
    // so any backspacing etc. has already been taken care of
    int ch = wgetch(scr);
    while ( ch != '\n' )
    {
        input.push_back( ch );
        wrefresh(scr);
        ch = wgetch(scr);
    }
    // restore your cbreak / echo settings here

    return input;
}

std::string getstring(bool t, WINDOW* scr, Me* usr)
{
    std::string input;
    int x, y;

    // let the terminal do the line editing
    cbreak();
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
        if(ch == '\t')
        {
            SelectDialog(usr);
        }
        input.push_back( ch );
        wrefresh(scr);
        ch = wgetch(scr);
    }
    // restore your cbreak / echo settings here

    return input;
}


void PrintNewMessages(WINDOW* windowTest) {
    int x, y;
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    wclear(stdscr);
    wmove(stdscr, 0, 0);
    for (int j = 0 ; j <  current_user.list_of_messages.size(); j++)
    {
        test.lock();
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
        test.unlock();
        current_user.list_of_messages[j].is_new = false;
    }
    current_user.SetNewMessages(false);
 }


void ShowMessages(Me* usr, LongPollSession* longPollSession, WINDOW* windowTest)
{
    int x, y;
    std::string buffer;
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);

    while(true)
    {
        if(!interrupt)
        {
            if (!longPollSession->queueOfMessages.empty())
            {
                test.lock();
                usr->IncMessagesSort(longPollSession->queueOfMessages.front());
                longPollSession->queueOfMessages.pop();
                test.unlock();
            }
            for (int k = 0; k < usr->list_of_user.size(); k++)
            {
                if (usr->list_of_user[k].NewMessages() &&
                    usr->list_of_user[k].GetFullName().compare(current_user.GetFullName()) == 0)
                {
                    for (int j = 0; j < usr->list_of_user[k].list_of_messages.size(); j++)
                    {
                        if (usr->list_of_user[k].list_of_messages[j].is_new)
                        {
                            getyx(windowTest, y, x);
                            if (current_user.list_of_messages[j].fromMe)
                            {
                                attron(COLOR_PAIR(1));
                                wscrl(stdscr, 1);
                                mvwprintw(stdscr, stdscr->_maxy - 2, 0,
                                          "%s >> %s :: %d",
                                          usr->list_of_user[k].GetFirstName().c_str(),
                                          usr->list_of_user[k].list_of_messages[j].text.c_str(),
                                          usr->list_of_user[k].list_of_messages[j].fromMe);
                                attroff(COLOR_PAIR(1));
                                wrefresh(stdscr);
                            }
                            else
                            {
                                attron(COLOR_PAIR(2));
                                wscrl(stdscr, 1);
                                wmove(stdscr, stdscr->_maxy - 2, 0);
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
}


int main()
{
    setlocale(LC_ALL, "");
    initscr();
//    wresize(stdscr, 30, 60);
    start_color();
    init_pair(1,COLOR_WHITE, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
    scrollok(stdscr, true);
    std::string password;
    cbreak();
    getmaxyx(stdscr, row, col);
    mvwprintw(stdscr, row / 2 - 1, (col - strlen("Введите пароль: ")) / 2, "%s", "Введите номер телефона: ");
    std::string username = getstring(true, stdscr);
    mvwprintw(stdscr, row / 2, (col - strlen("Введите пароль: ")) / 2, "%s", "Введите пароль: ");
    wmove(stdscr, row / 2, (col - strlen("Введите пароль: ")) / 2 + 16);
    wrefresh(stdscr);
    password = getstring(false, stdscr);
    VK_API vk_api = VK_API(username, password);
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
                wmove(windowTest,0, col_to_move);
                wclrtoeol(windowTest);
                wrefresh(windowTest);
                tmpStr = getstring(true, windowTest, &usr);
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
                            interrupt = true;
                            if(!usr.list_of_user[i].loaded_history)
                            {
                                usr.list_of_user[i].GetMessageHistory(0, 200);
                            }
                            current_user = usr.list_of_user[i];
                            PrintNewMessages(stdscr);
                            wclear(stdscr);
                            PrintNewMessages(stdscr);
                            col_to_move = current_user.GetFirstName().length() / 2 + 4;
                            interrupt = false;
                            break;
                        }
                    }
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
                wmove(windowTest,0, 0);
                wclrtoeol(windowTest);
                wprintw(windowTest, "%s >> ", current_user.GetFirstName().c_str());
                wrefresh(windowTest);
            }
        }
        else {
            wclear(stdscr);
            wmove(stdscr, row/2-1, col/2 - 20);
            wprintw(stdscr, "ERROR: %s :: %s \n", vk_api.error.c_str(), vk_api.error_description.c_str());
            wrefresh(stdscr);
//            std::cout << "ERROR: " + vk_api.error + " :: " + vk_api.error_description << std::endl;
            if (vk_api.needCaptcha) {
                wmove(stdscr, row/2, col/2 - 21);
                wprintw(stdscr, "Captcha sid :: %s ", vk_api.captcha.captcha_sid.c_str());
                wmove(stdscr, row/2+1, col/2 - 21);
                wprintw(stdscr, "Captcha img :: %s ", vk_api.captcha.captcha_img.c_str());
                wmove(stdscr, row/2+2, col/2 - 21);
                wprintw(stdscr, "Input Captcha key: ");
                wrefresh(stdscr);
                vk_api.captcha.captcha_key = getstring(true, stdscr);
                wclear(stdscr);
            }
        }
    }
    return 0;
}

//g++  -std=c++11 main.cpp Messages.cpp LongPollSession.cpp Me.cpp User.cpp VK_API.cpp FunctionsFile.cpp -o main -lcurlpp -lcurl -lncursesw -lpthread
