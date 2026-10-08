#include <iostream>

namespace tanriverdi {

    int run(std::istream& in, std::ostream& out, std::ostream& err) {
        int previous = 0;
        int current = 0;

        in >> previous;

        if (!in) {
            err << "Ошибка ввода последовательности\n";
            return 1;
        }

        if (previous == 0) {
            out << 0 << '\n';
            out << 0 << '\n';
            return 0;
        }

        int current_mon_inc = 1;
        int max_mon_inc = 1;
        int inc_seq_count = 0;

        while (true) {
            in >> current;

            if (!in) {
                err << "Ошибка ввода последовательности\n";
                return 1;
            }

            if (current == 0) {
                break;
            }

            if (current >= previous) {
                ++current_mon_inc;
            }
            else {
                current_mon_inc = 1;
            }

            if (current_mon_inc > max_mon_inc) {
                max_mon_inc = current_mon_inc;
            }

            if (current > previous) {
                ++inc_seq_count;
            }

            previous = current;
        }

        out << max_mon_inc << '\n';
        out << inc_seq_count << '\n';

        return 0;
    }
}