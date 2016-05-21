#ifndef MATRIX_HEADER
#define MATRIX_HEADER

#include "Vector3D.hpp"

class Matrix
{
  friend class Vector3D;

 private:
   double *vals;/* [i*n+j]*/
   int n/* number of columns */,m/* number of rows */;

 public:
   Matrix(const Matrix&mat);
   Matrix(int m,int n);
   Matrix& operator=(const Matrix&mat);
   Matrix operator*(const Matrix & m) const;
   Matrix operator*(double s) const;
   Matrix& operator*=(double s);
   void mul(const Vector3D& v,Vector3D & result) const;

   void set(int i,int j,double val);
   int getRowCount() const; 
   int getColCount() const;
};


#endif