// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : ArtelysTimer.h                                            *
// * Description : Timer tools                                        *
// ********************************************************************

#ifndef ARTELYSTIMER
#define ARTELYSTIMER

#include "Globals.h"

#ifdef ACK_LIB
//#include "boost/thread/thread_time.hpp"
#endif

class DLLIMPORTEXPORT ArtelysTimer
{

public:

  ArtelysTimer();
  ~ArtelysTimer();

  // First call to the clock, counter starts
  void initialize();

  // Call to the clock
  void update();
  
  // Give program time between the start of the counter and the
  // last call to the clock
  // (in seconds)
  double get_total_time();

  // Give program time between the two last calls to the clock
  // (in seconds)
  double get_incremental_time();


private:
 
  // Last clock value
  double last_time;
  // Last date value (in Julian Day)
  double last_date;
  // Time increment between two clock calls
  double incremental_time;
  // Time since initialization
  double total_time;
#ifdef ACK_LIB
	// Last time value
//	boost::system_time last_time;
	// Time increment between two time calls
//	boost::posix_time::time_duration incremental_time;
	// Time since initialization
//	boost::posix_time::time_duration total_time;
#endif

}; // End of : class ArtelysTimer

#endif // End of : #ifndef ARTELYSTIMER

/////////////////////////////////////////////////////////////////
// End of file ArtelysTimer.h
/////////////////////////////////////////////////////////////////


