// dbs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <functional>
#include <vector>
#include "Vector.h"

using namespace std;
using namespace dbs;

double distance(const Segment& s1, const Segment& s2, double& tr1, double& tr2)
{
	if (s1.is_point() && s2.is_point())
		return (s2.m_end - s1.m_end).norm();
	else
	{
		auto f = [&s1, &s2](double t1, double t2) {
			return (s2.m_start - s1.m_start + 
				(s2.m_end - s2.m_start) * t2 -
				(s1.m_end - s1.m_start) * t1).sqr_norm();
		};

		auto dfdt1 = [&s1, &s2](double t1, double t2) {
			return (s2.m_start - s1.m_start) * (s1.m_end - s1.m_start) * -2 - (s2.m_end - s2.m_start) * (s1.m_end - s1.m_start) * 2 * t2 +
				2 * t1 * (s1.m_end - s1.m_start).sqr_norm();
		};

		auto dfdt2 = [&s1, &s2](double t1, double t2) {
			return (s2.m_start - s1.m_start) * (s2.m_end - s2.m_start) * 2 - (s2.m_end - s2.m_start) * (s1.m_end - s1.m_start) * 2 * t1 +
				2 * t1 * (s1.m_end - s1.m_start).sqr_norm();
		};

		vector<double> possible_t1;
		vector<double> possible_t2;
		double inner_product = (s2.m_end - s2.m_start) * (s1.m_end - s1.m_start);
		if (!s1.is_point() && !s2.is_point() && ((inner_product * inner_product) / (s1.sqr_len() * s2.sqr_len()) == 1))
		{
			double ts1, ts2;
			double te1, te2;
			Segment s2_start = { s2.m_start, s2.m_start };
			Segment s2_end = { s2.m_end, s2.m_end };
			double r1 = distance(s1, s2_start, ts1, ts2);
			double r2 = distance(s1, s2_end, te1, te2);

			if (((ts1 < 0) && (te1 < 0)) ||
				((ts1 > 1) && (te1 > 1)))
			{
				possible_t1 = { 0, 1 };
				possible_t2 = { 0, 1 };
			}
			else
			{
				tr1 = ts1;
				tr2 = ts2;
				return r1;
			}
		}
		else
		{
			double D = (s2.m_start - s1.m_start) * (s1.m_end - s1.m_start) * 2;
			double E = (s2.m_end - s2.m_start) * (s1.m_end - s1.m_start) * 2;
			double F = 2 * s1.sqr_len();

			double A = (s2.m_start - s1.m_start) * (s2.m_end - s2.m_start) * 2;
			double B = (s2.m_end - s2.m_start) * (s1.m_end - s1.m_start) * 2;
			double C = 2 * s2.sqr_len();

			double det = C * F - E * B;
			double det1 = C * D - A * E;
			double det2 = B * D - A * F;

			double t1 = !s1.is_point() ? (!s2.is_point() ? det1 / det : D / F) : 0;
			double t2 = !s2.is_point() ? (!s1.is_point() ? det2 / det : -A / C) : 0;

			if ((t1 < 0) || (t1 > 1))
			{
				possible_t1.push_back(0);
				possible_t1.push_back(1);
			}
			else
				possible_t1.push_back(t1);

			if ((t2 < 0) || (t2 > 1))
			{
				possible_t2.push_back(0);
				possible_t2.push_back(1);
			}
			else
				possible_t2.push_back(t2);
		}

		double min_d = numeric_limits<double>::max();

		for (double pt1 : possible_t1)
		{
			for (double pt2 : possible_t2)
			{
				double d = sqrt(f(pt1, pt2));
				if (d < min_d)
				{
					tr1 = pt1;
					tr2 = pt2;
					min_d = d;
				}
			}
		}

		return min_d;
	}
}

void test(const Segment& s1, const Segment& s2, double etalon)
{
	double t1, t2;
	double d = distance(s1, s2, t1, t2);
	if (d != etalon)
		cerr << "Error: answer=" << d << ", " << "etalon=" << etalon << endl;
	else
		cout << "Correct" << endl;
}

void test_all()
{
	{
		// two points
		Segment s1{ {0, 0, 0}, {0, 0, 0} };
		Segment s2{ {1, 1, 1}, {1, 1, 1} };
		test(s1, s2, sqrt(3));
	}

	{
		// segment and point on it
		Segment s1{ {0, 0, 0}, {0, 0, 0} };
		Segment s2{ {1, 0, 0}, {2, 0, 0} };
		test(s1, s2, 1);
	}

	{
		// segment and point not lying on it
		Segment s1{ {1, 0, 0}, {2, 0, 0} };
		Segment s2{ {0, 0, 0}, {0, 0, 0} };
		test(s1, s2, 1);
	}

	{
		// two skew segments
		Segment s1{ {0, 0, 0}, {1, 0, 1} };
		Segment s2{ {1, 1, 0}, {0, 1, 1} };
		test(s1, s2, 1);
	}

	{
		// two skew segments 2
		Segment s1{ {0, 0, 0}, {1, 0, 1} };
		Segment s2{ {0, 1, 1}, {-1, 1, 2} };
		test(s1, s2, sqrt(1.5));
	}

	{
		// two segments on same line
		Segment s1{ {1, 1, 1}, {2, 2, 2} };
		Segment s2{ {3, 3, 3}, {5, 5, 5} };
		test(s1, s2, sqrt(3));
	}

	{
		// two intersect segments on same line
		Segment s1{ {1, 1, 1}, {2, 2, 2} };
		Segment s2{ {1.5, 1.5, 1.5}, {1.8, 1.8, 1.8} };
		test(s1, s2, sqrt(0));
	}

	{
		// two parallel segments
		Segment s1{ {0, 0, 0}, {3, 0, 0} };
		Segment s2{ {1, 1, 0}, {2, 1, 0} };
		test(s1, s2, 1);
	}

	{
		// two intersect segments
		Segment s1{ {0, 0, 0}, {3, 0, 0} };
		Segment s2{ {0, 1, 0}, {0, 0, 0} };
		test(s1, s2, 0);
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
