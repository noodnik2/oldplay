

destdir="masterimages"

while read x; do
	srcfile="$x"
	read x
	destfile="$destdir/$x.jpg"
	if [ -f "$destfile" ]; then
		if cmp "$srcfile" "$destfile"; then
			echo "$destfile already exists; they are the same" 1>&2
		else
			echo "$destfile already exists; NOT the same!" 1>&2
		fi
		continue
	fi
	echo cp -i "$srcfile" "$destfile"
	cp -i "$srcfile" "$destfile"
done

