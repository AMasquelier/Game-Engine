#include "math.h"

Matrix::Matrix()
{
	_w = 0; _h = 0;
}

Matrix::~Matrix()
{
	if (_mat != nullptr)
	{
		for (int i = 0; i < _w; i++) delete _mat[i];
		delete _mat;
	}
}

Matrix::Matrix(const Matrix &m)
{
	if (m.is_defined())
	{
		if (_mat != nullptr)
		{
			for (int i = 0; i < _w; i++) delete _mat[i];
			delete _mat;
		}

		_w = m.get_w(); _h = m.get_h();
		
		_mat = new double*[_w];
		for (int i = 0; i < _w; i++)
			_mat[i] = new double[_h];

		for (int i = 0; i < _w; i++)
			for (int j = 0; j < _h; j++)
				_mat[i][j] = m.get(i, j);
	}
}

Matrix::Matrix(int w, int h)
{
	if (w > 0 && h > 0)
	{
		if (_mat != nullptr)
		{
			for (int i = 0; i < _w; i++) delete _mat[i];
			delete _mat;
		}

		_w = w; _h = h;

		_mat = new double*[w];
		for (int i = 0; i < w; i++)
			_mat[i] = new double[h];

		for (int i = 0; i < w; i++)
			for (int j = 0; j < h; j++)
				_mat[i][j] = 0;
	}
}

Matrix::Matrix(int w, int h, double v)
{
	if (w > 0 && h > 0)
	{
		if (_mat != nullptr)
		{
			for (int i = 0; i < _w; i++) delete _mat[i];
			delete _mat;
		}

		_w = w; _h = h;

		_mat = new double*[w];
		for (int i = 0; i < w; i++)
			_mat[i] = new double[h];

		for (int i = 0; i < w; i++)
			for (int j = 0; j < h; j++)
				_mat[i][j] = v;
	}

}

void Matrix::fill(double v)
{
	if (is_defined())
	{
		for (int i = 0; i < _w; i++)
			for (int j = 0; j < _h; j++)
				_mat[i][j] = v;
	}
}

double Matrix::get(int i, int j) const
{
	if (i < _w && j < _h)
	{
		return _mat[i][j];
	}
	return 0.0;
}

void Matrix::set(int i, int j, double v)
{
	if (i < _w && j < _h)
	{
		_mat[i][j] = v;
	}
}

void Matrix::copy(const Matrix & m)
{
	if (m.get_w() > 0 && m.get_h() > 0)
	{
		if (_mat != nullptr)
		{
			for (int i = 0; i < _w; i++) delete _mat[i];
			delete _mat;
		}

		_w = m.get_w(); _h = m.get_h();

		_mat = new double*[_w];
		for (int i = 0; i < _w; i++)
			_mat[i] = new double[_h];

		for (int i = 0; i < _w; i++)
			for (int j = 0; j < _h; j++)
				_mat[i][j] = m.get(i, j);
	}
}

int Matrix::get_w() const
{
	return _w;
}

int Matrix::get_h() const
{
	return _h;
}

bool Matrix::is_defined() const
{
	return (_mat != nullptr && _w > 0 && _h > 0);
}



Matrix &Matrix::operator=(Matrix m) throw()
{
	if (_mat != nullptr)
	{
		for (int i = 0; i < _w; i++) delete _mat[i];
		delete _mat;
	}

	_w = m.get_w(); _h = m.get_h();

	_mat = new double*[_w];
	for (int i = 0; i < _w; i++)
		_mat[i] = new double[_h];

	for (int i = 0; i < _w; i++)
		for (int j = 0; j < _h; j++)
			_mat[i][j] = m.get(i, j);
	
	return *this;
}

Matrix &Matrix::operator*=(const Matrix &m)
{
	Matrix buf(m.get_w(), _h);

	if (_w == m.get_h() && _mat != nullptr && m.is_defined())
	{
		for (int i = 0; i < m.get_w(); i++)
			for (int j = 0; j < _h; j++)
				for (int k = 0; k < m.get_h(); k++)
					buf.set(i, j, buf.get(i, j) + _mat[k][j] * m.get(i, k));
	}
	copy(buf);
	return *this;
}

Matrix &Matrix::operator+=(const Matrix &m)
{
	if (_mat != nullptr && m.is_defined())
	{
		for (int i = 0; i < _w; i++)
			for (int j = 0; j < _h; j++)
				_mat[i][j] += m(i, j);
	}
	return *this;
}

Matrix &Matrix::operator+=(double v)
{
	if (_mat != nullptr)
	{
		for (int i = 0; i < _w; i++)
			for (int j = 0; j < _h; j++)
				_mat[i][j] += v;
	}
	return *this;
}

