#include <iostream>
#include <vector>

using namespace std;

int Game(const vector<int>& numbers, int m) {
	int n = numbers.size();

	vector<long> prefix_sums(n + 1, 0);
	for (int i = 0; i < n; i++)
	{
		prefix_sums[i + 1] = prefix_sums[i] + numbers[i];
	}

	vector<long> max_sum(n + 2, 0);

	for (int i = n - 1; i >= 0; i--)
	{
		long max_diff = -1000000;

		for (int k = 1; k <= m && i + k <= n; k++)
		{
			long current_sum = prefix_sums[i + k] - prefix_sums[i];
			max_diff = max(max_diff, current_sum - max_sum[i + k]);
		}
		max_sum[i] = max_diff;
	}


	return max_sum[0] > 0 ? 1 : 0;
}

int main() {
	setlocale(LC_ALL, "Russian");
	cout << "Введите количество чисел для игры и количество чисел, которые игроки могут вычеркивать: ";
	int n, m;
	cin >> n >> m;

	vector<int> numbers(n);
	for (int i = 0; i < n; i++)
	{
		cin >> numbers[i];
	}

	cout << Game(numbers, m) << endl;

	return 0;
}
