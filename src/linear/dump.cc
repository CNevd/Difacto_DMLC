#include <iostream>
#include <string>
#include <unordered_map>
#include "dmlc/io.h"
#include "dmlc/logging.h"

using namespace std;
using namespace dmlc;
typedef int64_t K;

class Dump {
 
 public:

  Dump(string file_in, string file_out) : file_in_(file_in),file_out_(file_out) {}
  ~Dump() {data_.clear();}

  struct SGDEntry {
    float w = 0;
    inline void Load(Stream *fi) {
      CHECK_EQ(fi->Read(&w, sizeof(float)), sizeof(float));
    }
    
    inline bool Empty() const { return w == 0;}
  };

  void LoadModel(const std::string filename) {
    Stream* fi = CHECK_NOTNULL(Stream::Create(filename.c_str(), "r"));
    K key;
    while (true) {
      if (fi->Read(&key, sizeof(K)) != sizeof(K)) break;
      data_[key].Load(fi);
    }
    cout << "loaded " << data_.size() << " kv pairs";
  }

  void DumpModel(const std::string filename) {
    Stream* fo = CHECK_NOTNULL(Stream::Create(filename.c_str(), "w"));
    dmlc::ostream os(fo);
    int saved = 0;
    for (const auto& it : data_) {
      if (it.second.Empty()) continue;
      os << it.first << '\t' << it.second.w << '\n';
      saved ++;
    }
    LOG(INFO) << "dumped " << saved << " kv pairs";
  }

  void run() {
    LoadModel(file_in_);
    DumpModel(file_out_);
  }

 private:
  unordered_map<K, SGDEntry> data_;
  string file_in_;
  string file_out_;
};

int main(int argc, char *argv[]) {
  if (argc < 1) {
    cout << "Usage: <model_in> <dump_out>" << endl;
    return 0;
  }
  google::InitGoogleLogging(argv[0]);
  string model_in, dump_out;
  for (int i = 1; i < argc; ++i) {
    char name[256], val[256];
    if (sscanf(argv[i], "%[^=]=%s", name, val) == 2) {
      if (!strcmp(name, "model_in")) model_in = val;
      if (!strcmp(name, "dump_out")) dump_out = val;
    }
  }
  Dump d(model_in, dump_out);
  d.run();
  return 0;
}


