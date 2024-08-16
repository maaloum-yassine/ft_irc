#include <iostream>
#include <cstring>

int main() {
    char *haystack = "Hello, \npWorld!";
    const char *needle = "\n";

    char *result = strstr(haystack, needle);

    if (result != NULL) {
        std::cout << "Found: " << result << std::endl;  // Affiche: Found: World!
    } else {
        std::cout << "Not found!" << std::endl;
    }

    return 0;
}
hjfkjsdh\nfskjdhfksdjfh
fsdkfjsdkljflsdkjflks
f;kljsdlkfjslkdfjlsdjfl
