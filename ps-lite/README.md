<img src="http://parameterserver.org/images/parameterserver.png" alt="Parameter Server">
[![Build Status](https://travis-ci.org/dmlc/ps-lite.svg?branch=master)](https://travis-ci.org/dmlc/ps-lite)

### Note

A fixed version used for difacto, more info pls check https://github.com/dmlc/ps-lite

### Introduction

`ps-lite` is a high-performance implementation of the parameter server
framework. It provides simple yet powerful APIs. For example, a worker node can
communicate with the server nodes via
- `Push(keys, values)`: push a list of (key, value) pairs to the server nodes
- `Pull(keys)`: pull the values from servers for a given key list
- `Wait`: wait a push or pull finished.

A simple example:

```
  std::vector<uint64_t> key = {1, 3, 5};
  std::vector<float> val = {1, 1, 1};
  std::vector<float> recv_val;

  ps::KVWorker<float> w;
  w.Wait(w.Push(key, val));
  w.Wait(w.Pull(key, &recv_val));
```

More features:

- Flexible and high-performance communication: zero-copy push/pull, supporting
   dynamic length value, user-defined filters for communication compression
- Server-side programming: supporting user-defined handles on server nodes
- Flexible data consistency control via dependency DAG, allowing various
   consistency models such as eventual consistency, bounded delay, and BSP.

`ps-lite` is a part of the [DMLC](dmlc.github.io) project. It is
licensed under Apache 2.0.

### How to build

`ps-lite` requires a C++11 compiler such as `g++ >= 4.8` and `git`. You can
install them via
```
sudo apt-get update && sudo apt-get install -y build-essential git
```
for Ubuntu >= 13.10. Instructions for
[older Ubuntu](http://ubuntuhandbook.org/index.php/2013/08/install-gcc-4-8-via-ppa-in-ubuntu-12-04-13-04/),
[Centos](http://linux.web.cern.ch/linux/devtoolset/),
[Mac Os X](http://hpc.sourceforge.net/).

Then clone and build

```bash
git clone https://github.com/dmlc/ps-lite
cd ps-lite && make deps -j4 && make -j4
```
