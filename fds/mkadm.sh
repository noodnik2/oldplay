#! /bin/sh

#
#	mkadm.sh
#
#

# process the form
if ../mkupd; then
	# update the page upon completion of "POST"
	echo "|$REQUEST_METHOD|" > /tmp/xa
	case "$REQUEST_METHOD" in
		POST)	echo "$QUERY_STRING" | ../mkpgs.sh ;;
	esac
else
	echo "mkupd failed: $?" > /tmp/xa
fi

# end of script
