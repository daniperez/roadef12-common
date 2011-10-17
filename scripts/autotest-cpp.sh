#!/bin/bash
# The MIT License
#
# Copyright (c) 2011 daniperez
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

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

