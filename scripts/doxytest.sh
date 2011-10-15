#!/bin/sh

$1 $2 2>&1 | grep warning ; exit $(( $? == 0 ))
