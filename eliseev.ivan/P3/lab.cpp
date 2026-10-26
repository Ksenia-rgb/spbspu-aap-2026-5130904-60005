#include <iostream>
#include <fstream>

namespace eliseev
{

    int** createMatrix(int rows, int cols)
    {
        int** matrix = new (std::nothrow) int*[rows];
        if (matrix == nullptr)
            throw 2;

        for (int i = 0; i < rows; ++i)
        {
            matrix[i] = new (std::nothrow) int[cols];
            if (matrix[i] == nullptr)
            {
                for (int j = 0; j < i; ++j) delete[] matrix[j];
                delete[] matrix;
                throw 2;
            }
        }
        return matrix;
    }
    void freeMatrix(int** matrix, int rows)
    {
        if (matrix == nullptr) return;
        for (int i = 0; i < rows; ++i)
            delete[] matrix[i];
        delete[] matrix;
    }
    void readSizes(std::ifstream& in, int& rows, int& cols)
    {
        if (!(in >> rows >> cols))
            throw 3;
        if (rows <= 0 || cols <= 0)
            throw 3;
    }
    void readMatrix(std::ifstream& in, int** matrix, int rows, int cols)
    {
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                if (!(in >> matrix[i][j]))
                    throw 3;
            }
        }
    }
    int countColsNoSame(int** matrix, int rows, int cols)
    {
        int count = 0;
        for (int j = 0; j < cols; ++j)
        {
            bool hasSame = false;
            for (int i = 1; i < rows; ++i)
            {
                if (matrix[i][j] == matrix[i - 1][j])
                {
                    hasSame = true;
                    break;
                }
            }
            if (!hasSame) ++count;
        }
        return count;
    }
    int countNonZeroDiagonals(int** matrix, int rows, int cols)
    {
        int count = 0;
        for (int d = -(cols - 1); d <= (rows - 1); ++d)
        {
            bool hasZero = false;
            for (int i = 0; i < rows; ++i)
            {
                int j = i - d;
                if (j < 0 || j >= cols) continue;
                if (matrix[i][j] == 0)
                {
                    hasZero = true;
                    break;
                }
            }
            if (!hasZero) ++count;
        }
        return count;
    }
    void writeResults(std::ofstream& out, int r9, int r18)
    {
        out << r9 << '\n';
        out << r18 << '\n';
    }
    int parseNum(const char* s)
    {
        if (!s || !*s) throw 1;
        if (s[0] != '1' && s[0] != '2') throw 1;
        if (s[1] != '\0') throw 1;
        return s[0] - '0';
    }
    void runFixed(std::ifstream& in, std::ofstream& out)
    {
        const int MAX = 10000;

        int flat[MAX];
        int* matrix[MAX];

        int rows = 0, cols = 0;
        readSizes(in, rows, cols);

        if (rows > MAX / cols)
            throw 3;

        for (int i = 0; i < rows; ++i)
            matrix[i] = flat + i * cols;

        readMatrix(in, matrix, rows, cols);

        int var9  = countColsNoSame(matrix, rows, cols);
        int var18 = countNonZeroDiagonals(matrix, rows, cols);

        writeResults(out, var9, var18);
    }
    void runDynamic(std::ifstream& in, std::ofstream& out)
    {
        int rows = 0, cols = 0;
        readSizes(in, rows, cols);

        int** matrix = createMatrix(rows, cols);

        try
        {
            readMatrix(in, matrix, rows, cols);
        }
        catch (int e)
        {
            freeMatrix(matrix, rows);
            throw e;
        }

        int var9  = countColsNoSame(matrix, rows, cols);
        int var18 = countNonZeroDiagonals(matrix, rows, cols);

        writeResults(out, var9, var18);

        freeMatrix(matrix, rows);
    }

}
int main(int argc, char* argv[])
{
    using namespace eliseev;

    try
    {
        if (argc != 4) throw 1;

        int num = parseNum(argv[1]);

        std::ifstream in(argv[2]);
        if (!in.is_open()) throw 3;

        std::ofstream out(argv[3]);
        if (!out.is_open()) throw 3;

        if (num == 1)
            runFixed(in, out);
        else
            runDynamic(in, out);
    }
    catch (int e)
    {
        std::cerr << "Error " << e << '\n';
        return e;
    }

    return 0;
}