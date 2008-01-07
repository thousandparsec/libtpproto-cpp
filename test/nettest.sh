#!/bin/bash
if [ "$TP_TEST_HOST" = "" ]; then
	export TP_TEST_HOST=localhost
fi
./nettest $TP_TEST_HOST
