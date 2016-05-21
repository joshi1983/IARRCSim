#include "Matrix.hpp"


#include <cstdlib>
#include <iostream>

using namespace std;

Matrix::Matrix(const Matrix &mat): m(mat.m),n(mat.n)
{
  vals=(double*)malloc(n*m*sizeof(double));

  // copy the values over.
  for (int i=m*n-1;i>=0;i--)
  {
     vals[i]=mat.vals[i];
  }
}

Matrix::Matrix(int m,int n): vals((double*)malloc(n*m*sizeof(double)))
  ,m(m),n(n)
{
  if (vals==NULL)
  {
     this->m=0;
	this->n=0;
	cout << "Unable to allocate memory for matrix of "
		<<m<<" by "<<n<<endl;
	return;
  }

  // copy the values over.
  for (int i=m*n-1;i>=0;i--)
  {
     vals[i]=0;
  }
}

Matrix & Matrix::operator=(const Matrix&mat)
{
  if (n*m!=mat.m*mat.n)
  {
     free(vals);
     vals=(double*)malloc(mat.n*mat.m*sizeof(double));
  }
  this->n=mat.n;
  this->m=mat.m;

  // copy the values over.
  for (int i=m*n-1;i>=0;i--)
  {
     vals[i]=mat.vals[i];
  }
  return *this;
}

Matrix Matrix::operator*(const Matrix & m) const
{
 Matrix result(this->m,m.n);

  // loop through rows of the result.
  for (int i=0;i<result.m;i++)
  {
    // loop through columns of the result.
    for (int j=0;j<result.n;j++)
    {
      double val=0;

       for (int x=0;x<n;x++)
          val+=vals[i*this->n+x]*m.vals[x*m.n+j];

       result.vals[i*result.n+j]=val;
    }
  }

  return result;
}

Matrix Matrix::operator*(double s) const
{
 Matrix result(m,n);

  for (int i=m*n-1;i>=0;i++)
  {
     result.vals[i]=vals[i]*s;
  }

  return result;
}

Matrix& Matrix::operator*=(double s)
{
  for (int i=m*n-1;i>=0;i++)
  {
     vals[i]*=s;
  }

  return *this;
}

void Matrix::set(int i,int j,double val)
{
  if (i<0 || i>=n || j<0 || j>=m)
	return; // do not set anything if i,j are out of range.

  vals[i*n+j]=val;
}

int Matrix::getRowCount() const
{
  return m;
}

int Matrix::getColCount() const
{
  return n;
}

void Matrix::mul(const Vector3D& v,Vector3D & result) const
{
 if (m!=3 || n!=3)
 {
    cout << "mul requires that m=3 and n=3."<<endl;
    return;
 }
  double x,y,z;

  x=vals[0]*v.x+vals[1]*v.y+vals[2]*v.z;
  y=vals[3]*v.x+vals[4]*v.y+vals[5]*v.z;
  z=vals[6]*v.x+vals[7]*v.y+vals[8]*v.z;

  result.set(x,y,z);
}


