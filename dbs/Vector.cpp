#include <cmath>
#include <ostream>
#include "Vector.h"

using namespace std;

namespace dbs
{
	ostream& operator<<(ostream& os, const Vector& p)
	{
		os << "(" << p.m_x << ", " << p.m_y << ", " << p.m_z << ")";
		return os;
	}

	bool Vector::operator==(const Vector& p) const
	{
		return (m_x == p.m_x) && (m_y == p.m_y) && (m_z == p.m_z);
	}

	Vector Vector::operator-(const Vector& p) const
	{
		return { m_x - p.m_x, m_y - p.m_y, m_z - p.m_z };
	}

	Vector Vector::operator+(const Vector& p) const
	{
		return { m_x + p.m_x, m_y + p.m_y, m_z + p.m_z };
	}

	Vector Vector::operator*(double d) const
	{
		return { m_x * d, m_y * d, m_z * d };
	}

	double Vector::operator*(const Vector& p) const
	{
		return m_x * p.m_x + m_y * p.m_y + m_z * p.m_z;
	}

	double Vector::sqr_norm() const
	{
		return (*this) * (*this);
	}

	double Vector::norm() const
	{
		return sqrt(sqr_norm());
	}

	double Segment::sqr_len() const
	{
		return (m_end - m_start).sqr_norm();
	}

	double Segment::len() const
	{
		return (m_end - m_start).norm();
	}
}