#pragma once
#include "io/filesys.h"
#include "dmlc/logging.h"
#include <vector>
#include <string>
#include <sstream>
#include <regex.h>
namespace dmlc {


// match files by regex pattern
// such as s3://my_path/part-.*
inline std::vector<std::string> split(const std::string &s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    elems.push_back(std::move(item));
  }
  return elems;
}

inline void MatchFile_single(const std::string& pattern,
                      std::vector<std::string>* matched) {
  // get the path
  size_t pos = pattern.find_last_of("/\\");
  std::string path = "./";
  if (pos != std::string::npos) path = pattern.substr(0, pos);

  // find all files
  dmlc::io::URI path_uri(path.c_str());
  dmlc::io::FileSystem *fs =
      dmlc::io::FileSystem::GetInstance(path_uri.protocol);
  std::vector<io::FileInfo> info;
  fs->ListDirectory(path_uri, &info);

  // store all matached files
  regex_t pat;
  std::string file =
      pos == std::string::npos ? pattern : pattern.substr(pos+1);
  file = ".*" + file;
  int status = regcomp(&pat, file.c_str(), REG_EXTENDED|REG_NEWLINE);
  if (status != 0) {
	char error_message[1000];
	regerror(status, &pat, error_message, 1000);
    LOG(FATAL) << "error regex '" << pattern << "' : " << error_message;
  }

  regmatch_t m[1];
  CHECK_NOTNULL(matched);
  for (size_t i = 0; i < info.size(); ++i) {
    std::string file = info[i].path.str();
    if (regexec(&pat, file.c_str(), 1, m, 0)) continue;
    matched->push_back(file);
  }
  regfree(&pat);
}

inline void MatchFile(const std::string& pattern,
                      std::vector<std::string>* matched) {
  char delim[] = ";";
  std::vector<std::string> patterns;
  patterns = split(pattern, *delim);
  for(auto p : patterns) {
    MatchFile_single(p, matched);
  }
}
} // namespace dmlc



