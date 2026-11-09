#include <cstring>
#include <iostream>
#include <cctype>

const int success = 0;
const int error = 1;

namespace demyanov {

    char* readLine() {
        int size = 1;
        int length = 0;

        char* str = new char[size];
        char c;

        while (std::cin.get(c) && c != '\n') {
            if (length + 1 >= size) {
                size *= 2;
                char* temp = new char[size];

                for (int i = 0; i < length; i++) {
                    temp[i] = str[i];
                }

                delete[] str;
                str = temp;
            }

            str[length] = c;
            length++;
        }

        str[length] = '\0';

        return str;
    }

    int var_16(const char* str_1, const char* str_2, char* result, int max_length) {
        int str_1_len = strlen(str_1);

        for (int i = 0; i < str_1_len; i++) {
            result[i] = str_1[i];
        }

        result[str_1_len] = '\0';

        for (int i = 0; str_2[i] != '\0'; i++) {
            if (isdigit(static_cast<unsigned char>(str_2[i]))) {
                if (str_1_len + 1 < max_length) {
                    result[str_1_len] = str_2[i];
                    str_1_len++;
                    result[str_1_len] = '\0';
                }
                else {
                    std::cerr << "error: not enough memory\n";
                    return error;
                }
            }
        }

        return success;
    }

    int var_5(const char* str_1, const char* str_2, char* result, int max_length) {
        int result_len = 0;

        for (int i = 0; str_2[i] != '\0'; i++) {
            if (strchr(str_1, str_2[i]) == nullptr) {
                if (result_len + 1 < max_length) {
                    result[result_len] = str_2[i];
                    result_len++;
                }
                else {
                    std::cerr << "error: not enough memory\n";
                    return error;
                }
            }
        }

        for (int i = 0; str_1[i] != '\0'; i++) {
            if (strchr(str_2, str_1[i]) == nullptr) {
                if (result_len + 1 < max_length) {
                    result[result_len] = str_1[i];
                    result_len++;
                }
                else {
                    std::cerr << "error: not enough memory\n";
                    return error;
                }
            }
        }

        result[result_len] = '\0';

        return success;
    }
}

int main() {
    using demyanov::readLine;
    using demyanov::var_5;
    using demyanov::var_16;

    char* str_1 = readLine();
    char* str_2 = readLine();

    int max_str_length = strlen(str_1) + strlen(str_2) + 1;

    char* result = new char[max_str_length];

    if (var_5(str_1, str_2, result, max_str_length) != success) {
        delete[] str_1;
        delete[] str_2;
        delete[] result;
        return error;
    }

    std::cout << result << '\n';

    if (var_16(str_1, str_2, result, max_str_length) != success) {
        delete[] str_1;
        delete[] str_2;
        delete[] result;
        return error;
    }

    std::cout << result << '\n';

    delete[] str_1;
    delete[] str_2;
    delete[] result;

    return success;
}
