# common make for parameter server applications

ifndef DEPS_PATH
DEPS_PATH = ../../ps-lite/deps
endif

PS_PATH=../../ps-lite
CORE_PATH=../../dmlc-core

include $(PS_PATH)/make/config.mk
include $(PS_PATH)/make/ps.mk
include $(CORE_PATH)/make/dmlc.mk

INCLUDE=-I./ -I../ -I$(PS_PATH)/src -I$(CORE_PATH)/include -I$(CORE_PATH)/src -I$(DEPS_PATH)/include

CFLAGS  += -O3 -ggdb -Wall -std=c++11 $(INCLUDE) $(DMLC_CFLAGS) $(PS_CFLAGS) $(EXTRA_CFLAGS)
LDFLAGS += $(DMLC_LDFLAGS) $(PS_LDFLAGS) $(EXTRA_LDFLAGS)

.DEFAULT_GOAL := all

$(CORE_PATH)/libdmlc.a:
	$(MAKE) -C ../.. dmlc-core

$(PS_PATH)/build/libps.a:
	$(MAKE) -C ../.. ps-lite

DMLC_SLIB = $(CORE_PATH)/libdmlc.a $(PS_PATH)/build/libps.a

build:
	@mkdir -p build

build/%.o: %.cc | build
	$(CXX) $(CFLAGS) -MM -MT build/$*.o $< >build/$*.d
	$(CXX) $(CFLAGS) -c $< -o $@

%.pb.cc %.pb.h : %.proto
	${DEPS_PATH}/bin/protoc --cpp_out=. --proto_path=. $<

-include build/*.d
