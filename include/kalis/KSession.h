// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : Session.h                                                 *
// * Description : internal class which is in charge of :             *
// *                  - memory allocation and desallocation           *
// *                  - license management                            *
// ********************************************************************

#ifndef __KSESSION_H
#define __KSESSION_H

#include "Globals.h"


/** @doc
 *	Nothing can be done in Artelys Kalis outside a KSession object. All the problems stated and solved must be held by such an object : for this reason the creation of a KSession object is the first thing to do at the beginning of the program.
 *	The KSession class has one main functionality:
 *	<ul><li> license checking : when created, the KSession object will look for a valid license of the software </ul>
 *	The syntax for the creation of a KSession object is the following :
 *	@code
 *	KSession mySession(false);
 *	@endcode
 *	This statement creates a KSession object variable named mySession with no printing of the banner. 
 *	We could have created our KSession using this syntax :
 *	@code
 *	KSession mySession;
 *	@endcode
 *	In this case, the banner would have been printed.
 *	 @version 2013.1
 */
class DLLIMPORTEXPORT KSession {
 private :
	char * _debugServerAddress;
	int _debugServerPort; 
	bool _managedMode;
 public:
	
	KSession(KSession &sessionToCopy); 
	/// Default constructor , printBanner activates the printing of the banner
	KSession(bool printBanner=true); 
	/// Constructor with string license
	KSession(unsigned char* passwd,bool printBanner=true); 
	/// return current version of library
	float getVersion(void) const;
	//destructor
	virtual ~KSession();
	// Set the debug server address and port information
	void setDebugServerAddress(char * hostname,int port);
	// Return the hotname of the debug server
	char * getDebugServerAddress();
	// Return the port on wich to connect on the host debug server
	int getDebugServerPort();

	bool getManagedMode();

}; 
#endif


