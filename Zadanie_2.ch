using System;
using System.Collections.Generic;
using System.Linq;

class Program
{
    // Генератор случайных чисел
    static Random randomGenerator = new Random();

    // Функция для генерации случайного целого числа в диапазоне [a, b]
    static int GetRandomInt(int a, int b)
    {
        return randomGenerator.Next(a, b + 1);
    }

    // Функция для генерации случайного вещественного числа в диапазоне [a, b]
    static double GetRandomDouble(double a, double b)
    {
        return a + (randomGenerator.NextDouble() * (b - a));
    }

    // Генерация простых чисел до заданного предела с помощью решета Эратосфена
    static List<int> SieveOfEratosthenes(int limit)
    {
        bool[] isPrime = new bool[limit + 1];
        for (int i = 2; i <= limit; i++)
        {
            isPrime[i] = true;
        }

        // Просеивание: отмечаем составные числа
        for (int i = 2; i * i <= limit; i++)
        {
            if (isPrime[i])
            {
                for (int j = i * i; j <= limit; j += i)
                {
                    isPrime[j] = false;
                }
            }
        }

        // Собираем все простые числа в список
        List<int> primesList = new List<int>();
        for (int num = 2; num <= limit; num++)
        {
            if (isPrime[num])
            {
                primesList.Add(num);
            }
        }
        return primesList;
    }

    // Возведение в степень по модулю
    static long Mod(long baseNum, long exp, int mod)
    {
        long result = 1;
        baseNum %= mod;

        while (exp > 0)
        {
            if ((exp & 1) == 1)  // Если exponent нечётный
            {
                result = (result * baseNum) % mod;
            }
            baseNum = (baseNum * baseNum) % mod;
            exp >>= 1;  // Делим exponent на 2
        }
        return result;
    }

    // Вероятностный тест Миллера-Рабина для проверки простоты числа
    static bool MillerRabinTest(int n, int testIterations)
    {
        // Проверка тривиальных случаев
        if (n < 2) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0) return false;

        // Разложение n-1 в виде d * 2^s
        int d = n - 1;
        int s = 0;
        while (d % 2 == 0)
        {
            d /= 2;
            s++;
        }

