// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : ArtelysException.h                                        *
// * Description : it represents Artelys Exception main class         *
// ********************************************************************

#ifndef __ARTELYS_EXCEPTION_H
#define __ARTELYS_EXCEPTION_H

#include "Globals.h"

/**	 
  * Exception class thrown by the Artelys Kalis library  
	@code
		Example :
		try {
			new KSession();
			...
		} catch (ArtelysException &artelysException) {
			printf("An exception occured : %s\n",artelysException.getMessage());
		}
	  @endcode	
  *	 @version 2013.1
*/
class DLLIMPORTEXPORT ArtelysException {
protected:
  char *  _message;
  int     _code;
  char *  _file; // In which file was the exception thrown
  int     _line; // At which line
  char *  _date; // When was it compiled
  char *  _time; // At which time
  char *  _debuggingInformation; // Concatenation of 4 previous informations

public:
  // Constructor
  ArtelysException();
  #ifndef VARARGIGNORE
  ArtelysException(const int code, const char* message, ...);
  ArtelysException(char* file, int line, char* date, char* time, const int code, const char* message, ...);
  #endif
  ArtelysException(const ArtelysException &exceptionToCopy);

  //destructor
  virtual ~ArtelysException();

  //'get' methods
  virtual const char* getMessage() const;
  virtual const int getCode() const;
  virtual const char * getDebuggingInformation() const; // It returns "'_file' at line '_line' compiled '_date' at '_time'

}; // class ArtelysException


#endif
