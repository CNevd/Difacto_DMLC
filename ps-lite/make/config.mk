# default configuration of make
#
# you can copy it to the parent directory and modify it as you want. then
# compile by `make -j 8` using 8 threads

# compiler
CXX = g++

# whether use google logging
USE_GLOG = 1

# whether use HDFS support during compile
USE_HDFS = 0

# optimization flag. -O0 -ggdb for debug
# OPT = -O3 -ggdb

# the installed path of third party libraries
# DEPS_PATH = $(shell pwd)/deps

# additional link flags, such as -ltcmalloc_and_profiler
# EXTRA_LDFLAGS =

# additional compile flags
# EXTRA_CFLAGS =