Matrix &Matrix::operator-=(const Matrix &m)
{
	if (_mat != nullptr && m.is_defined())
	{
		for (int i = 0; i < _w; i++)
			for (int j = 0; j < _h; j++)
				_mat[i][j] -= m(i, j);
	}
	return *this;
}

Matrix &Matrix::operator-=(double v)
{
	for (int i = 0; i < _w; i++)
		for (int j = 0; j < _h; j++)
			_mat[i][j] -= v;
	return *this;
}

Matrix &Matrix::operator^=(int p)
{
	for (int i = 0; p > 0 && i < p - 1; i++)
		(*this) *= (*this);
	return *this;
}

double *Matrix::operator[](int i)
{
	if(_w > 0)
		return _mat[i];
	return nullptr;
}

double Matrix::operator()(int i, int j) const
{
	if (i < _w && j < _h)
	{
		return _mat[i][j];
	}
	return 0.0;
}

Matrix operator*(const Matrix &m1, const Matrix &m2)
{
	Matrix buf(m2.get_w(), m1.get_h());

	if (m1.get_w() == m2.get_h())
	{
		for (int i = 0; i < m2.get_w(); i++)
			for (int j = 0; j < m1.get_h(); j++)
				for (int k = 0; k < m2.get_h(); k++)
					buf.set(i, j, buf.get(i, j) + m1.get(k, j) * m2.get(i, k));
	}
	return buf;
}

Matrix operator+(const Matrix &m1, const Matrix &m2)
{
	Matrix buf = m1;

	if (m1.get_w() == m2.get_w() && m1.get_h() == m2.get_h() && m1.is_defined() && m2.is_defined())
	{
		for (int i = 0; i < m1.get_w(); i++)
			for (int j = 0; j < m1.get_h(); j++)
				buf[i][j] += m2(i, j);
	}

	return buf;
}

Matrix operator+(const Matrix &m, double v)
{
	Matrix buf = m;

	if (m.is_defined())
	{
		for (int i = 0; i < m.get_w(); i++)
			for (int j = 0; j < m.get_h(); j++)
				buf[i][j] += v;
	}

	return buf;
}

Matrix operator-(const Matrix &m1, const Matrix &m2)
{
	Matrix buf = m1;

	if (m1.get_w() == m2.get_w() && m1.get_h() == m2.get_h())
	{
		for (int i = 0; i < m1.get_w(); i++)
			for (int j = 0; j < m1.get_h(); j++)
				buf[i][j] -= m2(i, j);
	}

	return buf;
}

Matrix operator-(const Matrix &m, double v)
{
	Matrix buf = m;

	if (m.is_defined())
	{
		for (int i = 0; i < m.get_w(); i++)
			for (int j = 0; j < m.get_h(); j++)
				buf[i][j] -= v;
	}

	return buf;
}

Matrix operator^(const Matrix &m, int p)
{
	Matrix buf = m;

	for (int i = 0; p > 0 && i < p - 1; i++)
		buf *= buf;

	return buf;
}

std::ostream & operator<<(std::ostream &out, const Matrix &m)
{
	for (int i = 0; i < m.get_h(); i++)
	{
		for (int j = 0; j < m.get_w(); j++)
		{
			out << m.get(j, i) << " ";
			
		}
		out << std::endl;
	}
	return out;
}



// Vector
Vector::Vector()
{
}

Vector::~Vector()
{
	if (_vec != nullptr)
	{
		delete _vec;
	}
}

Vector::Vector(const Vector & v)
{
	if (v.is_defined())
	{
		if (_vec != nullptr)
			delete _vec;

		_size = v.get_size();

		_vec = new double[_size];

		for (int i = 0; i < _size; i++)
			_vec[i] = v.get(i);
	}
}

Vector::Vector(int s)
{
	if (s > 0)
	{
		if (_vec != nullptr)
			delete _vec;

		_size = s;
		_vec = new double[_size];

		for (int i = 0; i < _size; i++)
			_vec[i] = 0;
	}
}

Vector::Vector(int s, double v)
{
	if (s > 0)
	{
		if (_vec != nullptr)
			delete _vec;

		_size = s;
		_vec = new double[_size];

		for (int i = 0; i < _size; i++)
			_vec[i] = v;
	}
}

void Vector::load(int size, ...)
{
	if (size > 0)
	{
		va_list ap;

		if (_vec != nullptr)
			delete _vec;

		_size = size;
		_vec = new double[_size];

		va_start(ap, size);
		do
		{
			_vec[_size-size] = va_arg(ap, double);
			size--;
		} while (size > 1);

		va_end(ap);
	}
}

void Vector::reserve(int size)
{
	if (size > 0)
	{
		if (_vec != nullptr)
			delete _vec;

		_size = size;
		_vec = new double[_size];

		for (int i = 0; i < _size; i++)
			_vec[i] = 0;
	}
}

	// Return vector size
