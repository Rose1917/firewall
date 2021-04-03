#!/bin/bash
#arguments input test
TARGET=firewall
$TARGET start
$TARGET append --chain INCOME --service SSH --target DROP
$TARGET list


#./$TARGET policy --chain INCOME --target ACCEPT
#./$TARGET list-rules


