#! /bin/ksh

cleanandleave()
{
	rm -f ${tmpfile}
	exit 0
}

imhere()
{
	echo "
		<a href=\"telnet://advant.pos.co.il:5004\"><img
			src="images/open.gif"
			border=0
			align=left
		></a>
		<font size=+3>I</font>t appears that you've caught me while
		I'm ${1}!  If you'd like to chat with me, and you have 
		<a href=\"http://www.northernc.on.ca/wasted/cws/sterm.html\"
		>telnet</a> capability in your WEB browser, then click on the
		<a href=\"telnet://advant.pos.co.il:5004\">
		open door.
		</a>
	"
	cleanandleave
}

nothere()
{
	set - `date -u +"%H %w %M"`
	thr="$1"
	tdy="$2"
	tmn="$3"
	if [ $tdy -lt 6 ]; then
		if [ $thr -ge 10 -a $thr -lt 19 ]; then
			doing="working"
		else
			if [ $thr -ge 19 ]; then
				doing="out dancing"
			else 
				doing="sleeping"
			fi
		fi
	else
		doing="away for the weekend"
	fi
	echo "
		<img src="images/closed.gif" border=0 align=left>
		<font size=+3>I</font>'d like to visit with you in person,
		but right now I'm doing something else.
"
	if [ "$doing" != "working" ]; then
		echo "
			<i>
			The time is now ${thr}:${tmn} GMT.  I'm probably
			${doing} right now.  If you come back here during
			the hours I'm at my PC at work (which is generally
			each Sunday through Thursday, from 10:00 to 19:00
			GMT), then you can chat with me online.
			</i>
	"
	else
		echo "
			<i>
			According to the hour, I should be at work.  Perhaps
			I've switched off my computer because I got sick of
			staring at it, or maybe I'm in a meeting.  Try back
			in a couple of hours, ok?
			</i>
	"	
	fi
	cleanandleave
}

tmpfile="/tmp/amihome.tmp"

# trap "nothere" 1 2 3 6 10 11 12 13 14 15 17 18 19 20 21 22

if /usr/local/bin/mtcheck marty > ${tmpfile}; then
	imhere "`cat ${tmpfile}`"
else
	nothere
fi

# notreached
