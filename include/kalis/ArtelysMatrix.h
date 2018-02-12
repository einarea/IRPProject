// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : ArtelysMatrix.h                                           *
// * Description : represents Artelys general template for a matrix of*
// * primary types int , double etc...                                *
// ********************************************************************

#ifndef __ARTELYS_MATRIX_H
#define __ARTELYS_MATRIX_H

#include <stdarg.h>
#include <stdlib.h>
#include "ArtelysException.h"



//************
//*** class Declaration
//************
//
template <class T>
class Container2DRow
{
public:
    T& operator [] (int j);
   // const T& operator [] (int j) const; 
    T **m_ppMatrix;
    int i; //ROW (Y coord)
    int m_nXSize;
};

/**
 * A Matrix template
 */
template <class T>
class ArtelysMatrix  
{
public:
    //Helper class for [][] indexing, it is not neccesarily 
    // to agragated by ArtelysMatrix it could be just a friend
    Container2DRow<T> row;

private:
    int m_nXSize;
    int m_nYSize;
    int m_nMemorySize;
    T **m_ppMatrix;

    bool m_bCreated;
public:
    //Constructor & Copy Constructor
    ArtelysMatrix(int nYSize, int nXSize);
    ArtelysMatrix(const ArtelysMatrix& matrix);

    //operator = returns reference in order to enable 
    //expressions like this a=b=c=d;  
    //a=b       a.operator=(b)
    //a=b+c     a.operator=(b.operator+(c));
    //a=b-c     a.operator=(b.operator-(c)); 
    ArtelysMatrix& operator= (const ArtelysMatrix& matrix);
    ArtelysMatrix  operator+ (const T& item);
    ArtelysMatrix  operator- (const T& item);

    //Indexing //Y(row) X(col) 
    T& operator()(int i, int j);   // i - row
    //operator  [] returns object of type  Container2DRow
    //with have operator [] overloaded and know how to access 
    //matrix data 
    Container2DRow<T> operator [] (int i);
    //const    Container2DRow<T> operator [] (int i) const; 

    //Helper functions, you can expand this section to do
    //LU decomposition, determinant evaluation and so on,  
    T SumAll();
    //Get Size
    int getN();
    int getM();
    T GetMinValue();
    T GetMaxValue();
    virtual ~ArtelysMatrix();

};

//#ifdef ACK_LIB

///Class container

/*
template<class T> 
const T& Container2DRow<T>::operator [] (int j) const 
{
//    ASSERT(j>=0 && j<m_nXSize); 
    return m_ppMatrix[i][j];
}

*/
template<class T> 
T& Container2DRow<T>::operator [] (int j) 
{
//    ASSERT(j>=0 && j<m_nXSize); 
	//printf("Here %i\n",j);
    return m_ppMatrix[i][j];
}

template<class T>
ArtelysMatrix<T>::ArtelysMatrix(int nYSize, int nXSize)
{
    m_bCreated = false;
 //   ASSERT(nXSize>0 && nYSize>0);


    m_nXSize = nXSize;
    m_nYSize = nYSize;
    m_nMemorySize = m_nYSize*m_nXSize*sizeof(T);

    m_ppMatrix    = new T* [m_nYSize];
    m_ppMatrix[0] = new T  [m_nYSize*m_nXSize];

    for (int i=1; i<m_nYSize; i++)
        m_ppMatrix[i] = m_ppMatrix[0]+i*m_nXSize;

    memset(m_ppMatrix[0], 0, m_nMemorySize);
    m_bCreated = true;
    row.m_ppMatrix = m_ppMatrix;
    row.m_nXSize   = m_nXSize;
}

