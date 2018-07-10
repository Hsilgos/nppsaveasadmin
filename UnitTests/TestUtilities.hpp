#pragma once

#include <memory>
#include <string>
#include <vector>

class FileAutoremover {
 public:
  FileAutoremover(std::string filename) : FileAutoremover({filename}) {}

  FileAutoremover(std::initializer_list<std::string> filenames)
      : m_filenames(filenames) {
    remove();
  }

  ~FileAutoremover() { remove(); }

 private:
  void remove() const {
    for (const std::string& filename : m_filenames) {
      std::remove(filename.c_str());
    }
  }
  const std::vector<std::string> m_filenames;
};
