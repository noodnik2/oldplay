
#
#	This script "brands" comments into the specified graphic
#	files with the contents of the ".desc" (description) file.
#
#	syntax:
#
#		./mkcmt < list
#

while read x; do

	descfile="${x}.desc"
	if [ ! -e "$descfile" ]; then
		continue
	fi

	for y in ${x}*.jpg; do
		jpgfile="$y"
		jpgtemp="${y}.tmp"
		if ! mv "$jpgfile" "$jpgtemp"; then
			continue
		fi
		if ! wrjpgcom -replace -cfile "$descfile" "$jpgtemp" > "$jpgfile"; then
			continue
		fi
		if [ -s "$jpgfile" ]; then
			rm "$jpgtemp"
		else
			echo "warning: output size zero; \"$jpgtemp\" file not deleted" 1>&2
		fi
	done

done


