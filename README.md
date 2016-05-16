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

