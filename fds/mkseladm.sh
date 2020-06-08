
#
#	creates the "select admin" page
#
#	syntax:
#
#		./mkseladm.sh listname
#
#

imgdir="images"
listname="$1"
listfile="${listname}.list"
listseladmpage="${listname}-seladm.html"


#
#
#

if [ -z "$listname" ]; then
	echo "please specify listname" 1>&2
	exit 1
fi

if [ ! -e "$listfile" ]; then
	echo "listfile not found" 1>&2
	exit 1
fi


#
#
#

mklist()
{
	echo "<html>"
	echo "<head>"
	echo "<title>List Creation</title>"
	echo "</head>"
	echo "<body>"

	echo "<form method=\"post\" action=\"mklist.cgi\">"
	echo "<table>"

	echo "<tr><td colspan=\"3\">"
	echo "<b>Enter List Name:</b><br>"
	echo "<input align=left type=\"text\" name=\"list\">"
	echo "<input align=center type=\"submit\" value=\"Save\">"
	echo "<input align=right type=\"reset\" value=\"Clear\">"
	echo "</td></tr>"

	while read x; do

		thumbfile="$imgdir/${x}-s.jpg"
		linkfile="$imgdir/${x}-m.jpg"

		if [ ! -e "$thumbfile" ]; then
			echo "warning: \"$thumbfile\" not found." 1>&2
		fi

		echo "<tr>"

		echo "<td align=\"center\" valign=\"middle\">"
		echo "<input type=\"checkbox\" name=\"add\" value=\"${x}\">"
		echo "</td>"

		echo "<td align=\"center\" valign=\"middle\">"
		echo "<a href=\"$linkfile\">"
		echo "<img src=\"$thumbfile\">"
		echo "</a>"
		echo "</td>"

		echo "<td align=\"center\" valign=\"middle\">"
		echo "<font size=\"+1\"><i>${x}</i></font><br>"
		echo "</td>"

		echo "</tr>"

	done

	echo "</table>"
	echo "</form>"

	echo "</body>"
	echo "</html>"

}


#
#
#

echo "Creating: \"$listseladmpage\""
mklist < "$listfile" > "$listseladmpage"
RC="$?"

exit $RC

# end of script