int Vector::append(double v)
{
	double *buf = (double*)realloc(_vec, _size+1);
	
	if (buf != nullptr)
	{
		_vec = buf;
		_vec[_size] = v;
		_size++;
	}
	return _size;
}

void Vector::fill(double v)
{
	if (is_defined())
	{
		for (int i = 0; i < _size; i++)
			_vec[i] = v;
	}
}

void Vector::set(int i, double v)
{
	if (is_defined())
	{
		_vec[i] = v;
	}
}

void Vector::copy(const Vector & v)
{
	if (v.is_defined())
	{
		if (_vec != nullptr)
			delete _vec;

		_size = v.get_size();

		_vec = new double[_size];

		for (int i = 0; i < _size; i++)
			_vec[i] = v.get(i);
	}
}

double Vector::get(int i) const
{
	return _vec[i];
}

inline int Vector::get_size() const
{
	return _size;
}

bool Vector::is_defined() const
{
	return (_vec != nullptr && _size > 0);
}

double Vector::get_norm_sqr()
{
	double n = 0;
	for (int i = 0; i < _size; i++)
		n += _vec[i] * _vec[i];
	return n;
}

Vector & Vector::operator=(Vector v)
{
	if (v.is_defined())
	{
		if (_vec != nullptr)
			delete _vec;

		_size = v.get_size();

		_vec = new double[_size];

		for (int i = 0; i < _size; i++)
			_vec[i] = v.get(i);
	}

	return *this;
}

Vector & Vector::operator+=(const Vector & v)
{
	if (v.is_defined() && is_defined())
	{
		for (int i = 0; i < _size; i++)
			_vec[i] += v.get(i);
	}
	return *this;
}

Vector & Vector::operator+=(double v)
{
	if (is_defined())
	{
		for (int i = 0; i < _size; i++)
			_vec[i] += v;
	}
	return *this;
}

Vector & Vector::operator-=(const Vector & v)
{
	if (v.is_defined() && is_defined())
	{
		for (int i = 0; i < _size; i++)
			_vec[i] -= v.get(i);
	}
	return *this;
}

Vector & Vector::operator-=(double v)
{
	if (is_defined())
	{
		for (int i = 0; i < _size; i++)
			_vec[i] -= v;
	}
	return *this;
}

Vector & Vector::operator*=(double v)
{
	if (is_defined())
	{
		for (int i = 0; i < _size; i++)
			_vec[i] *= v;
	}
	return *this;
}

double &Vector::operator[](int i)
{
	return _vec[i];
}

double Vector::operator()(int i) const
{
	if (i < _size && is_defined()) return _vec[i];
	return 0;
}

Vector operator*(const Vector & vec, double v)
{
	Vector buf(vec.get_size());

	if (vec.is_defined())
	{
		for (int i = 0; i < vec.get_size(); i++)
				buf[i] = vec.get(i) * v;
	}

	return buf;
}

Vector operator*(const Matrix & m, const Vector & v)
{
	Vector buf(v.get_size());

	if (v.is_defined() && m.is_defined() && v.get_size() == m.get_w())
	{
		for (int i = 0; i < m.get_h(); i++)
		{
			for (int j = 0; j < v.get_size(); j++)
				buf[i] += v.get(j) * m(i, j);
		}
	}

	return buf;
}

Vector operator+(const Vector & v1, const Vector & v2)
{
	Vector buf = v1;

	if (v1.is_defined() && v2.is_defined())
	{
		for (int i = 0; i < v1.get_size(); i++)
			buf[i] += v2.get(i);
	}

	return buf;
}

Vector operator+(const Vector & vec, double v)
{
	Vector buf = vec;

	if (vec.is_defined())
	{
		for (int i = 0; i < vec.get_size(); i++)
			buf[i] += v;
	}

	return buf;
}

Vector operator-(const Vector & v1, const Vector & v2)
{
	Vector buf = v1;

	if (v1.is_defined() && v2.is_defined())
	{
		for (int i = 0; i < v1.get_size(); i++)
			buf[i] -= v2.get(i);
	}

	return buf;
}

Vector operator-(const Vector & vec, double v)
{
	Vector buf = vec;

	if (vec.is_defined())
	{
		for (int i = 0; i < vec.get_size(); i++)
			buf[i] -= v;
	}

	return buf;
}

std::ostream & operator<<(std::ostream & out, const Vector & v)
{
	if (v.is_defined())
	{
		out << "(";
		for (int i = 0; i < v.get_size(); i++)
		{
			if (i > 0) out << ", ";
			else out << " ";
			out << v(i);
		}
		out << " )";
	}
	return out;
}

