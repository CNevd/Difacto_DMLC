include ../../ps-lite/make/ps_app.mk

all: build/linear.dmlc build/dump.dmlc

clean:
	rm -rf build *.pb.*

build/linear.dmlc: build/config.pb.o build/linear.o $(DMLC_SLIB)
	$(CXX) $(CFLAGS) $(filter %.o %.a, $^) $(LDFLAGS) -o $@

build/dump.dmlc: build/dump.o $(DMLC_SLIB)
	$(CXX) $(CFLAGS) $(filter %.o %.a, $^) $(LDFLAGS) -o $@
