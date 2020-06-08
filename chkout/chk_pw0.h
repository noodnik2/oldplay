/*

	chk_pw0.h		Group and User Password File Access

*/


#include	<pwd.h>
#include	<grp.h>

struct passwd *getpwnam() ;
struct group *getgrgid() ;
struct group *getgrnam() ;

