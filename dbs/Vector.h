#pragma once

namespace dbs
{
	struct Vector
	{
		double m_x;
		double m_y;
		double m_z;

		bool operator==(const Vector& p) const;
		Vector operator-(const Vector& p) const;
		Vector operator+(const Vector& p) const;
		Vector operator*(double d) const;
		double operator*(const Vector& p) const;
		double sqr_norm() const;
		double norm() const;
	};

	std::ostream& operator<<(std::ostream& os, const Vector& p);

	struct Segment
	{
		Vector m_start;
		Vector m_end;

		double len() const;
		double sqr_len() const;
	};
}
