#!/bin/bash
#arguments input test
TARGET=firewall
$TARGET start
$TARGET append  --sport 22  --target DROP --chain INCOME
$TARGET list-rules
#./$TARGET policy --chain INCOME --target ACCEPT
#./$TARGET list-rules


