#! /bin/bash -u

cd $(git rev-parse --show-toplevel)/build
exit_code=0

echo "=========== ls test =========="
ROS3FS_TMPFILE=$(mktemp)
NORMALFS_TMPFILE=$(mktemp)
ls ros3fs_mountpoint >& ${ROS3FS_TMPFILE}
ls ros3fs_mountpoint_answer >& ${NORMALFS_TMPFILE}

if [[ $(diff ${ROS3FS_TMPFILE} ${NORMALFS_TMPFILE}) ]]; then
    diff ${ROS3FS_TMPFILE} ${NORMALFS_TMPFILE}
    echo "test failed"
    exit_code=1
fi

echo "=========== ls -F test =========="
ROS3FS_TMPFILE=$(mktemp)
NORMALFS_TMPFILE=$(mktemp)
ls -F ros3fs_mountpoint >& ${ROS3FS_TMPFILE}
ls -F ros3fs_mountpoint_answer >& ${NORMALFS_TMPFILE}

if [[ $(diff ${ROS3FS_TMPFILE} ${NORMALFS_TMPFILE}) ]]; then
    diff ${ROS3FS_TMPFILE} ${NORMALFS_TMPFILE}
    echo "test failed"
    exit_code=1
fi

echo "=========== cat test =========="
ROS3FS_TMPFILE=$(mktemp)
NORMALFS_TMPFILE=$(mktemp)
cat ros3fs_mountpoint/testfile_a >& ${ROS3FS_TMPFILE}
cat ros3fs_mountpoint_answer/testfile_a >& ${NORMALFS_TMPFILE}

if [[ $(diff ${ROS3FS_TMPFILE} ${NORMALFS_TMPFILE}) ]]; then
    diff ${ROS3FS_TMPFILE} ${NORMALFS_TMPFILE}
    echo "test failed"
    exit_code=1
fi

echo "=========== grep -r test =========="
ROS3FS_TMPFILE=$(mktemp)
NORMALFS_TMPFILE=$(mktemp)
pushd ros3fs_mountpoint
grep -r aa . | sort >& ${ROS3FS_TMPFILE}
popd
pushd ros3fs_mountpoint_answer
grep -r aa . |sort >& ${NORMALFS_TMPFILE}
popd

if [[ $(diff ${ROS3FS_TMPFILE} ${NORMALFS_TMPFILE}) ]]; then
    diff ${ROS3FS_TMPFILE} ${NORMALFS_TMPFILE}
    echo "test failed"
    exit_code=1
fi

exit ${exit_code}
