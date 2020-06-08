#! /bin/sh

loguser()
{
	echo -n "<tr>"
	echo -n "<td>${REMOTE_HOST}</td>"
	echo -n "<td>${REMOTE_ADDR}</td>"
	echo -n "<td>${HTTP_USER_AGENT}</td>"
	echo -n "<td>`date "+%a %x %k:%M"`</td>"
	echo -n "</tr>"
	echo ""
}

logfile="${WN_ROOT}/logs/user.log"
loguser >> ${logfile} 2>&1
