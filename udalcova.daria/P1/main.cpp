#include <iostream>
#include <cstddef>

namespace udalcova {
	bool con(std::istream & cin, std::size_t & count, std::size_t & max, std::size_t & total) {

		max = 0;
		total = 0;
		count = 0;
		int a = 0;
		int b = 0;

		if (!(cin >> a)) {
			return 0;
		}

		if (a == 0) {
			return 1;
		}

		total = 1;
		bool hasPrev = 0;
		int next = 0;

		while (cin >> next && next != 0) {
			++total;

			if (next > a) {
				++count;
			}

			if (next > a) {
				++count;
			}

			if (hasPrev && a > b && a > next) {
				++max;
			}

			b = a;
			a = next;
			hasPrev = 1;

		}
		if (!cin.eof() && cin.fail()) {
			return 0;
		}
		return 1;
	}
}

int main() {

	std::size_t count = 0;
	std::size_t max = 0;
	std::size_t total = 0;

	if (!udalcova::con(std::cin, count, max, total)) {
		std::cerr << "Error: Ввод не является корректной последовательность\n";
		return 1;
	}

	int result = 0;

	if (total < 3) {
		std::cerr << "Error: Невозможно вычислить количество локальных максимумов\n";
		result = 2;

		if (total >= 2) {
			std::cout << count << '\n';
		}
	}
	else {
		std::cout << count << '\n';
		std::cout << max << '\n';
	}

	return result;
}
