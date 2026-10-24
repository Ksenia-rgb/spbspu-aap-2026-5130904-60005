#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstddef>
#include <new>
#include <limits>

namespace andrijchuk
{
    const int arg_errors_exitcode1 = 1;
    const int arr_errors_exitcode2 = 2;
    const int other_errors_exitcode3 = 3;
    const int num_index = 1;
    const char stack_num = '1';
    const char heap_num = '2';
    const std::size_t stack_args_count = 10000;

    void Checks(int ac, char **av)
    {
        if (ac < 4)
        {
            throw std::runtime_error("Not enough arguments");
        }
        if (ac > 4)
        {
            throw std::runtime_error("Too many arguments");
        }
        if (av[num_index][0] == '\0')
        {
            throw std::runtime_error("First parameter is not a number");
        }
        for (std::size_t k = 0; av[num_index][k] != '\0'; k++)
        {
            if (av[num_index][k] < '0' || av[num_index][k] > '9')
            {
                throw std::runtime_error("First parameter is not a number");
            }
        }
        if (!((av[num_index][0] == '1' || av[num_index][0] == '2') && av[num_index][1] == '\0'))
        {
            throw std::runtime_error("First parameter is out of range");
        }
    }

    void MatrixInput(int arr[], std::size_t rows, std::size_t cols, std::istream &input)
    {
        const std::size_t total = rows * cols;
        for (std::size_t i = 0; i < total; i++)
        {
            if (!(input >> arr[i]))
            {
                throw std::invalid_argument("Wrong matrix arguments");
            }
        }
    }

    std::size_t NumColLSR(const int arr[], std::size_t rows, std::size_t cols)
    {
        if (rows == 0 || cols == 0)
        {
            return 0;
        }
        std::size_t max_len = 1;
        std::size_t result = 1;
        for (std::size_t j = 0; j < cols; j++)
        {
            std::size_t equal_cnt = 1;
            for (std::size_t i = 0; i < rows - 1; i++)
            {
                if (arr[i * cols + j] == arr[(i + 1) * cols + j])
                {
                    equal_cnt++;
                }
                else
                {
                    equal_cnt = 1;
                }
                if (equal_cnt > max_len)
                {
                    max_len = equal_cnt;
                    result = j + 1;
                }
            }
        }
        return result;
    }

    std::size_t CntColNSM(const int arr[], std::size_t rows, std::size_t cols)
    {
        if (rows == 0 || cols == 0)
        {
            return 0;
        }
        std::size_t result = 0;
        for (std::size_t j = 0; j < cols; j++)
        {
            std::size_t equal_cnt = 0;
            for (std::size_t i = 0; i < rows - 1; i++)
            {
                if (arr[i * cols + j] == arr[(i + 1) * cols + j])
                {
                    equal_cnt++;
                }
            }
            if (equal_cnt == 0)
            {
                result++;
            }
        }
        return result;
    }

    void Results(const int arr[], std::size_t rows, std::size_t cols, std::ostream &output)
    {
        output << NumColLSR(arr, rows, cols) << '\n';
        output << CntColNSM(arr, rows, cols) << '\n';
    }
}

int main(int argc, char **argv)
{
    namespace ad = andrijchuk;
    int stack_array[ad::stack_args_count] = {};
    int *heap_array = nullptr;
    try
    {
        ad::Checks(argc, argv);

        const char *num = argv[1];
        const char *input_path = argv[2];
        const char *output_path = argv[3];

        std::ifstream input{input_path};
        if (!input)
        {
            std::cerr << "Cannot open input file" << '\n';
            return ad::other_errors_exitcode3;
        }
        std::ofstream output{output_path};
        if (!output)
        {
            std::cerr << "Cannot open output file" << '\n';
            return ad::other_errors_exitcode3;
        }

        int int_rows{};
        int int_cols{};
        if (!(input >> int_rows >> int_cols) || int_rows < 0 || int_cols < 0)
        {
            throw std::invalid_argument("Wrong input");
        }
        std::size_t rows = static_cast<std::size_t>(int_rows);
        std::size_t cols = static_cast<std::size_t>(int_cols);

        if (cols != 0 && rows > std::numeric_limits<std::size_t>::max() / cols)
        {
            std::cerr << "Cols * rows is too much" << '\n';
            return ad::other_errors_exitcode3;
        }
        if (num[0] == ad::stack_num)
        {
            ad::MatrixInput(stack_array, rows, cols, input);
            ad::Results(stack_array, rows, cols, output);
        }
        if (num[0] == ad::heap_num)
        {
            heap_array = new int[rows * cols];
            ad::MatrixInput(heap_array, rows, cols, input);
            ad::Results(heap_array, rows, cols, output);
        }
        delete[] heap_array;
    }
    catch (const std::runtime_error &e)
    {
        delete[] heap_array;
        std::cerr << e.what() << '\n';
        return ad::arg_errors_exitcode1;
    }
    catch (const std::invalid_argument &e)
    {
        delete[] heap_array;
        std::cerr << e.what() << '\n';
        return ad::arr_errors_exitcode2;
    }
    catch (const std::bad_alloc &e)
    {
        delete[] heap_array;
        std::cerr << e.what() << '\n';
        return ad::other_errors_exitcode3;
    }
    return 0;
}
