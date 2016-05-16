#-----------------------------------------------------
#  dmlc-core: the configuration compile script
#
#  This is the default configuration setup for
#  If you want to change configuration, do the following steps:
#
#  - copy this file to the root of dmlc-core folder
#  - modify the configuration you want
#  - type make or make -j n on each of the folder
#----------------------------------------------------

# choice of compiler
export CC = /data1/home/wangchao/gcc-4.8.2/bin/gcc-4.8
export CXX = /data1/home/wangchao/gcc-4.8.2/bin/g++-4.8
export MPICXX = mpicxx

# whether to compile with -fPIC option
# Note: to build shared library(so files), fPIC is required
WITH_FPIC = 1

# whether use HDFS support during compile
USE_HDFS = 1

# whether use AWS S3 support during compile
USE_S3 = 0

# whether use Azure blob support during compile
USE_AZURE = 0

# path to libjvm.so
LIBJVM=$(JAVA_HOME)/jre/lib/amd64/server

# whether building unittest (gtest is required)
BUILD_TEST=0

# path to gtest library (only used when $BUILD_TEST=1)
# there should be an include path in $GTEST_PATH/include and library in $GTEST_PATH/lib
GTEST_PATH=

# path to third-party dependences such as glog
DEPS_PATH=
