// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KMessage.h                                                *
// * Description : message base class and derived classes             *
// ********************************************************************

#ifndef __KMESSAGE_H
#define __KMESSAGE_H

#include "Globals.h"

class DLLIMPORTEXPORT KMessage : public KExternalObject<KMessage_I> {
public:
	KMessage();
	KMessage(KMessage_I* message);
	KMessage(const KMessage& toCopy);
	virtual ~KMessage();
	void setSender(int sender);
	int getSender() const;
};

class DLLIMPORTEXPORT KFinishedMessage : public KMessage {
public:
	KFinishedMessage();
	KFinishedMessage(const KFinishedMessage& toCopy);
};

#endif
