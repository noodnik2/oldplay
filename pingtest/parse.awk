#
#
#

BEGIN {
	open= 0
}

function printout() {
	printf("%s %s %s %s %s %s\n", ipaddr, time, loss, statmin, statavg, statmax) 
}

index($0, "IST")> 0	{
	if (open) {
		printout()
		open= 0
	}
	loss="."
	statmin= "."
	statavg= "."
	statmax= "."
	ipaddr=$1
	gsub(":", "", ipaddr)
	time=$5
	gsub(":", "", time)
	open= 1
}

index($0, "packet loss")> 0 {
	n= NF - 2
	loss=$n
	gsub("%", "", loss)
}

index($0, "avg")> 0 {
	split($4, stats, "/")
	statmin= stats[1]
	statavg= stats[2]
	statmax= stats[3]
}

END {
	if (open) {
		printout()
		open= 0
	}
}

