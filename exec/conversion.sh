#!/bin/bash

FILEIN="$1"
ISMC=$2
LOGFILE="$3"

# This directory
DIR_THIS="$(dirname "$(realpath "$0")")"
echo $DIR_THIS
# Run the macro.
alienv setenv AliPhysics/latest -c root -b -q -l "$DIR_THIS/convertAO2D.C(\"$FILEIN\", $ISMC)" > "$LOGFILE" 2>&1
ExitCode=$?

# Show warnings, errors and fatals in the log file.
grep -e '^'"W-" -e '^'"Warning" -e '^'"E-" -e '^'"Error" -e '^'"F-" -e '^'"Fatal" "$LOGFILE" | sort -u

exit $ExitCode
