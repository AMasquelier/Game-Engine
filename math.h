#pragma once
#include <iostream>
#include <string>
#include <stdarg.h>

static float PI = 3.14159265;

// Algebra
class Matrix
{
	public :
		Matrix();
		~Matrix();
		Matrix(const Matrix &m);
		Matrix(int w, int h);
		Matrix(int w, int h, double v);
		void fill(double v);
		void set(int i, int j, double v);
		void copy(const Matrix &m);
		inline double get(int i, int j) const;
		inline int get_w() const;
		inline int get_h() const;
		bool is_defined() const;
		

		inline Matrix &operator=(Matrix m);
		inline Matrix &operator*=(const Matrix &m);
		inline Matrix &operator+=(const Matrix &m);
		inline Matrix &operator+=(double v);
		inline Matrix &operator-=(const Matrix &m);
		inline Matrix &operator-=(double v);
		inline Matrix &operator^=(int p);

		inline double *operator[](int i);
		inline double operator()(int i, int j) const;

		friend Matrix operator*(const Matrix &m1, const Matrix &m2);
		friend Matrix operator+(const Matrix &m1, const Matrix &m2);
		friend Matrix operator+(const Matrix &m, double v);
		friend Matrix operator-(const Matrix &m1, const Matrix &m2);
		friend Matrix operator-(const Matrix &m, double v);
		friend Matrix operator^(const Matrix &m, int p);
		friend std::ostream &operator<<(std::ostream &out, const Matrix &m);
		

	private :
		double **_mat = nullptr;
		int _w = 0, _h = 0;

};

class Vector
{
	public:
		Vector();
		~Vector();
		Vector(const Vector &v);
		Vector(int s);
		Vector(int s, double v);
		void load(int size, ...);
		void reserve(int size);
		int append(double v);
		void fill(double v);
		void set(int i, double v);
		void copy(const Vector &v);
		double get(int i) const;
		inline int get_size() const;
		bool is_defined() const;
		double get_norm_sqr();

		Vector &operator=(Vector v);
		Vector &operator+=(const Vector &v);
		Vector &operator+=(double v);
		Vector &operator-=(const Vector &v);
		Vector &operator-=(double v);
		Vector &operator*=(double v);

		double &operator[](int i);
		double operator()(int i) const;

		friend Vector operator*(const Vector &vec, double v);
		friend Vector operator*(const Matrix &m, const Vector &v);
		friend Vector operator+(const Vector &v1, const Vector &v2);
		friend Vector operator+(const Vector &vec, double v);
		friend Vector operator-(const Vector &v1, const Vector &v2);
		friend Vector operator-(const Vector &vec, double v);
		friend std::ostream &operator<<(std::ostream &out, const Vector &v);


	private:
		double *_vec = nullptr;
		int _size = 0;
};


inline double dot_product(const Vector &a, const Vector &b)
{
	int n = a.get_size();
	double res = 0;

	if (a.get_size() == b.get_size())
	{
		for (int i = 0; i < n; i++)
			res += a(i) * b(i);
	}
	return res;
}

inline Vector comp_product(const Vector &v1, const Vector &v2)
{
	int n = v1.get_size();
	Vector buf(n);

	if (v1.get_size() == v2.get_size())
	{
		for (int i = 0; i < n; i++)
			buf[i] = v1(i) * v2(i);
	}

	return buf;
}

// Geometry
inline double dist2(Vector x, Vector y)
{
	return (x-y).get_norm_sqr();
}

inline float normalizeRadians0_2PI(float a)
{
	if (a < 0) a = 2 * PI + a;
	if (a >= 2 * PI) a = a - int(a / (2 * PI)) * 2 * PI;
	return a;
}

inline float normalizeRadiansPI_PI(float a)
{
	if (a < -PI) a = 2 * PI + a;
	if (a > PI) a = -2 * PI + a;
	return a;
}

inline float radialDistance(float a, float b)
{
	float a1 = normalizeRadians0_2PI(a);
	float b1 = normalizeRadians0_2PI(b);

	float dist1 = fabs(a1 - b1);


	float a2 = normalizeRadiansPI_PI(a);
	float b2 = normalizeRadiansPI_PI(b);

	float dist2 = fabs(a2 - b2);

	return fmin(dist1, dist2);
}