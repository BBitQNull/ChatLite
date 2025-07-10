#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <ncurses.h>

#define BUFFER 1024

using std::string;

class Ncurses {
public:
    // UI命令类型定义
    enum class UICommandType {
        ADD_MESSAGE,
        DRAW_INTERFACE,
        HANDLE_INPUT,
       
    };
protected:
    WINDOW *msg_win, *input_win;
    int rows, cols;
    bool running = false;
    char message[BUFFER];
    char sender[BUFFER];


    // UI命令结构
    struct UICommand {
        UICommandType type;

    };

public:
    Ncurses () {
        initscr();
        start_color();
        cbreak();            // 禁用行缓冲
        noecho();            // 禁用回显
        init_pair(1, COLOR_GREEN, COLOR_BLACK);  // 系统消息
        init_pair(2, COLOR_WHITE, COLOR_BLACK);  // 普通消息
        init_pair(3, COLOR_CYAN, COLOR_BLACK);   // 输入提示
        getmaxyx(stdscr, rows, cols);
        draw_interface(msg_win, input_win, rows, cols);
    }
    ~Ncurses () {
        delwin(input_win);
        delwin(msg_win);
        endwin();
    }
    // 绘制界面
    void draw_interface(WINDOW* msg_win, WINDOW* input_win, int rows, int cols) {
        // 清除屏幕
        clear();

        // 创建消息窗口 (占3/4高度)
        msg_win = newwin(rows * 3 / 4 - 1, cols, 0, 0);
        scrollok(msg_win, TRUE); // 启用滚动
        wbkgd(msg_win, COLOR_PAIR(1));
        box(msg_win, 0, 0);
        mvwprintw(msg_win, 0, 2, "消息窗口");
        wrefresh(msg_win);

        // 创建输入窗口 (占1/4高度)
        input_win = newwin(rows / 4, cols, rows * 3 / 4, 0);
        wbkgd(input_win, COLOR_PAIR(1));
        box(input_win, 0, 0);
        mvwprintw(input_win, 0, 2, "输入窗口");
        wattron(input_win, COLOR_PAIR(3));
        mvwprintw(input_win, 1, 2, "输入消息:");
        wattroff(input_win, COLOR_PAIR(3));
        wrefresh(input_win);
    }
    // 添加消息到消息窗口
    void add_message(const char (&sender)[], const char (&message)[]) {
        int y, x;
        getyx(msg_win, y, x);

        // 检查是否需要滚动
        if (y >= getmaxy(msg_win) - 2) {
            wscrl(msg_win, 1); // 向上滚动一行
            y = getmaxy(msg_win) - 2;
        }

        // 添加消息
        wattron(msg_win, COLOR_PAIR(2));
        mvwprintw(msg_win, y + 1, 2, "%s:", sender);
        wattroff(msg_win, COLOR_PAIR(2));

        mvwprintw(msg_win, y + 2, 4, "%s", message);

        // 更新窗口
        wmove(msg_win, getmaxy(msg_win) - 1, 1);
        wrefresh(msg_win);
    }
    void handle_input(char (&input_buf)[], int max_buf) {
        int y, x;
        // 定位到输入位置
        getyx(input_win, y, x);
        wmove(input_win, 1, 12); // 输入提示后

        // 读取用户输入
        echo(); // 启用回显
        wgetnstr(input_win, input_buf, max_buf - 1);
        noecho();

        // 清空输入行
        wmove(input_win, 1, 12);
        wclrtoeol(input_win);
        wrefresh(input_win);
    }
    void exec_command (const UICommand &command) {
        switch (command.type) {
            case UICommandType::ADD_MESSAGE :
                add_message(sender, message);
                break;
            case UICommandType::HANDLE_INPUT :
                handle_input(message, BUFFER);
                break;
            case UICommandType::DRAW_INTERFACE :
                draw_interface(msg_win, input_win, rows, cols);
                break;
        }
    }
};

class NcursesUI : public Ncurses {
private:
    std::queue<UICommand> commandQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    std::thread ui_thread;
public:
    NcursesUI () {
        running = true;
        ui_thread = std::thread(&ui_thread_func);
    }
    ~NcursesUI () {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            running = false;
        }
        queueCV.notify_one();
        if (ui_thread.joinable()) {
            ui_thread.join();
        }
    }
private:
    void ui_thread_func () {
        while (running) {
            UICommand command;
            // 等待新命令
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                queueCV.wait(lock, [this]{ return !commandQueue.empty() || !running; });
                
                if (!running) break;
                
                command = commandQueue.front();
                commandQueue.pop();
            }
            
            // 执行命令（所有ncurses调用都在这个线程）
            exec_command(command);
        }
    }
public:
        // 外部接口：向UI线程发送命令
    void send_command(const UICommand& command) {
        std::lock_guard<std::mutex> lock(queueMutex);
        commandQueue.push(command);
        queueCV.notify_one();
    }

};