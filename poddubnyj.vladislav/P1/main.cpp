#include <iostream>
#include <stdexcept>

int max(int *arr, int len)
{
    if (len < 1)
    {
        throw std::out_of_range("Bro, length of arr must belong to the interval");
    }
    else
    {
        int max = arr[0];
        for (int i = 0; i < len; i++)
        {
            if (max < *(arr + i))
            {
                max = *(arr + i);
            }
        }
        return max;
    }
}

int count_local_max(int *arr, int len)
{
    if (len < 3)
    {
        throw std::out_of_range("Bro, length of arr must belong to the interval");
    }

    int count = 0;
    for (int i = 1; i < len - 1; i++)
    {
        if (*(arr + i - 1) < *(arr + i) && *(arr + i) > *(arr + i + 1))
        {
            count += 1;
        }
    }

    return count;
}

int main(void)
{
    const int  max_len = 10000;
    int arr[max_len];
    int i = 0;
    int x = -1;

    try
    {
        while (x != 0)
        {
            std::cin >> x;
            if (std::cin.fail())
            {
                throw std::invalid_argument("Bro, we were waiting whole integer");
            }
            if (x != 0)
            {
                if (i >= max_len)
                {
                    throw std::out_of_range("Bro, length of arr must belong to the interval");
                }
                arr[i] = x;
                i += 1;
            }
        }

        if (i > 0)
        {
            std::cout << "Maximum: " << max(arr, i) << std::endl;
        }
        else
        {
            throw std::out_of_range("Bro, length of arr must belong to the interval");
        }

        if (i >= 3)
        {
            std::cout << "Count of local maximums: " << count_local_max(arr, i) << std::endl;
        }
        else
        {
            throw std::out_of_range("Bro, length of arr must belong to the interval");
        }
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (std::out_of_range &e)
    {
        std::cerr << e.what() << std::endl;
        return 2;
    }

    return 0;
}
