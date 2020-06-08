#!/usr/bin/wish
# pingtool - a simple user interface to ping
# Tkexample chapter

# Set title and allow window resizing.
wm title . "Pingtool v1"
wm minsize . 20 20

# Create a frame for buttons.
frame .buttons -borderwidth 10
pack .buttons -side top -fill x

# Create the command buttons.
button .buttons.quit -text Quit -command exit
button .buttons.ping -text Ping -command Ping
pack .buttons.quit .buttons.ping -side right

# Create a labeled entry for hostname type in.
frame .f ; pack .f -side top
label .f.l -text Host:
entry .f.host -width 20 -relief sunken
pack .f.l .f.host -side left

# A Return triggers the Ping command
bind .f.host <Return> Ping

# Create a text widget to log the output
text .log -width 40 -height 10 -bd 2 -relief raised
pack .log -side top

# Run ping and log the results
proc Ping {} {
    global hostEntry log
    set hostname [.f.host get]
    catch {exec ping -v -c 1 $hostname} result
    .log insert end $result
    .log insert end \n
}
