
/* start at 64 */

static char ascch[] = "\
 \0\0\0\0\0\0\0\0\0\
|.<(+|&\
\0\0\0\0\0\0\0\0\0\
!$*);^-/\
\0\0\0\0\0\0\0\0\
|,%_>?\0\0\0\0\0\0\0\0\0\
`:#@'=\"\0abcdefghi\0\0\0\0\0\0\0\
jklmnopqr\0\0\0\0\0\0\0\
~stuvwxyz\0\0\0[\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0]\
\0\0{ABCDEFGHI\0\0\0\0\0\0}\
JKLMNOPQR\0\0\0\0\0\0\\\0\
STUVWXYZ\0\0\0\0\0\0\
\060\06123456789" ; 

#if 0
#include <stdio.h>

main()
{ 
	verify(106,'|');
	verify(107,',');
	verify(108,'%');
	verify(109,'_');
	verify(110,'>');
	verify(111,'?');
	verify(121,'`');
	verify(122,':');
	verify(123,'#');
	verify(124,'@');
	verify(125,'\'');
	verify(126,'=');
	verify(127,'"');
	verify(129,'a');
	verify(130,'b');
	verify(131,'c');
	verify(132,'d');
	verify(133,'e');
	verify(134,'f');
	verify(135,'g');
	verify(136,'h');
	verify(137,'i');
	verify(145,'j');
	verify(146,'k');
	verify(147,'l');
	verify(148,'m');
	verify(149,'n');
	verify(150,'o');
	verify(151,'p');
	verify(152,'q');
	verify(153,'r');
	verify(161,'~');
	verify(162,'s');
	verify(163,'t');
	verify(164,'u');
	verify(165,'v');
	verify(166,'w');
	verify(167,'x');
	verify(168,'y');
	verify(169,'z');
	verify(173,'[');
	verify(189,']');
	verify(192,'{');
	verify(193,'A');
	verify(194,'B');
	verify(195,'C');
	verify(196,'D');
	verify(197,'E');
	verify(198,'F');
	verify(199,'G');
	verify(200,'H');
	verify(201,'I');
	verify(208,'}');
	verify(209,'J');
	verify(210,'K');
	verify(211,'L');
	verify(212,'M');
	verify(213,'N');
	verify(214,'O');
	verify(215,'P');
	verify(216,'Q');
	verify(217,'R');
	verify(224,'\\');
	verify(226,'S');
	verify(227,'T');
	verify(228,'U');
	verify(229,'V');
	verify(230,'W');
	verify(231,'X');
	verify(232,'Y');
	verify(233,'Z');
	verify(240,'0');
	verify(241,'1');
	verify(242,'2');
	verify(243,'3');
	verify(244,'4');
	verify(245,'5');
	verify(246,'6');
	verify(247,'7');
	verify(248,'8');
	verify(249,'9');
	verify(64 ,32);
	verify(74,'|');
	verify(75,'.');
	verify(76,'<');
	verify(77,'(');
	verify(78,'+');
	verify(79,'|');
	verify(80,'&');
	verify(90,'!');
	verify(91,'$');
	verify(92,'*');
	verify(93,')');
	verify(94,';');
	verify(95,'^');
	verify(96,'-');
	verify(97,'/');

	printf("verification complete\n") ; 
	return(0) ; 
}
	
static int verify(i,c)
int i ; 
unsigned char c ; 
{
	int t ;

	t = seb2asc(i) ; 
	if (t != ((int) c)) { 
		printf("%d translates to %c, not %c %d\n",i,(char) t, 
			c,t)  ;
	}
	return(0) ; 
}
#endif

int seb2asc(e)
int e ; 
{
	int i ; 

	if (e<64) return(e) ; 
	i = (int) ascch[e-64] ; 
	return(i!=0 ? i : e) ; 
}
