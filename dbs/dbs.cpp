// dbs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <functional>
#include <vector>
#include "Vector.h"

using namespace std;
using namespace dbs;

double distance(const Segment& s1, const Segment& s2)
{
	double n1 = s1.len();
	double n2 = s2.len();

	if ((n1 + n2) == 0)
		return (s2.m_end - s1.m_end).norm();
	else
	{
		auto f = [&s1, &s2, n1, n2](double t1, double t2) {
			return (s2.m_start - s1.m_start + 
				((n2 > 0) ? (s2.m_end - s2.m_start) * (t2 / n2) : Vector{ 0, 0, 0 }) -
				((n1 > 0) ? (s1.m_end - s1.m_start) * (t1 / n1) : Vector{ 0, 0, 0 })
				).sqr_norm();
		};

		auto dfdt1 = [&s1, &s2, n1, n2](double t1, double t2) {
			return (s2.m_start - s1.m_start) * (s1.m_end - s1.m_start) * -2 / n1 - (s2.m_end - s2.m_start) * (s1.m_end - s1.m_start) * 2 * t2 / (n1 * n2) +
				2 * t1 / (n1 * n1) * (s1.m_end - s1.m_start).sqr_norm();
		};

		auto dfdt2 = [&s1, &s2, n1, n2](double t1, double t2) {
			return (s2.m_start - s1.m_start) * (s2.m_end - s2.m_start) * 2 / n2 - (s2.m_end - s2.m_start) * (s1.m_end - s1.m_start) * 2 * t1 / (n1 * n2) +
				2 * t1 / (n1 * n1) * (s1.m_end - s1.m_start).sqr_norm();
		};

		double D = (s2.m_start - s1.m_start) * (s1.m_end - s1.m_start) * 2 / n1;
		double E = (s2.m_end - s2.m_start) * (s1.m_end - s1.m_start) * 2 / (n1 * n2);
		double F = 2;

		double A = (s2.m_start - s1.m_start) * (s2.m_end - s2.m_start) * 2 / n2;
		double B = (s2.m_end - s2.m_start) * (s1.m_end - s1.m_start) * 2 / (n1 * n2);
		double C = 2;

		double det = C * F - E * B;
		double det1 = C * D - A * E;
		double det2 = B * D - A * F;

		double t1 = n1 > 0 ? (n2 > 0 ? det1 / det : D / F): 0;
		double t2 = n2 > 0 ? (n1 > 0 ? det2 / det : -A / C) : 0;

		vector<double> possible_t1;
		if ((t1 < 0) || (t1 > n1))
		{
			possible_t1.push_back(0);
			possible_t1.push_back(n1);
		}
		else
			possible_t1.push_back(t1);

		vector<double> possible_t2;
		if ((t2 < 0) || (t2 > n2))
		{
			possible_t2.push_back(0);
			possible_t2.push_back(n2);
		}
		else
			possible_t2.push_back(t2);

		double min_d = numeric_limits<double>::max();

		for (double pt1 : possible_t1)
		{
			for (double pt2 : possible_t2)
			{
				double d = sqrt(f(pt1, pt2));
				if (d < min_d)
					min_d = d;
			}
		}

		return min_d;
	}

	return 0;
}

void test(const Segment& s1, const Segment& s2, double etalon)
{
	double d = distance(s1, s2);
	if (d != etalon)
		cerr << "Error: answer=" << d << ", " << "etalon=" << etalon << endl;
	else
		cout << "Correct" << endl;
}

void test_all()
{
	{
		Segment s1{ {0, 0, 0}, {0, 0, 0} };
		Segment s2{ {1, 1, 1}, {1, 1, 1} };
		test(s1, s2, sqrt(3));
	}

	{
		Segment s1{ {0, 0, 0}, {0, 0, 0} };
		Segment s2{ {1, 0, 0}, {2, 0, 0} };
		test(s1, s2, 1);
	}

	{
		Segment s1{ {1, 0, 0}, {2, 0, 0} };
		Segment s2{ {0, 0, 0}, {0, 0, 0} };
		test(s1, s2, 1);
	}

	{
		Segment s1{ {0, 0, 0}, {1, 0, 1} };
		Segment s2{ {1, 1, 0}, {0, 1, 1} };
		test(s1, s2, 1);
	}

	{
		Segment s1{ {0, 0, 0}, {1, 0, 1} };
		Segment s2{ {0, 1, 1}, {-1, 1, 2} };
		test(s1, s2, sqrt(1.5));
	}

	{
		Segment s1{ {1, 1, 1}, {2, 2, 2} };
		Segment s2{ {3, 3, 3}, {5, 5, 5} };
		test(s1, s2, sqrt(3));
	}

	{
		Segment s1{ {1, 1, 1}, {2, 2, 2} };
		Segment s2{ {1.5, 1.5, 1.5}, {1.8, 1.8, 1.8} };
		test(s1, s2, sqrt(0));
	}
}

int main()
{
	test_all();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
