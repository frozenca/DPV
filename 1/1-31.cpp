#include <cstddef>

std::size_t factorial(std::size_t n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int main() {

}
