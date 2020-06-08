
masterdir="masterimages"

(cd $masterdir > /dev/null 2>&1; ls -1 *.jpg) | sed "1,$ s/\\.jpg//g"

# end of script
