#!/bin/bash
#
# This script creates all the configs found under configs/ folder
# having ".conf" file extension.
#
# Author: daniperez 
#

_BUILD_DIR=build
_PWD=$PWD

mkdir -p $_BUILD_DIR 

#######################################
function createConfigFolder
#######################################
# Parameters
# ----------
#
# $1: config file (path relative to
#     project's root folder).
#
#######################################
{
    config_file=$1
    config_file_name=$(basename $config_file)
    config_name=${config_file_name%.conf}
    config_build_folder=$_BUILD_DIR/$config_name

    echo "Creating $config_build_folder..."

    mkdir -p $config_build_folder > /dev/null
    cd $config_build_folder > /dev/null
 
    echo "=============== CMake output ==============="
    cmake -C $_PWD/$config_file $_PWD

    if [ "$?" -ne "0" ]; then
        echo "And error ocurred. Exiting..."
        exit 1
    fi

    echo "============= End CMake output ============="

    echo "...$config_build_folder created and configured"

    cd $_PWD 
}

#######################################
# Main
#######################################
for config_file in $(ls configs/*.conf)
do

    createConfigFolder $config_file

done



