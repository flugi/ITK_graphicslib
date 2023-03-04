#include "graphics.hpp"

using namespace genv;

int main() {
    gout.open(800,600);
    gout << text("hello world") << refresh;
    event ev;
    while (gin >> ev) {}
}
