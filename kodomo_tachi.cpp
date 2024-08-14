#include <bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(0) -> sync_with_stdio(0);

	double n;
	cin >> n;

	cout << fixed << setprecision(2) << (n / (double)(2 * sqrt(2))) * (n / (double)(2 * sqrt(2))) * 3.14;

	return 0;
}