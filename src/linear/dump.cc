#include <iostream>
#include <string>
#include <unordered_map>
#include "dmlc/io.h"
#include "dmlc/logging.h"
#include "base/localizer.h"

using namespace std;
using namespace dmlc;
typedef uint64_t K;

class Dump {
 
 public:

  Dump(string file_in, string file_out, bool need_inverse) : file_in_(file_in),file_out_(file_out), need_inverse_(need_inverse) {}
  ~Dump() {data_.clear();}

  // value type stored on sever nodes, can be also other Entrys
  struct FTRLEntry {
    float w = 0;
    float z= 0;
    float sq_cum_grad = 0;
    inline void Load(Stream *fi) {
      CHECK_EQ(fi->Read(&w, sizeof(float)), sizeof(float));
      CHECK_EQ(fi->Read(&z, sizeof(float)), sizeof(float));
      CHECK_EQ(fi->Read(&sq_cum_grad, sizeof(float)), sizeof(float));
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
    cout << "loaded " << data_.size() << " kv pairs\n";
  }

  // how to dump the info
  void DumpModel(const std::string filename) {
    Stream* fo = CHECK_NOTNULL(Stream::Create(filename.c_str(), "w"));
    dmlc::ostream os(fo);
    int dumped = 0;
    for (const auto& it : data_) {
      if (it.second.Empty()) continue;
      uint64_t feature_id = need_inverse_ ? ReverseBytes(it.first) : it.first;
      os << feature_id << '\t' << it.second.w << '\t' << it.second.z << '\t' << it.second.sq_cum_grad <<'\n';  // check your entry
      dumped ++;
    }
    cout << "dumped " << dumped << " kv pairs\n";
  }

  void run() {
    LoadModel(file_in_);
    DumpModel(file_out_);
  }

 private:
  unordered_map<K, FTRLEntry> data_;
  string file_in_;
  string file_out_;
  bool need_inverse_;
};

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cout << "Usage: <model_in> <dump_out> [need_inverse]\n";
    return 0;
  }
  google::InitGoogleLogging(argv[0]);
  string model_in, dump_out;
  bool need_inverse = false;
  for (int i = 1; i < argc; ++i) {
    char name[256], val[256];
    if (sscanf(argv[i], "%[^=]=%s", name, val) == 2) {
      if (!strcmp(name, "model_in")) model_in = val;
      if (!strcmp(name, "dump_out")) dump_out = val;
      if (!strcmp(name, "need_inverse")) need_inverse = !strcmp(val, "0") ? false : true;
    }
  }
  Dump d(model_in, dump_out, need_inverse);
  d.run();
  return 0;
}


