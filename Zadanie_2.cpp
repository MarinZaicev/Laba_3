#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iomanip>

using namespace std;

mt19937 randomGenerator(random_device{}());

// Функция для генерации случайного целого числа в диапазоне [a, b]
int getRandomInt(int a, int b) {
    return uniform_int_distribution<int>(a, b)(randomGenerator);
}

// Функция для генерации случайного вещественного числа в диапазоне [a, b]
double getRandomDouble(double a, double b) {
    return uniform_real_distribution<double>(a, b)(randomGenerator);
}

// решето Эратосфена
vector<int> sieve_of_eratosthenes(int limit) {
    vector<bool> isPrime(limit + 1, true);
    isPrime[0] = isPrime[1] = false;

    for (int i = 2; i * i <= limit; i++) {
        if (isPrime[i]) {
            for (int j = i * i; j <= limit; j += i) {
                isPrime[j] = false;
            }
        }
    }

    vector<int> primesList;
    for (int num = 2; num <= limit; num++) {
        if (isPrime[num]) {
            primesList.push_back(num);
        }
    }
    return primesList;
}

// Возведение в степень по модулю
long mod(long base, long exp, int mod) {
    long result = 1;
    base %= mod;

    while (exp > 0) {
        if (exp % 2 != 0) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// тест Миллера-Рабина
bool millerRabinTest(int n, int testIterations) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    int d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    for (int i = 0; i < testIterations; ++i) {
        int a = getRandomInt(2, n - 2);
        long x = mod(a, d, n);

        if (x == 1 || x == n - 1) continue;

        bool f = true;
        for (int r = 1; r < s; ++r) {
            x = mod(x, 2, n);
            if (x == n - 1) {
                f = false;
                break;
            }
        }

        if (f) return false;
    }
    return true;
}

// Генерация - метод Миллера
pair<int, vector<int>> Miller_prime(const vector<int>& primesList, int bitSize) {
    int maxPrimeIndex = 0;
    while (maxPrimeIndex < primesList.size() && primesList[maxPrimeIndex] < (1 << (bitSize - 1))) {
        maxPrimeIndex++;
    }

    int m = 1;
    vector<int> usedPrimes;

    while (true) {
        int randPrimeIndex = getRandomInt(0, maxPrimeIndex - 1);
        int randExp = getRandomInt(1, bitSize - 1);
        long temp = 1;

        for (int i = 0; i < randExp; i++) {
            temp *= primesList[randPrimeIndex];
        }

        if (temp > 0 && m <= INT32_MAX / temp) {
            m *= temp;
            usedPrimes.push_back(primesList[randPrimeIndex]);
        }

        if (m > (1 << (bitSize - 2))) {
            if (m >= (1 << (bitSize - 1))) {
                m = 1;
                usedPrimes.clear();
            }
            else {
                break;
            }
        }
    }
    return { 2 * m + 1, usedPrimes };
}

// Тест Миллера
bool Miller_test(int n, const vector<int>& factors, int testCount) {
    if (n < 2) return false;

    vector<int> Bases;
    for (int i = 0; i < min(testCount, n - 3); i++) {
        Bases.push_back(getRandomInt(2, n - 2));
    }

    for (int a : Bases) {
        if (mod(a, n - 1, n) != 1) {
            return false;
        }
    }

    for (int primeFactor : factors) {
        bool foundWitness = false;
        for (int a : Bases) {
            if (mod(a, (n - 1) / primeFactor, n) != 1) {
                foundWitness = true;
                break;
            }
        }
        if (!foundWitness) {
            return false;
        }
    }
    return true;
}

// Генерация - метод Поклингтона
pair<int, vector<int>> Pocklington_prime(const vector<int>& primesList, int bitSize) {
    int maxPrimeIndex = 0;
    while (maxPrimeIndex < primesList.size() && primesList[maxPrimeIndex] < (1 << ((bitSize / 2) + 1))) {
        maxPrimeIndex++;
    }

    int F = 1;
    vector<int> usedFactors;

    while (true) {
        int randomIndex = getRandomInt(0, maxPrimeIndex - 1);
        int randomPower = getRandomInt(1, (bitSize / 2) + 1);
        long temp = pow(primesList[randomIndex], randomPower);

        if (temp > 0 && F <= INT32_MAX / temp) {
            F *= temp;
            usedFactors.push_back(primesList[randomIndex]);
        }

        if (F > (1 << (bitSize / 2))) {
            if (F >= (1 << ((bitSize / 2) + 1))) {
                F = 1;
                usedFactors.clear();
            }
            else {
                break;
            }
        }
    }

    int R;
    do {
        R = getRandomInt((1 << ((bitSize / 2) - 1)) + 1, (1 << (bitSize / 2)) - 1);
    } while (R % 2 != 0);

    return { R * F + 1, usedFactors };
}

// Тест Поклингтона
bool Pocklington_test(int n, const vector<int>& factors, int testRounds) {
    for (int i = 0; i < testRounds; ++i) {
        int a = getRandomInt(2, n - 2);
        if (mod(a, n - 1, n) != 1) {
            return false;
        }

        bool f = false;
        for (int factor : factors) {
            if (mod(a, (n - 1) / factor, n) == 1) {
                f = true;
                break;
            }
        }
        if (!f) {
            return true;
        }
    }
    return false;
}

// ГОСТ Р 34.10-94
int Gost_Prime(const vector<int>& primesList, int bitSize) {
    int maxPrimeIndex = 0;
    while (maxPrimeIndex < primesList.size() && primesList[maxPrimeIndex] < (1 << (bitSize / 2))) {
        maxPrimeIndex++;
    }

    int q = primesList[getRandomInt(0, maxPrimeIndex - 1)];

    while (true) {
        double epsilon = getRandomDouble(0, 1);
        double N = (pow(2, bitSize - 1) / q) + (pow(2, bitSize - 1) * epsilon / q);
        int n = ceil(N);
        if (n % 2 != 0) {
            n++;
        }

        for (int u = 0; u < 1000; u += 2) {
            int p = (n + u) * q + 1;
            if (p > (1 << bitSize)) {
                break;
            }

            if (mod(2, p - 1, p) == 1 && mod(2, n + u, p) != 1) {
                return p;
            }
        }
    }
}

void print_table(const vector<pair<int, int>>& results, const string& method_name) {
    cout << "\n" << method_name << ":\n";
    cout << "+-----+------------+-----------+--------+\n";
    cout << "|  №  |   Число    | Проверка  |   k    |\n";
    cout << "+-----+------------+-----------+--------+\n";

    for (size_t i = 0; i < results.size(); ++i) {
        cout << "| " << setw(3) << i + 1 << " | " << setw(10) << results[i].first
            << " |     +     | " << setw(6) << results[i].second << " |\n";
    }
    cout << "+-----+------------+-----------+--------+\n";
}

int main() {
    vector<int> primesList = sieve_of_eratosthenes(500);
    int bitSize = 16;
    const int testIterations = 3;

    // Метод Миллера
    vector<pair<int, int>> miller_results;
    int rejectedCount = 0;
    while (miller_results.size() < 10) {
        // Явно указываем тип вместо auto декомпозиции
        pair<int, vector<int>> result = Miller_prime(primesList, bitSize);
        int prime_number = result.first;
        vector<int> factors = result.second;

        // Проверка на дубликаты
        bool isDuplicate = any_of(miller_results.begin(), miller_results.end(),
            [prime_number](const auto& p) { return p.first == prime_number; });
        if (isDuplicate) continue;

        // Проверка числа на простоту
        if (Miller_test(prime_number, factors, testIterations)) {
            miller_results.emplace_back(prime_number, rejectedCount);
            rejectedCount = 0;
        }
        else {
            rejectedCount++;
        }
    }
    print_table(miller_results, "Метод Миллера");

    // Метод Поклингтона
    vector<pair<int, int>> pocklington_results;
    rejectedCount = 0;
    while (pocklington_results.size() < 10) {
        pair<int, vector<int>> result = Pocklington_prime(primesList, bitSize);
        int prime_number = result.first;
        vector<int> factors = result.second;

        bool isDuplicate = any_of(pocklington_results.begin(), pocklington_results.end(),
            [prime_number](const auto& p) { return p.first == prime_number; });
        if (isDuplicate) continue;

        if (Pocklington_test(prime_number, factors, testIterations)) {
            pocklington_results.emplace_back(prime_number, rejectedCount);
            rejectedCount = 0;
        }
        else {
            rejectedCount++;
        }
    }
    print_table(pocklington_results, "Метод Поклингтона");

    // Метод ГОСТ
    vector<pair<int, int>> gost_results;
    rejectedCount = 0;
    while (gost_results.size() < 10) {
        int prime_number = Gost_Prime(primesList, bitSize);

        bool isDuplicate = any_of(gost_results.begin(), gost_results.end(),
            [prime_number](const auto& p) { return p.first == prime_number; });
        if (isDuplicate) continue;

        if (millerRabinTest(prime_number, testIterations)) {
            gost_results.emplace_back(prime_number, rejectedCount);
            rejectedCount = 0;
        }
        else {
            rejectedCount++;
        }
    }
    print_table(gost_results, "Метод ГОСТ");

    return 0;
}
