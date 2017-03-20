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
  struct AdaGradEntry {

    AdaGradEntry() { }
    ~AdaGradEntry() { Clear(); }

    inline void Clear() {
      if ( size > 1 ) { delete [] w; delete [] sqc_grad; }
      size = 0; w = NULL; sqc_grad = NULL;
    }

    /// length of w. if size == 1, then using w itself to store the value to save
    /// memory and avoid unnecessary new (see w_0())
    int size = 1;
    /// w and V
    float *w = NULL;
    /// square root of the cumulative gradient
    float *sqc_grad = NULL;

    inline float w_0() const { return size == 1 ? *(float *)&w : w[0]; }
    inline float sqc_grad_0() const {
      return size == 1 ? *(float *)&sqc_grad : sqc_grad[0];
    }

    void Load(Stream* fi) {
    fi->Read(&size, sizeof(size)) ;
    if (size == 1) {
      fi->Read(&w, sizeof(float*));
      fi->Read(&sqc_grad, sizeof(float*));
    } else {
      w = new float[size];
      sqc_grad = new float[size+1];
      fi->Read(w, sizeof(float)*size);
      fi->Read(sqc_grad, sizeof(float)*(size+1));
    }
  }

    bool Empty() const { return (w_0() == 0 && size == 1); }
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
      os << feature_id << '\t';
      os << it.second.size;
      if (it.second.size == 1) {
        os << '\t' << *(float *)&(it.second.w) << '\t' << *(float *)&(it.second.sqc_grad) << '\n';
      } else {
        for (int i = 0; i < it.second.size; ++i) {
          os << '\t' << it.second.w[i];
        }
        os << '\t' << it.second.sqc_grad_0() << '\n';
      }
      dumped ++;
    }
    cout << "dumped " << dumped << " kv pairs\n";
  }

  void run() {
    LoadModel(file_in_);
    DumpModel(file_out_);
  }

 private:
  unordered_map<K, AdaGradEntry> data_;
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


