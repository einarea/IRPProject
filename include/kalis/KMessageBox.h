// ********************************************************************
// * Artelys Kalis                                                    *
// * Copyright (C) 2001-2013 by Artelys                               *
// * All Rights Reserved                                              *
// *                                                                  *
// * File : KMessageBox.h                                             *
// * Description : message box tool for KSolver objects               *
// ********************************************************************

#ifndef __KMESSAGEBOX_H
#define __KMESSAGEBOX_H

#include "Globals.h"
#include "ArtelysList.h"

class DLLIMPORTEXPORT KMessageBox : public KAutoExternalObject<KMessageBox,KMessageBox_I> {
public:
	KMessageBox();
	KMessageBox(int senderId);
	KMessageBox(KMessageBox_I* messageBox);
	KMessageBox(const KMessageBox& toCopy);
	virtual ~KMessageBox();
	KMessageBox* getCopyPtr() const; // for compatibility with ArtelysList
	void setRecipientMessageBoxes(const ArtelysList<KMessageBox>& recipientMessageBoxes);
	void sendMessage(KMessage& message, int recipient);
	void sendMessage(KMessage& message, NAMESPACE_STL::vector<int>& recipients); 
	bool hasUnreadMessages() const;
	const KMessage getFirstUnreadMessage();
	void waitForNextMessage();

	KMessageBox& box();
};

#endif
