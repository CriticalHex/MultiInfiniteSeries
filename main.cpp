#include<iostream>
#include<list>
#include<thread>

using namespace std;

long double f(uint64_t n) {
	//return pow(-1, n - 1) * (4 / (long double)((2 * n) - 1));
	return 1 / pow(n, (long double)1.25);
}

void sum(long double partialSums[], uint64_t i, uint64_t start, uint64_t stop) {
	for (uint64_t n = start; n < stop; n++) {
		partialSums[i] += f(n);
	}
}

void spawnThreads(list<thread>& threads, const int nThreads, long double partialSums[], uint64_t end) {
	uint64_t start;
	uint64_t stop;
	uint64_t blockSize = end / ((uint64_t)nThreads + 1);
	cout << "Spawning threads..." << endl;
	for (uint64_t i = 0; i < nThreads; i++) {
		start = (i + 1) * blockSize;
		stop = (i + 2) * blockSize;
		//cout << "[" << start << ", " << stop << "]" << endl;
		threads.emplace_back(sum, partialSums, i, start, stop);
		start = stop;
	}
	cout << "Spawned." << endl;
}

template<typename T>
void zero(T arr[], size_t size) {
	for (int i = 0; i < size; i++) {
		arr[i] = 0;
	}
}

void printIters(uint64_t iter) {
	if (iter < 1000000) {
		cout << iter << endl;
	}
	else if (iter < 1000000000) {
		cout << iter / (double)1000000 << " Million" << endl;
	}
	else if (iter < 1000000000000) {
		cout << iter / (double)1000000000 << " Billion" << endl;
	}
	else if (iter < 1000000000000000) {
		cout << iter / (double)1000000000000 << " Trillion" << endl;
	}
}

int main() {
	const unsigned int nThreads = 12;
	long double partialSums[nThreads];
	long double Sn = 0;
	list<thread> threads;
	cout.precision(18);
	for (uint64_t max = (uint64_t)nThreads + 1; max < ULLONG_MAX; max *= (uint64_t)nThreads + 1) {
		cout << "Iterations: "; printIters(max - 1);
		spawnThreads(threads, nThreads, partialSums, max);
		cout << "|";
		for (auto& th : threads) {
			if (th.joinable()) {
				th.join();
				cout << "-";
			}
		}
		cout << "|" << endl;
		for (long double partial : partialSums) {
			Sn += partial;
		}
		cout << "Result: " << Sn << endl << endl;
		zero(partialSums, nThreads);
	}
}