        // Тестируем число заданное количество раз
        for (int i = 0; i < testIterations; ++i)
        {
            int a = GetRandomInt(2, n - 2);
            long x = Mod(a, d, n);

            if (x == 1 || x == n - 1) continue;

            bool f = true;
            for (int r = 1; r < s; ++r)
            {
                x = Mod(x, 2, n);
                if (x == n - 1)
                {
                    f = false;
                    break;
                }
            }

            if (f) return false;
        }
        return true;
    }

    // Генерация простого числа по методу Миллера
    static Tuple<int, List<int>> MillerPrime(List<int> primesList, int bitSize)
    {
        int maxPrimeIndex = 0;
        while (maxPrimeIndex < primesList.Count && primesList[maxPrimeIndex] < (1 << (bitSize - 1)))
        {
            maxPrimeIndex++;
        }

        int m = 1;
        List<int> usedPrimes = new List<int>();

        // Создаём число m
        while (true)
        {
            int randPrimeIndex = GetRandomInt(0, maxPrimeIndex - 1);
            int randExp = GetRandomInt(1, bitSize - 1);
            long temp = 1;

            for (int i = 0; i < randExp; i++)
            {
                temp *= primesList[randPrimeIndex];
            }

            if (temp > 0 && m <= int.MaxValue / temp)
            {
                m *= (int)temp;
                usedPrimes.Add(primesList[randPrimeIndex]);
            }

            if (m > (1 << (bitSize - 2)))
            {
                if (m >= (1 << (bitSize - 1)))
                {
                    m = 1;
                    usedPrimes.Clear();
                }
                else
                {
                    break;
                }
            }
        }
        return Tuple.Create(2 * m + 1, usedPrimes);
    }

    // Тест Миллера для проверки простоты сгенерированного числа
    static bool MillerTest(int n, List<int> factors, int testCount)
    {
        if (n < 2) return false;

        List<int> Bases = new List<int>();
        for (int i = 0; i < Math.Min(testCount, n - 3); i++)
        {
            Bases.Add(GetRandomInt(2, n - 2));
        }

        // Проверка малой теоремы Ферма
        foreach (int a in Bases)
        {
            if (Mod(a, n - 1, n) != 1)
            {
                return false;
            }
        }

        // Дополнительные проверки для каждого простого делителя
        foreach (int primeFactor in factors)
        {
            bool foundWitness = false;
            foreach (int a in Bases)
            {
                if (Mod(a, (n - 1) / primeFactor, n) != 1)
                {
                    foundWitness = true;
                    break;
                }
            }
            if (!foundWitness)
            {
                return false;
            }
        }
        return true;
    }

    // Генерация числа по методу Поклингтона
    static Tuple<int, List<int>> PocklingtonPrime(List<int> primesList, int bitSize)
    {
        int maxPrimeIndex = 0;
        while (maxPrimeIndex < primesList.Count && primesList[maxPrimeIndex] < (1 << ((bitSize / 2) + 1)))
        {
            maxPrimeIndex++;
        }

        int F = 1;
        List<int> usedFactors = new List<int>();

        while (true)
        {
            int randomIndex = GetRandomInt(0, maxPrimeIndex - 1);
            int randomPower = GetRandomInt(1, (bitSize / 2) + 1);
            long temp = (long)Math.Pow(primesList[randomIndex], randomPower);

            if (temp > 0 && F <= int.MaxValue / temp)
            {
                F *= (int)temp;
                usedFactors.Add(primesList[randomIndex]);
            }

            if (F > (1 << (bitSize / 2)))
            {
                if (F >= (1 << ((bitSize / 2) + 1)))
                {
                    F = 1;
                    usedFactors.Clear();
                }
                else
                {
                    break;
                }
            }
        }

        int R;
        do
        {
            R = GetRandomInt((1 << ((bitSize / 2) - 1)) + 1, (1 << (bitSize / 2)) - 1);
        } while (R % 2 != 0);

        return Tuple.Create(R * F + 1, usedFactors);
    }

    // Тест Поклингтона для проверки простоты сгенерированного числа
    static bool PocklingtonTest(int n, List<int> factors, int testRounds)
    {
        for (int i = 0; i < testRounds; ++i)
        {
            int a = GetRandomInt(2, n - 2);
            if (Mod(a, n - 1, n) != 1)
            {
                return false;
            }

            bool f = false;
            foreach (int factor in factors)
            {
                if (Mod(a, (n - 1) / factor, n) == 1)
                {
                    f = true;
                    break;
                }
            }
            if (!f)
            {
                return true; // Кандидат вероятно простой
            }
        }
        return false; // Кандидат составной
    }

    // Генерация простого числа по алгоритму ГОСТ
    static int GostPrime(List<int> primesList, int bitSize)
    {
        int maxPrimeIndex = 0;
        while (maxPrimeIndex < primesList.Count && primesList[maxPrimeIndex] < (1 << (bitSize / 2)))
        {
            maxPrimeIndex++;
        }

        int q = primesList[GetRandomInt(0, maxPrimeIndex - 1)];

        while (true)
        {
            double epsilon = GetRandomDouble(0, 1);
            double N = (Math.Pow(2, bitSize - 1) / q) + (Math.Pow(2, bitSize - 1) * epsilon / q);
            int n = (int)Math.Ceiling(N);
            if (n % 2 != 0)
            {
                n++;
            }

            for (int u = 0; u < 1000; u += 2)
            {
                int p = (n + u) * q + 1;
                if (p > (1 << bitSize))
                {
                    break;
                }

                if (Mod(2, p - 1, p) == 1 && Mod(2, n + u, p) != 1)
                {
                    return p;
                }
            }
        }
    }

    static void Main()
    {
        // Генерируем список простых чисел до 500
        List<int> primesList = SieveOfEratosthenes(500);

        // Используем 16 битные числа
        int bitSize = 16;

        // Используем 3 итерации теста
        const int testIterations = 3;  // так как (1/4)^3 = 0.015625 < 0.1

        // Список для хранения результатов
        List<Tuple<int, int>> results = new List<Tuple<int, int>>();

        // Генерация и проверка чисел методом Миллера
        {
            results.Clear();
            string ver = "-";
            int rejectedCount = 0;

            while (results.Count < 10)
            {
                var primeData = MillerPrime(primesList, bitSize);
                int prime_number = primeData.Item1;
                List<int> factors = primeData.Item2;

                // Проверяем, не генерировали ли мы это число ранее
                bool isDuplicate = results.Any(r => r.Item1 == prime_number);
                if (isDuplicate) continue;

                if (MillerTest(prime_number, factors, testIterations))
                {
                    ver = "+";
                    results.Add(Tuple.Create(prime_number, rejectedCount));
                    rejectedCount = 0;
                }
                else
                {
                    rejectedCount++;
                }
            }

            // Вывод результатов для метода Миллера
            Console.WriteLine("\nМетод Миллера:");
            Console.WriteLine("----------------------------------------");
            Console.WriteLine("|  №  |   Число   | Проверка |    k   |");

            for (int i = 0; i < results.Count; ++i)
            {
                Console.WriteLine($"| {i + 1,3} | {results[i].Item1,9} | {ver,8} | {results[i].Item2,6} |");
            }
            Console.WriteLine("----------------------------------------");
        }

        // Генерация и проверка чисел методом Поклингтона
        {
            results.Clear();
            string ver = "-";
            int rejectedCount = 0;

            while (results.Count < 10)
            {
                var primeData = PocklingtonPrime(primesList, bitSize);
                int prime_number = primeData.Item1;
                List<int> factors = primeData.Item2;

                bool isDuplicate = results.Any(r => r.Item1 == prime_number);
                if (isDuplicate) continue;

                if (PocklingtonTest(prime_number, factors, testIterations))
                {
                    ver = "+";
                    results.Add(Tuple.Create(prime_number, rejectedCount));
                    rejectedCount = 0;
                }
                else
                {
                    rejectedCount++;
                }
            }

            // Вывод результатов для метода Поклингтона
            Console.WriteLine("\nМетод Поклингтона:");
            Console.WriteLine("----------------------------------------");
            Console.WriteLine("|  №  |   Число   | Проверка |    k   |");

            for (int i = 0; i < results.Count; ++i)
            {
                Console.WriteLine($"| {i + 1,3} | {results[i].Item1,9} | {ver,8} | {results[i].Item2,6} |");
            }
            Console.WriteLine("----------------------------------------");
        }

        // Генерация и проверка чисел по ГОСТ
        {
            results.Clear();
            string ver = "-";
            int rejectedCount = 0;

            while (results.Count < 10)
            {
                int prime_number = GostPrime(primesList, bitSize);

                bool isDuplicate = results.Any(r => r.Item1 == prime_number);
                if (isDuplicate) continue;

                if (MillerRabinTest(prime_number, testIterations))
                {
                    ver = "+";
                    results.Add(Tuple.Create(prime_number, rejectedCount));
                    rejectedCount = 0;
                }
                else
                {
                    rejectedCount++;
                }
            }

            // Вывод результатов для метода ГОСТ
            Console.WriteLine("\nМетод ГОСТ:");
            Console.WriteLine("----------------------------------------");
            Console.WriteLine("|  №  |   Число   | Проверка |    k   |");

            for (int i = 0; i < results.Count; ++i)
            {
                Console.WriteLine($"| {i + 1,3} | {results[i].Item1,9} | {ver,8} | {results[i].Item2,6} |");
            }
            Console.WriteLine("----------------------------------------");
        }
    }
}
