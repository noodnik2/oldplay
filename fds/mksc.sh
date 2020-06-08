
#
#	creates the scaled versions of the master images
#
#	syntax:
#
#	./mksc.sh {[-s]|[-m]|[-l]} listname
#
#	options:
#
#		-t	creates tiny (thumbnail) images
#		-s	creates small (thumbnail) images
#		-m	creates medium images
#		-l	creates large images
#

srcdir="masterimages"
destdir="images"
mktny=""
mksml=""
mkmed=""
mklrg=""


#
#
#

while getopts "tsml" opt $*; do
	case $opt in
		t)	mktny="yes" ;;
		s)	mksml="yes" ;;
		m)	mkmed="yes" ;;
		l)	mklrg="yes" ;;
		*)	echo "unrecognized option: $OPTARG" 1>&2
			exit 2
			;;
	esac	
done
shift `expr $OPTIND - 1`

listname="$1"
listfile="${listname}.list"


#
#
#

if [ -z "$listname" ]; then
	echo "please specify listname" 1>&2
	exit 1
fi

if [ ! -e "$listfile" ]; then
	echo "listfile not found" 1>&2
	exit
fi

if [ -z "$mksml" -a -z "$mkmed" -a -z "$mklrg" ]; then
	echo "no images selected: use -s -m and/or -l to select" 1>&2
	exit 1
fi


#
#
#

doresize()
{
	image="$1"
	suffix="$2"
	xsize="$3"
	ysize="$4"

	destfile="$destdir/${image}${suffix}.jpg"
	echo "Creating: \"$destfile\""
	jpegtopnm "$srcfile" | pnmscale -xysize "$xsize" "$ysize" | ppmtojpeg > "$destfile"
}

dolist()
{
	while read x; do
		srcfile="$srcdir/${x}.jpg"
		if [ ! -e "$srcfile" ]; then
			echo "file \"$srcfile\" not found." 1>&2
			continue
		fi
		[ -n "$mktny" ] && doresize "$x" "-t" 32 32
		[ -n "$mksml" ] && doresize "$x" "-s" 128 128
		[ -n "$mkmed" ] && doresize "$x" "-m" 560 560
		[ -n "$mklrg" ] && doresize "$x" "-l" 1200 1200
	done
}

dolist < "$listfile"
RC="$?"

exit $RC

# end of script
