#! /bin/bash -u

cd $(git rev-parse --show-toplevel)/build

echo "=========== ls test =========="
OZONEFS_TMPFILE=$(mktemp)
NORMALFS_TMPFILE=$(mktemp)
ls -F ozonefs_mountpoint >& ${OZONEFS_TMPFILE}
ls -F ozonefs_mountpoint_answer >& ${NORMALFS_TMPFILE}

if [[ $(diff ${OZONEFS_TMPFILE} ${NORMALFS_TMPFILE}) ]]; then
    diff ${OZONEFS_TMPFILE} ${NORMALFS_TMPFILE}
    echo "ls test failed"
    exit 1
fi
