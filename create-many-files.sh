#! /bin/bash -eu
# Check http://localhost:9876/#!/ when you encounter errors.

TMPDIR=$(mktemp -d)
for i in $(seq 1 3000000)
do
    echo ${RANDOM} > ${TMPDIR}/${i}

    if [[ $((${i} % 100000)) == "0" ]]
    then
        echo "Done ${i}"
    fi
done

aws s3 --endpoint http://localhost:9878 cp --storage-class REDUCED_REDUNDANCY --recursive ${TMPDIR} s3://bucket1/many
aws s3 --endpoint http://localhost:9878 ls s3://bucket1/

rm -rf ${TMPDIR}
