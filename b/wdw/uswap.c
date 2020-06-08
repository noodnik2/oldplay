void uswap(up1,up2)
unsigned *up1, *up2;
{
unsigned ut;
ut = *up1;
*up1 = *up2; 
*up2 = ut;
}
