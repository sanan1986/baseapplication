#ifndef DISPATCH_H
#define DISPATCH_H

/******************************************************
*				INCLUDES
********************************************************/

/******************************************************
*				FORWARD DECLARATIONS
********************************************************/
class Client;

/******************************************************
*				CLASS
********************************************************/
class Dispatch
{
public:
	Dispatch();
	~Dispatch();

/**************************************************
*			VARIABLES
**************************************************/

	int				mMaxSize;
	int				mSize;
	int				mReadCount;
	char*			mCharArray;

/**************************************************
*			METHODS
**************************************************/
	//admin

	int             getReadCount() { return mReadCount; }
	void			BeginReading();
	char			*GetNewPoint(int length);
	void			Clear();
	int				GetSize()		{ return mSize; }
	void			SetSize(int s)		{ mSize = s; }

	//write
	void			WriteByte  (char c);
	void			WriteShort (short c);
	void			WriteFloat (float c);

	//read
	char			ReadByte   ();
	short			ReadShort  ();
	float			ReadFloat  ();

};



#endif
