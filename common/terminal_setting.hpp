#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
#include <mutex>

using std::string;
using std::cout;
using std::mutex;
using std::lock_guard;
using std::to_string;

class Terminal_setting {
private:
    // 成员
    // 文字颜色
    string terminal_color;
    // 光标位置
    string terminal_position;
    // 终端大小结构体
    struct winsize w;
    // 终端行数
    unsigned short rows;
    // 终端列数
    unsigned short cols;
    // 锁
    mutex tty_mtx;
protected:
    // 构造函数 初始化提示color, 获取终端行列数
    Terminal_setting() : terminal_color("\033[38;5;82m"),
                         rows(), cols() {
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
            // 出错处理（如终端不支持该命令）
            perror("ioctl failed");
            rows = -1;
            cols = -1;
            return;
        }
        rows = w.ws_row;
        cols = w.ws_col;
        // 设置滚动区域
        cout << "\033[1;" << rows - 3 << "r";
    }
    // 方法
    // 更改文字颜色 color=1 消息输出；color=2 消息输入；
    // color=3打印错误信息；color=4打印提示信息
    void change_color (int color) {
        lock_guard<mutex> lock(tty_mtx);
        switch (color) {
            case 1 :
                terminal_color = "\033[38;5;171m";
                break;
            case 2 :
                terminal_color = "\033[38;5;159m";
                break;
            case 3 :
                terminal_color = "\033[38;5;196m";
                break;
            case 4 :
                terminal_color = "\033[38;5;82m";
                break;
        }
    }
    // 更改光标位置 position=1 保存光标位置；position=2 恢复光标位置
    // position=3 光标位置移动到消息发送位置；position=4 光标移动到输出位置
    void change_position (int position) {
        int x = rows - 2;
        lock_guard<mutex> lock(tty_mtx);
        switch (position) {
            case 1 :
                terminal_position = "\033[s";
                break;
            case 2 :
                terminal_position = "\033[u";
                break;
            case 3 :
                terminal_position = "\033[" + to_string(x) + ";1H";
                break;
            case 4 :
                terminal_position = "\033[1E";
                break;
        }
    }
    // 将ANSI序列输出到终端 type=1(颜色)；type=2(位置)
    void change_tty (int type) {
        lock_guard<mutex> lock(tty_mtx);
        if (type == 1) {
            cout << terminal_color;
        } else {
            cout << terminal_position;
        }
    }
};

class Terminal_setting_change : public Terminal_setting {
public:
    // 恢复位置1；保存位置2；输出位置3；输入位置4
    void tty_pos_ch (int model) {
        switch (model) {
            case 1 :
                change_position(2); // 恢复光标位置
                change_tty(2); // 输出位置
                break;
            case 2 :
                change_position(1); // 保存当前光标位置
                change_tty(2); // 输出位置
                break;
            case 3 :
                change_position(4); // 移动输出光标
                change_tty(2); // 输出位置
                break;
            case 4 :
                change_position(3); // 移动输入光标
                change_tty(2); // 输出位置
                break;
        }
    }
    void tty_info () {
        change_color(4); // 提示信息的颜色
        change_tty(1); // 输出颜色
    }
    void tty_error () {
        change_color(3); // 错误信息的颜色
        change_tty(1); // 输出颜色
    }
    void tty_input () {
        tty_pos_ch(2);
        tty_pos_ch(4);
        change_color(2); // 输入信息颜色
        change_tty(1); // 输出颜色
    }
    void tty_output () {
        tty_pos_ch(1);
        tty_pos_ch(3);
        change_color(1); // 输出信息颜色
        change_tty(1); // 输出颜色
    }
};
// terminal_setting
extern Terminal_setting_change tty_set;