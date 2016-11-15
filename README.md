# Distributed FM and LR with parameter server

[![GitHub license](http://dmlc.github.io/img/apache2.svg)](./LICENSE)

Portable, scalable and reliable distributed machine learning.

Wormhole is a place where DMLC projects works together to provide
scalable and reliable machine learning toolkits that can run on various platforms

Since wormhole has been Deprecated, we retain some useful tools and keep updating

Features
====
* Portable:
  - Supported platforms: local machine, Apache YARN, MPI and Sungrid Engine
* Rich support of Data Source
  - All projects can read data from HDFS, S3 or local filesystem
* Scalable and Reliable

List of Tools
====
* Linear method: [linear](src/linear)
* Factorization Machine based on PS: [difacto](src/difacto)

Build & Run
====

* Requires a C++11 compiler (e.g.~`g++ >=4.8`) and `git`. Install them on Ubuntu >= 13.10

* `cd dmlc-core; make` to make dmlc core  
  `cd ps-lite; make` to make ps  
  `cd src/linear; make` or `cd src/difacto; make`

Questions
====
* How to set multipath?  
  if you have date paths like: `./data/train1` `./data/train2`  
  pls set `train_data = "./data/train1;./data/train2"` or `"./data/train.*"`  
  for hdfsfiles: `train_data = "hdfs://data/train1;hdfs://data/train2"` or `"hdfs://data/train.*"`  
* How to use HDFS?  
  set `USE_HDFS=1` in [dmlc-core/make/config.mk](dmlc-core/make/config.mk) and [ps-lite/make/config.mk](ps-lite/make/config.mk)  
* How to get readable weight?  
  use `./build/dump.dmlc model_in=your_model_path(should be local file) dump_out=dump_file need_inverse=1(0 or 1)` then dump_file is the readable weight  
* dump error when use hdfs: `./build/dump.dmlc: error while loading shared libraries: libhdfs.so.0.0.0: cannot open shared object file: No such file or directory`?  
  pls add hadoop lib path to LD_LIBRARY_PATH before dump  
  in my case: `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/clusterserver/hadoop/lib/native/`  
* Why the ids of dumped file is large like `-2305843009213693952` and how to use the original ids?  
  see [issue8](https://github.com/CNevd/Difacto_DMLC/issues/8) and [issue10](https://github.com/CNevd/Difacto_DMLC/issues/10)
