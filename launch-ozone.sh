#! /bin/bash

# Check https://issues.apache.org/jira/browse/HDDS-4677 for ports details.
sudo docker run --rm --name ozone-instance -p 9860:9860 -p 9862:9862 -p 9872:9872 -p 9874:9874 -p 9878:9878 -p 9876:9876 apache/ozone:1.3.0
