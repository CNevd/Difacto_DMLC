#!/bin/bash

../../dmlc-core/tracker/dmlc_local.py -n 1 -s 1 build/linear.dmlc guide/demo.conf
build/dump.dmlc model_in="agaricus_model_part-0" dump_out="dump.txt"
