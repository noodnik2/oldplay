
#
#
#
#

fromdate="$1"
todate="$2"

if [ "`expr length \"$fromdate\"`" != "12" -o "`expr length \"$todate\"`" != "12" ]; then
	echo "please specify fromdate and todate 12-digits each" 1>&2
	exit 1
fi

if [ $fromdate -gt $todate ]; then
	echo "fromdate is greater than todate" 1>&2
	exit 1
fi

while read x; do
	[ $x -lt $fromdate ] && continue
	[ $x -gt $todate ] && continue
	echo "$x"
done

# end of script
