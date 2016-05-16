/**
 * \file   progress.h
 * \brief The algorithm progress a worker/server report to the scheduler. It
 * should be printable, serializable, and mergable.
 */
#pragma once
#include <string>
#include <vector>
#include "dmlc/io.h"
#include "dmlc/logging.h"

namespace dmlc {

/// DEPRECATED

/**
 * \brief  The interface of progress
 */
class IProgress : public Serializable {
 public:
  IProgress() { }
  virtual ~IProgress() { }

  virtual void Clear() = 0;
  virtual bool Empty() const = 0;

  /// merger from another progress
  virtual void Merge(const IProgress* const other) = 0;

  /// head string for printing
  virtual std::string HeadStr() = 0;

  /// string for printing
  virtual std::string PrintStr(const IProgress* const prev) = 0;
};


class VectorProgress : public IProgress {
 public:
  VectorProgress() { Resize(0, 0); }
  VectorProgress(int inum, int fnum) { Resize(inum, fnum); }
  virtual ~VectorProgress() { }

  void Resize(int inum, int fnum) {
    fvec_.resize(fnum);
    ivec_.resize(inum);
  }

  virtual bool Empty() const {
    for (double f : fvec_) if (f != 0) return false;
    for (int64_t i : ivec_) if (i != 0) return false;
    return true;
  }

  virtual void Merge(const IProgress* const other) {
    const VectorProgress* const o = (VectorProgress const*) other;
    CHECK_EQ(fvec_.size(), o->fvec_.size());
    for (size_t i = 0; i < fvec_.size(); ++i) {
      fvec_[i] += o->fvec_[i];
    }
    CHECK_EQ(ivec_.size(), o->ivec_.size());
    for (size_t i = 0; i < ivec_.size(); ++i) {
      ivec_[i] += o->ivec_[i];
    }
  }

  virtual void Clear() {
    memset(fvec_.data(), 0, fvec_.size() * sizeof(double));
    memset(ivec_.data(), 0, ivec_.size() * sizeof(int64_t));
  }


  virtual void Load(Stream* fi) {
    fi->Read(&fvec_);
    fi->Read(&ivec_);
  }

  virtual void Save(Stream *fo) const {
    fo->Write(fvec_);
    fo->Write(ivec_);
  }

 protected:
  std::vector<double> fvec_;
  std::vector<int64_t> ivec_;
};

}  // namespace dmlc

// namespace wormhole {

// /**
//  * \brief  Serializable progress for objective value, accuracy, ...
//  */
// class Progress : public Serializable {
//  public:
//   Progress() { }
//   virtual ~Progress() { }

//   /// \brief merge from another progress
//   virtual void Merge(const Progress* const other) {
//     CHECK_EQ(other->vec_.size(), vec_.size());
//     for (size_t i = 0; i < vec_.size(); ++i) {
//       vec_[i] += other->vec_[i];
//     }
//   }

//   /// \brief Returns a header string for printing
//   virtual std::string HeadStr() = 0;

//   /// \brief Returns a string for printing
//   virtual std::string PrintStr() = 0;

//   virtual void Load(Stream* fi) { fi->Read(&vec_); }
//   virtual void Save(Stream *fo) const { fo->Write(vec_); }

//   double& objv() { return vec_[0]; }
//   double objv() const { return vec_[0]; }
//  protected:
//   std::vector<double> vec_;
// };

// }  // namespace wormhole
