#!/bin/bash

cd cat/
RESULT=$(bash test_cat.sh | grep "FAIL: 0")
if [ -z "$RESULT" ]; then
    echo 'Integration test failed';
    exit 1;
fi

cd ../grep/
RESULT=$(bash test_grep.sh | grep "FAIL: 0")
if [ -z "$RESULT" ]; then
    echo 'Integration test failed';
    exit 1;
fi
echo 'Integration test successful!';
exit 0;