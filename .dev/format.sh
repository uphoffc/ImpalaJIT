#! /usr/bin/env sh

# NOTE: This script was taken from PointCloudLibrary/pcl and adapted for ImpalaJIT

# sample command line usage: $0 clang-format(version >= 6.0) $IMPALA_SOURCE_DIR
# $ sh ./.dev/format.sh `which clang-format` ./
# $ sh format.sh `which clang-format` ../

format() {
    # don't use a directory with whitespace
    local whitelist="compiler/engine compiler/code-gen"

    local IMPALA_DIR="${2}"
    local formatter="${1}"

    if [ ! -f "${formatter}" ]; then
        echo "Could not find a clang-format. Please specify one as the first argument"
        exit 166
    fi

    # check for self
    if [ ! -f "${IMPALA_DIR}/.dev/format.sh" ]; then
        echo "Please ensure that IMPALA_SOURCE_DIR is passed as the second argument"
        exit 166
    fi

    for dir in ${whitelist}; do
        path=${IMPALA_DIR}/${dir}
        find ${path} -type f -iname *.[ch] -o -iname *.[ch]pp -o -iname *.[ch]xx \
            -iname *.cu | xargs -n1 ${formatter} -i -style=file
    done
}

format $@
