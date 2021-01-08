#!/bin/bash

function calculateLinesInFiles() {
	LINES=0
	LINES_CPP=$((find $1 -name "*.cpp" -print0 | xargs -0 cat ) | wc -l)
	LINES=$(( $LINES + $LINES_CPP))
	LINES_H=$((find $1 -name "*.h" -print0 | xargs -0 cat ) | wc -l)
	LINES=$(( $LINES + $LINES_H))
	LINES_HPP=$((find $1 -name "*.hpp" -print0 | xargs -0 cat ) | wc -l)
	LINES=$(( $LINES + $LINES_HPP))
	LINES_C=$((find $1 -name "*.c" -print0 | xargs -0 cat ) | wc -l)
	LINES=$(( $LINES + $LINES_C))
	LINES_CC=$((find $1 -name "*.cc" -print0 | xargs -0 cat ) | wc -l)
	LINES=$(( $LINES + $LINES_CC))
	echo $LINES

}

LINES_SRC=$(calculateLinesInFiles "../../fhq-server/src")
LINES_SRC_3RDPARTY=$(calculateLinesInFiles "../../fhq-server/src/3rdParty")
LINES_WSJCPP=$(calculateLinesInFiles "../../fhq-server/src.wsjcpp")

echo "Lines in src: $LINES_SRC"
echo "Lines in src/3rdParty: $LINES_SRC_3RDPARTY"
echo "Lines in src.wsjcpp: $LINES_WSJCPP"

LINES_FHQ_SERVER=$(($LINES_SRC - $LINES_SRC_3RDPARTY))
echo "Lines in development files for fhq-server: $LINES_FHQ_SERVER"
