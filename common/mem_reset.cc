#include <algorithm>

using std::fill;

template <int N>
void mem_reset (char (&buf)[N]) {
    std::fill(buf, buf + N, 0);
}