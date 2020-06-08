#! /bin/sh

#
#
#	syntax:
#
#		./mkpgs.sh {[listname] | < listfile}
#
#	description:
#
#		updates all of the pages specified by "listname" (including
#		the "listname-intro" page) or, if "listname" is not specified,
#		updates only the pages specified in "stdin" (note: the index
#		page is not automatically updated when stdin is used).
#

imgdir="images"

listname="$1"
if [ -n "$listname" ]; then
	listfile="${listname}.list"
	introfile="${listname}.desc"
	intropage="${listname}-intro.html"
else
	listfile=""
	introfile=""
	intropage=""
fi


#
#
#

if [ -n "$listfile" ]; then
	if [ ! -e "$listfile" ]; then
		echo "list not found or specified" 1>&2
		exit 1
	fi
fi


#
#
#

makepage()
{
	x="$1"

	altfile="${x}.alt"
	descfile="${x}.desc"
	imgfile="$imgdir/${x}-m.jpg"
	imgfile_large="$imgdir/${x}-l.jpg"

	alttext="${x}"
	if [ -s "$altfile" ]; then
		alttext="`cat \"$altfile\"`"
	fi

	desctext=""
	if [ -s "$descfile" ]; then
		desctext="`cat \"$descfile\"`"
	fi

	if [ ! -e "$imgfile" ]; then
		echo "warning: \"$imgfile\" not found." 1>&2
		return
	fi

	echo "<html>"
	echo "<head>"
	echo "<base target=\"main\">"
	echo "</head>"
	echo "<body>"
	echo "<center>"

	[ -e "$imgfile_large" ] && echo "<a href=\"$imgfile_large\">"
	echo "<img src=\"$imgfile\" alt=\"$alttext\" title=\"$desctext\">"
	[ -e "$imgfile_large" ] && echo "</a>"

	if [ -n "$desctext" ]; then
		echo "<p><font size="+1" face="Arial"><i>"
		echo "$desctext"
		echo "</i></font></p>"
	fi
	
	echo "</center>"
	echo "</body>"
	echo "</html>"

}

mkintro()
{
	echo "<html>"
	echo "<head>"
	echo "</head>"
	echo "<body>"
	echo "<font size=\"+1\" font=\"Arial\">"
	cat "$introfile"
	echo "</font>"
	echo "</body>"
	echo "</html>"
}

mkpages()
{
	while read x; do
		page="${x}.html"
		echo "Creating: \"$page\""
		makepage "$x" > "$page"
	done 
}


#
#
#

if [ -n "$introfile" ]; then
	if [ -e "$introfile" ]; then
		echo "Creating: \"$intropage\""
		mkintro > "$intropage"
	fi
fi

if [ -n "$listfile" ]; then
	mkpages < "$listfile"
else
	mkpages
fi

#
# end of script
