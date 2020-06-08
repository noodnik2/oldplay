#! /bin/sh

query="$QUERY_STRING"

cmd="`expr substr \"$query\" 1 1`"
arg="`expr substr \"$query\" 2 99`"


#

ok()
{
	echo "Location: $HTTP_REFERER"
	echo ""
}

bad()
{
	echo ""
	echo "ERROR: Invalid argument(s) passed to cgi script."
	echo "-"
	echo "SCRIPT_NAME=$SCRIPT_NAME"
	echo "HTTP_REFERER=$HTTP_REFERER"
	echo "-"
	echo "Press the <BACK> button on your browser when done."
	echo ""
}


#

case "$cmd" in

d)	xterm -display ":0.0" -T "$arg description" -e vi ${arg}.desc
	ok
	;;

a)	xterm -display ":0.0" -T "$arg alternate text" -e vi ${arg}.alt
	ok
	;;

*)	bad
	;;

esac

# end of script

