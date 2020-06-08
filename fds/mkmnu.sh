
#
#	creates the "menu" web-page (the left panel)
#
#	syntax:
#
#		./mkmnu.sh [-a] listname
#
#	options:
#
#		-a	Put menu out in "adminstrative" mode
#

imgdir="images"
mode="normal"

#
#
#

while getopts "a" opt $*; do
	case "$opt" in
		a)	mode="adm" ;;
		*)	echo "unrecognized option: $OPTARG" 1>&2
			exit 2
			;;
	esac	
done
shift `expr $OPTIND - 1`

listname="$1"
listfile="${listname}.list"
if [ "$mode" = "adm" ]; then
	listpage="${listname}-adm.html"
else
	listpage="${listname}.html"
fi
intropage="${listname}-intro.html"


#
#
#

if [ ! -e "$listfile" ]; then
	echo "list file not found" 1>&2
	exit 1
fi

if [ -e "$listpage" ]; then
	echo "page already exists; erase first" 1>&2
	exit 1
fi


#
#
#

mkpage()
{
	echo "<html>"
	echo "<head>"
	echo "<base target=\"main\">"
	echo "</head>"
	echo "<body>"
	echo "<center>"
	
	if [ -e "$intropage" ]; then
		echo "<p>"
		echo "<a href=\"$intropage\">"
		echo "<font size="+2"><b>Intro</b></font>"
		echo "</a>"
		echo "<p>"
	fi
	
	while read x; do
	
		altfile="${x}.alt"
		thumbfile="$imgdir/${x}-s.jpg"
		if [ "$mode" = "adm" ]; then
			linkfile="mkupd.cgi?${x}"
		else
			linkfile="${x}.html"
		fi
		alttext="${x}"
		if [ -s "$altfile" ]; then
			alttext="`cat \"$altfile\"`"
		fi
	
		if [ ! -e "$thumbfile" ]; then
			echo "warning: \"$thumbfile\" not found." 1>&2
			continue
		fi
	
		echo "<a href=\"$linkfile\">"
		echo "<img src=\"$thumbfile\" alt=\"$alttext\">"
		echo "</a>"
	
		echo "<p>"
	
	done
	
	echo "</center>"
	echo "</body>"
	echo "</html>"

}


#
#
#

mkpage < "$listfile" > "$listpage"
RC="$?"

if [ "$RC" != "0" ]; then
	echo "R($RC);" 1>&2
	exit $RC
fi

echo "Created: \"$listpage\""

# end of file
