#include		<dos.h>

unsigned dataseg()
{
struct SREGS sregs ;

segread(&sregs);
return(sregs.ds);
}
