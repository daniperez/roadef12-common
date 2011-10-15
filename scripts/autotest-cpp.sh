#!/bin/bash

include=""
exclude=""

while getopts I:E: opt
do  case "$opt" in
  I)  include="$OPTARG";;
  E)  exclude="$OPTARG";;
  [?])  echo "Usage: $0 -I <folder to watch> [-E <folder to exclude>]"
    exit 1;;
  esac
done

if [ -z "${include}" ]; then
  echo "Usage: $0 -I <folder to watch> [-E <folder to exclude>]" 
  exit 1
fi

echo "include: [${include}]"
echo "exclude: [${exclude}]"

inotifywait -mr --exclude "(^\./\.git/.*)" --exclude "(^\./\.git/.*)" --exclude "(^\./\.hg/.*)" --exclude "(\.swp$)" --exclude "${exclude}.*" --format "%w%f %e" -e modify -e create -e attrib ${include}

