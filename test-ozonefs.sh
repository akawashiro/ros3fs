#! /bin/bash -u

cd $(git rev-parse --show-toplevel)/build
exit_code=0

echo "=========== ls test =========="
OZONEFS_TMPFILE=$(mktemp)
NORMALFS_TMPFILE=$(mktemp)
ls ozonefs_mountpoint >& ${OZONEFS_TMPFILE}
ls ozonefs_mountpoint_answer >& ${NORMALFS_TMPFILE}

if [[ $(diff ${OZONEFS_TMPFILE} ${NORMALFS_TMPFILE}) ]]; then
    diff ${OZONEFS_TMPFILE} ${NORMALFS_TMPFILE}
    echo "test failed"
    exit_code=1
fi

echo "=========== ls -F test =========="
OZONEFS_TMPFILE=$(mktemp)
NORMALFS_TMPFILE=$(mktemp)
ls -F ozonefs_mountpoint >& ${OZONEFS_TMPFILE}
ls -F ozonefs_mountpoint_answer >& ${NORMALFS_TMPFILE}

if [[ $(diff ${OZONEFS_TMPFILE} ${NORMALFS_TMPFILE}) ]]; then
    diff ${OZONEFS_TMPFILE} ${NORMALFS_TMPFILE}
    echo "test failed"
    exit_code=1
fi

echo "=========== cat test =========="
OZONEFS_TMPFILE=$(mktemp)
NORMALFS_TMPFILE=$(mktemp)
cat ozonefs_mountpoint/testfile_a >& ${OZONEFS_TMPFILE}
cat ozonefs_mountpoint_answer/testfile_a >& ${NORMALFS_TMPFILE}

if [[ $(diff ${OZONEFS_TMPFILE} ${NORMALFS_TMPFILE}) ]]; then
    diff ${OZONEFS_TMPFILE} ${NORMALFS_TMPFILE}
    echo "test failed"
    exit_code=1
fi

echo "=========== grep -r test =========="
OZONEFS_TMPFILE=$(mktemp)
NORMALFS_TMPFILE=$(mktemp)
grep -r bb ozonefs_mountpoint >& ${OZONEFS_TMPFILE}
grep -r bb ozonefs_mountpoint_answer >& ${NORMALFS_TMPFILE}

if [[ $(diff ${OZONEFS_TMPFILE} ${NORMALFS_TMPFILE}) ]]; then
    diff ${OZONEFS_TMPFILE} ${NORMALFS_TMPFILE}
    echo "test failed"
    exit_code=1
fi

exit ${exit_code}
