#!/bin/bash

set -x

g++ -Wall -g uvwget.cpp  -luv -lcurl -o  uvwget


g++ -Wall curl_test.cpp -g -luv -lcurl -O2 -o curl_test

./curl_test www.baidu.com www.google.com