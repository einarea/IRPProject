// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KIntSetIntAnnotation                                      *
// ********************************************************************

#ifndef __KINTSETINTANNOTATION_H
#define __KINTSETINTANNOTATION_H

#include "Globals.h"

class DLLIMPORTEXPORT KIntSetIntAnnotation {
 
 private:

  StoredValArray<int> * _map; 
  int _offset;
  KIntSetIntAnnotation();
  
 public:

  //constructors
  KIntSetIntAnnotation(KProblem * p,char * name,int binf,int bsup,int init=0);

  //destructor
  ~KIntSetIntAnnotation();
  
  int  getIntAnnotation(int val);
  void setIntAnnotation(int val,int annot);  
  int getIndexInf();
  int getIndexSup();
  int getSize();
  void print();

  #ifdef ACK_LIB
  KIntSetIntAnnotation(KProblem_I * p,char * name,int binf,int bsup);
  KIntSetIntAnnotation(KProblem_I * p,char * name,int binf,int bsup,int init);
  #endif

};



class DLLIMPORTEXPORT KNumAnnotation {
public :
  
  virtual double  getAnnotation()=0;  
  virtual void setAnnotation(double value)=0;    
  virtual void print()=0;
};


class DLLIMPORTEXPORT KIntAnnotation : public KNumAnnotation {
 
 private:

  StoredVal<int> * _map;
  
  KIntAnnotation();  

 public:

  //constructors
 
  KIntAnnotation(KProblem * p,char * name,int init=0);

  //destructor
  ~KIntAnnotation();  
 
  
  //virtual int  getAnnotation();
  virtual void setAnnotation(double value);    
  virtual double  getAnnotation();  

  virtual void print();

  #ifdef ACK_LIB
  KIntAnnotation(KProblem_I * p,char * name,int init=0);  
  #endif

};


class DLLIMPORTEXPORT KDoubleAnnotation : public KNumAnnotation{
 
 private:

  StoredVal<double> * _map;

  KDoubleAnnotation();
  
 public:

  //constructors
  KDoubleAnnotation(KProblem * p,char * name,double init=0.0);

  //destructor
  ~KDoubleAnnotation();  
  //virtual int getAnnotation();
  virtual double getAnnotation();  
  virtual void setAnnotation(double value);    
  virtual void print();

  #ifdef ACK_LIB
  KDoubleAnnotation(KProblem_I * p,char * name,double init=0.0);  
  #endif

};

#endif


