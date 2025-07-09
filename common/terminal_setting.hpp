#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

using std::string;
using std::cout;

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
public:
    // 构造函数 初始化提示color, 获取终端行列数
    Terminal_setting () : 
        terminal_color("\033[38;5;82m"),
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
        }
    // 方法
    // 更改文字颜色 color=1 消息输出；color=2 消息输入；color=3打印错误信息
    void change_color (int color) {
        if (color == 1) {
            terminal_color = "\033[38;5;171m";
        } else if (color == 2) {
            terminal_color = "\033[38;5;159m";
        } else {
            terminal_color = "\033[38;5;196m";
        }
    }
    // 更改光标位置 position=1 保存光标位置；position=2 恢复光标位置
    // position=3 光标位置移动到消息发送位置
    void change_position (int position) {
        switch (position) {
            case 1 :
                terminal_position = "\033[s";
                break;
            case 2 :
                terminal_position = "\033[u";
                break;
            default :
                int x = rows - 2;
                terminal_position = "\033[x;1H";
                break;
        }
    }
};