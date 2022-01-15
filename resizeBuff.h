#ifndef resizeBuff_h
#define resizeBuff_h

#include "Arduino.h"



//****************************************************************************************
//                   TOOLS FOR THOSE THAT DON'T FEAR DYNAMIC MEMORY 	
//                IF YOU CAN'T STAND THE HEAT, GET OUT OF THE KITCHEN!
//****************************************************************************************



//****************************************************************************************
// resizeBuff : 
//
// Ok, what is this resizeBuff thing?
// 
// Forever I found myself writing..
// 
// if (buff) {
// 	free(buff);
// 	buff = NULL;
// }
// buff = malloc(newSize);
// if (buff) {
// 	do stuff..
// }
// 
// Over and over. Then one day I wrote my first version of a "stretchy buffer" and that
// got me thinking.. Is there a way to generalize this idea? 
// 
// Hence : resizeBuff()
// 
// As long as your buffer starts life allocated, or with a NULL. You can use this quick
// call to resize it as much as you like. And, it returns a boolean true or false as to if
// it was able to do it or not. Keeps you from writing into unallocated RAM as it were.
// 
// NOTE: Resizing still means you loose whatever data was originally in it.
// 
// Now its just..
// 
// if (resizeBuff(newSize,&buffPtr)) {		// Notice?
// 	do stuff..									// Address of the pointer there..
// }													// Eww! So tricky!
// 
// 
// NOTE : Why a pointer to uint8_t?
// Originally this was setup at char* because it started with c strings. Then It started
// being used for binary data blocks. Then binary data blocks crossing over serial to
// other processors.. Word size stuff bit me in the.. Foot. uint8_t  forced buffer sizes
// to match up.
//****************************************************************************************

extern bool resizeBuff(int numBytes,uint8_t** buff);
extern bool resizeBuff(int numBytes,char** buff);
extern bool resizeBuff(int numBytes,void** buff);



//****************************************************************************************
// maxBuff :
//
// Lets say you need to write to something with, possibly, more data than you can allocate
// at one time? What to do? I ran into this issue when I found myself needing to transfer
// data from one file to another. The data could be WAY larger than the available RAM. 
// And, as always, I was in a terrific hurry. So doing it byte by byte was not going to
// cut it. Hence, maxBuff.
//
// This class will start at your maximum desired buffer size and try allocating it. If
// that fails, it tries for half that size. Then 1/3 the size, 1/4.. Until either it
// succeeds in allocating a buffer, or, hits the buffer minimum size and gives up.
//
// On success : 
// theBuff is the pointer to your allocated buffer.
// numBuffBytes is the size in bytes of that buffer.
// numPasses is the number of passes it will take to write/read your desired data.
// Meaning? This is how many time to call it in a for loop.
//
// On failure :
// theBuff will equal NULL.
//
// NOTE : This should be allocated as a local variable. Then on exit, it will recycle the
// buffer. Pretty slick huh?
//****************************************************************************************


#define BYTE_CUTOFF 20

class maxBuff {

	public:
				maxBuff(unsigned long numBytes,unsigned long  minBytes=BYTE_CUTOFF);
	virtual	~maxBuff(void);
	
				void*				theBuff;
				unsigned long	numBuffBytes;
				int				numPasses;
	};
	


//****************************************************************************************
// heapStr():
//
// In the good old days people used to do..
//
// char* aStr = "Look data!";
//
// And that would allocate room for the text and put there. Sadly it would only do it
// once. Now, wouldn't that be nice if you could do it repeatedly? Now you can!
//
// char* aStr = NULL;									// ALLWAYS initialize at NULL for this.
//
// heapStr(&aStr,"Look data!");						// Allocates and stuffs it in.
// heapStr(&aStr,"Look longer data!");				// Recycles, re-allocates and writes.
// heapStr(&aStr,"Can be called as needed.");	// Get the picture?
// freeStr(&aStr);										// Just recycles and sets back to NULL.
//****************************************************************************************


extern bool heapStr(char** resultStr,const char* inStr);
extern void freeStr(char** resultStr);



//****************************************************************************************
// tempStr :
//
// Ok, take all these bits to their limit and make the easiest thing to use for a quick
// string buffer. The tempStr class is a stack based class. Meaning? It will store your
// string only until it goes out of scope, and then it automatically recycles the memory
// for you.
//
// tempStr myStr(readThing(pinNum));   // Copy the output of a string function.
// 
// -or-
//
// tempStr myStr;								// Or an empty one. Ready to save a string.
//
// myStr.setStr(readThing(pinNum));		// You can save a string later. Or reuse.
// formatAndDisplay(myStr.getStr());	// We don't worry about what the readThing() does.
//													// We now have a local copy.
//
// return;										// When tempStr goes out of scope, It recycles.
//****************************************************************************************


class tempStr {

	public:
				tempStr(char* inStr=NULL);
	virtual	~tempStr(void);
	
				void	setStr(char* inStr);
				char* getStr(void);
				
				char*	theStr;
};
		
#endif
