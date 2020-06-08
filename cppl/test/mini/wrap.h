/*

*/

#if	__cplusplus
extern "C" {
#endif

void IniInit() ;
void IniFini() ;
void IniFlush() ;
void IniRead(char *fn, char *section, char *entry, char *output, int iMaxLen) ;
void IniWrite(char *fn, char *section, char *entry, char *input) ;

#if	__cplusplus
}
#endif

/* end of file */
