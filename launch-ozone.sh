#! /bin/bash

docker run --rm --name ozone-instance -p 9878:9878 -p 9876:9876 apache/ozone:1.3.0 
