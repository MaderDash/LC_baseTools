#ifndef textBuff_h
#define textBuff_h

#include <Arduino.h>



//****************************************************************************************
// textBuff:
// Lets say you have something that streams out text and you need a place to store the
// stream before you can process it. This happens when hardware dumps data and you have no
// control over when and how much. This gives a place to stream all that text. Then you
// can read it back and deal with it at your leisure. You can even read it out as its
// being streamed in. That's pretty handy!
//****************************************************************************************


class textBuff {

  public:
            textBuff(int inNumBytes,bool inOverwrite=false);
            ~textBuff(void);

            
            bool  addChar(char inChar);						        // Add a char.
            bool  addStr(char* inCStr,bool andNULL=true);	// Add a c string. (With or without the nul terminator.)
            char  peekHead(void);									        // Look at the next char to come out.
            char  readChar(void);									        // Read out the next char. (removes it)
            int	buffSize(void);									          // How many chars CAN we store?
            int   numChars(void);									        // How many chars ARE we storing?
            bool  empty(void);										        // Are we empty?
            bool  full(void);											        // Are we full?
            void  clear(void);										        // Dump all the chars, reset to empty.
            
	private:  
				void  inc(int* index);
				       
            char* buff;
            int   numBytes;
            int   head;
            int   tail;
            bool  isFull;
            bool	overwrite;           
};

#endif