// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : Console routines                                          *
// ********************************************************************

#ifndef __CONSOLE_H
#define __CONSOLE_H

#include "Globals.h"

#define KCOL_BLACK   0
#define KCOL_DBLUE   1
#define KCOL_DGREEN  2
#define KCOL_DCYAN   3
#define KCOL_DRED    4
#define KCOL_DPURPLE 5
#define KCOL_DBROWN  6
#define KCOL_LGREY   7
#define KCOL_DGREY   8
#define KCOL_BLUE    9
#define KCOL_GREEN  10
#define KCOL_CYAN   11
#define KCOL_RED    12
#define KCOL_PURPLE 13
#define KCOL_YELLOW 14
#define KCOL_WHITE  15 

/**
 *  Utility class for text console managment 
 */
class DLLIMPORTEXPORT Console {
	int _SPX;
	int _SPY;
	unsigned int _defaultAttributes;
  public :
	void savePosition(void);
	/// return current X cursor position
	int getX(void);
	/// return current Y cursor position
	int getY(void);
	/// position the cursor to (x,y) coordinates
	void gotoxy(int x, int y);
	void gotoSavedPosition(void);	
	/// set the color of the printed text (background and foreground)
	void setColor(unsigned int col);
	/// set the Background color of printed text
	void setBackgroundColor(char col);
	/// set the Foreground color of printed text
	void setForegroundColor(char col);
	/// return the default console text attributes
	unsigned int getConsoleTextAttributes();
	/// restore the default console text attributes
	void restoreDefaultConsoleTextAttributes();
	/// Clear the screen
	void clearScreen(void);
	void fbox(int x,int y,int x1,int y1,bool relative);
	void split_h(int x,int x1,int y);
	void split_v(int y,int y1,int x);	
	void drawGrid(int x,int y,int col,int row,int w,int h);
};


#endif

