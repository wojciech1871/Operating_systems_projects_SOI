#!/bin/sh
echo C1 with A1 and A2
./testA2 &
./testC1 &
./testA1 &
