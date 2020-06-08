#! /bin/sh

progpath="$0"
progname=`basename $progpath`
nowdate=`date +"%y%m%d"`

errmsg()
{
	echo "${progname}: $*" 1>&2
}

process()
{
	_fname="$1"
	set - `wc ${_fname}`
	_fsize="$3"
	echo "[${_fname}]"
	echo "acquiry=${nowdate}"
	echo "filesize=${_fsize}"
	echo ""
}

for x in $1; do
	if [ -d $x ]; then
		$progpath $x
	else
		case "$x" in
		*.mid|*.MID)
			process $x
			;;
		*)	errmsg "NOTE: $x not processed"
			;;
		esac
	fi
done

exit 0

# end