template<class T>
ArtelysMatrix<T>::ArtelysMatrix(const ArtelysMatrix& matrix)
{	
	
    m_nXSize = matrix.m_nXSize;
    m_nYSize = matrix.m_nYSize;
		//printf("OK %i,%i\n",getN(),getM());
    m_nMemorySize = m_nYSize*m_nXSize*sizeof(T);

    m_ppMatrix    = new T* [m_nYSize];
//    ASSERT(m_ppMatrix!=NULL);

    m_ppMatrix[0] = new T  [m_nYSize*m_nXSize];
//    ASSERT(m_ppMatrix[0]!=NULL);

    for (int i=1; i<m_nYSize; i++)
        m_ppMatrix[i] = m_ppMatrix[0]+i*m_nXSize;

    row.m_ppMatrix = m_ppMatrix;
    row.m_nXSize   = m_nXSize;
    memcpy(m_ppMatrix[0],matrix.m_ppMatrix[0], m_nMemorySize);
	m_bCreated = true;
	

 
    
}


template<class T>
ArtelysMatrix<T>& ArtelysMatrix<T>::operator= (const ArtelysMatrix& matrix)
{
    if (this == &matrix) return *this;

    //ASSERT(m_nXSize == matrix.m_nXSize && m_nYSize == matrix.m_nYSize);
    memcpy(m_ppMatrix[0],matrix.m_ppMatrix[0], m_nMemorySize);

    return *this;
}
template<class T>
T ArtelysMatrix<T>::GetMinValue()
{
    T minValue = m_ppMatrix[0][0];
    int i,j;

    for (i=0; i<m_nYSize; i++)
        for (j=0; j<m_nXSize; j++)
        {
            if(m_ppMatrix[i][j]<minValue)
                minValue = m_ppMatrix[i][j];
        }
        return minValue;
}

template<class T>
T ArtelysMatrix<T>::GetMaxValue()
{
    T maxValue = m_ppMatrix[0][0];
    int i,j;

    for (i=0; i<m_nYSize; i++)
        for (j=0; j<m_nXSize; j++)
        {
            if(m_ppMatrix[i][j]>maxValue)
                maxValue = m_ppMatrix[i][j];
        }
        return maxValue;
}

template<class T>
ArtelysMatrix<T> ArtelysMatrix<T>::operator+ (const T& item)
{
    int i, j;

    ArtelysMatrix<T> mtrx(m_nYSize, m_nXSize);
    for (i=0; i<m_nYSize; i++)
        for (j=0; j<m_nXSize; j++)
        {
            mtrx.m_ppMatrix[i][j] = m_ppMatrix[i][j]+item ;
        }
        return mtrx;
}

template<class T>
ArtelysMatrix<T> ArtelysMatrix<T>::operator- (const T& item)
{
    int i, j;

    ArtelysMatrix<T> mtrx(m_nYSize, m_nXSize);
    for (i=0; i<m_nYSize; i++)
        for (j=0; j<m_nXSize; j++)
        {
            mtrx.m_ppMatrix[i][j] = m_ppMatrix[i][j]-item ;
        }
        return mtrx;
}

template<class T>
ArtelysMatrix<T>::~ArtelysMatrix()
{
    if (m_bCreated)
    {
        delete [] m_ppMatrix[0];
        delete [] m_ppMatrix;
    }
}

template<class T>
int ArtelysMatrix<T>::getM()
{
    return m_nXSize;
}

template<class T>
T ArtelysMatrix<T>::SumAll()
{
    T sum = 0;
    int i, j;

    for (i=0; i<m_nYSize; i++)
        for (j=0; j<m_nXSize; j++)
        {
            sum += m_ppMatrix[i][j];
        }
        return sum;
}

template<class T>
int ArtelysMatrix<T>::getN()
{
    return m_nYSize;
}
template<class T>        //Y(row) X(col)      
T& ArtelysMatrix<T>::operator()(int i, int j)
{
    //ASSERT(i>=0 && i<m_nYSize && j>=0 && j<m_nXSize);
    return m_ppMatrix[i][j];
}

//Fancy Indexing
template<class T> 
Container2DRow<T> ArtelysMatrix<T>::operator [] (int i)
{
    //ASSERT(i>=0 && i<m_nYSize); 
    row.i = i;
    return row;
}


/*
template<class T> 
const Container2DRow<T> ArtelysMatrix<T>::operator [] (int i) const
{
    //ASSERT(i>=0 && i<m_nYSize); 
    row.i = i;
    return row;
}
*/



//#endif // End of : #ifdef ACK_LIB

#endif // End of : #ifndef 



