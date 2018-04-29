#!/bin/bash

CPP_LINES_ALL=$(( find ../../fhq-server/src -name '*.cpp' -print0 | xargs -0 cat ) | wc -l)
CPP_LINES_SMTP=$(( find ../../fhq-server/src/3rdParty -name '*.cpp' -print0 | xargs -0 cat ) | wc -l)

echo "Lines in 3rdParty: $CPP_LINES_SMTP"
echo "All lines: $CPP_LINES_ALL"


CPP_LINES_TMP=0
if [ -d "../../fhq-server/src/tmp" ]; then
	CPP_LINES_TMP=$(( find ../../fhq-server/src/tmp -name '*.cpp' -print0 | xargs -0 cat ) | wc -l)
fi


CPP_LINES=$(expr $CPP_LINES_ALL - $CPP_LINES_SMTP - $CPP_LINES_TMP)
echo "Ours lines in cpp files: $CPP_LINES"

# TODO another statistics





