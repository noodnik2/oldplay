void cursor();
int getch(), kbhit();

int montype = 1;

void pause(s)
char *s;
{
printf("%s\n",s);
while(kbhit()) getch();
while(!kbhit());
}

void main()
{
pause("About to set cursor.");
cursor(1);
pause("Cursor has been changed.");
cursor(0);
pause("Cursor was restored.");
}
