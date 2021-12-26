#!/bin/bash

INK=/usr/bin/inkscape

if [ ! -f "$INK" ]; then
	echo "Not found $INK"
	exit -1;
fi

function makePng512x512 {
	SVG=$1
	BASE=`basename $1 .svg`
	echo "Will be $1 converted to "$BASE".png"
	OUT="dist/"$BASE".png"
	$INK -z -D --export-area-page -e $OUT  -f $SVG -w 512 -h 512
}

FILES=./images/*
for f in $FILES
do
  	echo "Processing $f ..."
	makePng512x512 "$f"
done