include ../../ps-lite/make/ps_app.mk

all: build/difacto.dmlc build/dump.dmlc

clean:
	rm -rf build *.pb.*

build/difacto.dmlc: build/config.pb.o build/difacto.o $(DMLC_SLIB)
	$(CXX) $(CFLAGS) $(filter %.o %.a, $^) $(LDFLAGS) -o $@

build/dump.dmlc: build/dump.o $(DMLC_SLIB)
	$(CXX) $(CFLAGS) $(filter %.o %.a, $^) $(LDFLAGS) -o $@
