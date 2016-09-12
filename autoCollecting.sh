#!/bin/sh

# check if directory has existed and make it
dateNow=$(date +%Y%m%d)
if [ -d "$dateNow" ]; then
	echo The directory has existed.
	exit 0
else mkdir $dateNow
fi

# revise
./revise resource/20160817/photo_0.png
# open photo

# collecting data and cut

