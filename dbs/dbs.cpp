// dbs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <functional>
#include <vector>
#include "Vector.h"

using namespace std;
using namespace dbs;

double distance(const Segment& s1, const Segment& s2)
{
	if (s1.is_point() && s2.is_point()) // trivial case: all segments are points
		return (s2.m_end - s1.m_end).norm();
	else
	{
		// this function returns square of distance between points of line segments. Each point represents by number t between 0 and 1,
		// where 0 is start point of segment and 1 - end point. This function is quadratic with respect to variables t1 and t2. So finding 
		// it's minimum is solving system of two linear equations.
		auto f = [&s1, &s2](double t1, double t2) {
			return (s2.m_start - s1.m_start + 
				(s2.m_end - s2.m_start) * t2 -
				(s1.m_end - s1.m_start) * t1).sqr_norm();
		};

		vector<double> possible_t1;
		vector<double> possible_t2;

		// calculation coefficiets of system.
		double A = (s2.m_start - s1.m_start) * (s2.m_end - s2.m_start) * 2;
		double B = (s2.m_end - s2.m_start) * (s1.m_end - s1.m_start) * 2;
		double C = 2 * s2.sqr_len();
		double D = (s2.m_start - s1.m_start) * (s1.m_end - s1.m_start) * 2;
		double E = B;
		double F = 2 * s1.sqr_len();

		// calculation of determinants.
		double det = C * F - E * B;
		double det1 = C * D - A * E;
		double det2 = B * D - A * F;

		if ((abs(det) < numeric_limits<double>::epsilon()) && !s1.is_point() && !s2.is_point())
		{
			// Determinant of coefficient matrix is zero, but both segments have non-zero length. This is a case of parallel segments.
			// System has infinite number of solutions. But need to be sure that both values lie in interval [0, 1]. Otherwise, check only boundary values.
			double ts1 = A / B;			// value of t1 corresponding to t2 = 0
			double te1 = (C + A) / B;	// value of t1 corresponding to t2 = 1
			if (((ts1 < 0) && (te1 < 0)) || ((ts1 > 1) && (te1 > 1)))
			{
				// ts1 and te1 lie on same side of first segment. So, there is no solution of system that is appropriate.
				possible_t1 = { 0, 1 };
				possible_t2 = { 0, 1 };
			}
			else if ((ts1 >= 0) && (ts1 <= 1))
			{
				possible_t1.push_back(ts1);
				possible_t2.push_back(0);
			}
			else 
			{
				possible_t1.push_back(te1);
				possible_t2.push_back(1);
			}
		}
		else
		{
			// solving system of equations. If one of segments is point system reduces to single equation.
			double t1 = !s1.is_point() ? (!s2.is_point() ? det1 / det : D / F) : 0;
			double t2 = !s2.is_point() ? (!s1.is_point() ? det2 / det : -A / C) : 0;

			// Check that values lie in the interval [0, 1]. If it's false, use the nearest boundary value.
#define	CHECK_AND_ADD_VALUE(v, vs)	if (v < 0)				\
										vs.push_back(0);	\
									else if (v > 1)			\
										vs.push_back(1);	\
									else					\
										vs.push_back(v);

			CHECK_AND_ADD_VALUE(t1, possible_t1)
			CHECK_AND_ADD_VALUE(t2, possible_t2)
		}

		double min_d = numeric_limits<double>::max();

		for (double pt1 : possible_t1)
		{
			for (double pt2 : possible_t2)
			{
				double d = f(pt1, pt2);
				if (d < min_d)
					min_d = d;
			}
		}

		return sqrt(min_d);
	}
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
		// two intersect perpendicular segments
		Segment s1{ {0, 0, 0}, {3, 0, 0} };
		Segment s2{ {0, 1, 0}, {0, 0, 0} };
		test(s1, s2, 0);
	}
}

int main()
{
	//test_all();
	Segment s1, s2;
	cout << "Input start point of first segment (type Enter after each value)" << endl;
	cin >> s1.m_start.m_x >> s1.m_start.m_y >> s1.m_start.m_z;
	cout << "Input end point of first segment (type Enter after each value)" << endl;
	cin >> s1.m_end.m_x >> s1.m_end.m_y >> s1.m_end.m_z;
	cout << "Input start point of second segment (type Enter after each value)" << endl;
	cin >> s2.m_start.m_x >> s2.m_start.m_y >> s2.m_start.m_z;
	cout << "Input end point of second segment (type Enter after each value)" << endl;
	cin >> s2.m_end.m_x >> s2.m_end.m_y >> s2.m_end.m_z;
	cout << "distance: " << distance(s1, s2) << endl;
	return 0;
}