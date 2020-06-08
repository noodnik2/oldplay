//	Revision:      		10

#ifndef	M_THROW_H_INC
#define	M_THROW_H_INC

/*

	mthrow.h 		qad-NewARC21 C++ Header: exception handling
				Copyright (C) 1995, Contahal Ltd.


	This file defines the exception-handling class.

*/

/*
	class definitions
*/

class M_Throw
{
	public:

		// NOTE: 'n' specifies beep style; platform-dependant
		static void BeepMessage(int n = 0) ;

		static void DisplayMessage(
			const char *pszMessage,
			const char *pszTitle = "Message"
		) ;

		static void FatalError(
			const char *pszErrorText,
			const char *pszTitle = "Fatal Error"
		) ;

		// returns 0 for No, 1 for Yes.
		static int YesNoQuestion(
			const char *pszQuestion,
			const char *pszTitle = "Question"
		) ;

} ;

// #ifndef M_THROW_H_INC
#endif

// end of mthrow.h
