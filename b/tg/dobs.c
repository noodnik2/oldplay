/*
	old routine included here for reference 

      else if (ch=='\
	 if (cp==0) continue;
	 scoputch(ch); 
	 if (cp==mp) { RUBOUT; cp=(--mp); continue; }
	 if (insmode) {
	    memcpy( (buffer+cp)-1, (buffer+cp), (mp-cp) );
	    scopcp(0); draw((buffer+cp)-1,(mp-cp)); cp--; mp--;	
	    RUBOUT;
	    scopcp(1);
	    }

*/

#define		CP		(*cpp)
#define		MP		(*mpp)

extern char insmode ; 

void dobs(s,cpp,mpp)
char *s;
int *cpp, *mpp;
{
int l;
char *p;

if (CP) {
   scoputch('\b'); 
   if ( (l=(MP-CP)) ) {
      if (insmode) {
	 p=s+CP; memcpy(p-1, p, l+1);
         scopcp(0); draw(p-1, l); scoputch(' '); scopcp(1); 
	 MP--; 
         }
      CP--; 	
      return;	
      }
   scoputch(' ');
   scoputch('\b');
   CP=(--MP); 
   }
}
