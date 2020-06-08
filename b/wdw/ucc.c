#include		"ucc.h"

						/* External library routines */
char *memcpy() ;

void ucccpy(ucto, ucfrom)
struct uccoord *ucto, *ucfrom ;
{
memcpy(ucto,ucfrom,sizeof(struct uccoord));
}

void uccsum(ucto, uco1, uco2)
struct uccoord *ucto, *uco1, *uco2 ;
{
ucto->y = (unsigned char)(uco1->y + uco2->y);
ucto->x = (unsigned char)(uco1->x + uco2->x);
}

void uccsub(ucto, uco1, uco2)
struct uccoord *ucto, *uco1, *uco2 ;
{
ucto->y = (unsigned char)(uco1->y - uco2->y);
ucto->x = (unsigned char)(uco1->x - uco2->x);
}
