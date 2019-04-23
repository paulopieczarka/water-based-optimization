#include "DataSet.hpp"

DataSet::DataSet (std::string fileSrc, char delimiter, int labelIndex) :
  fileSrc(fileSrc),
  delimiter(delimiter),
  labelIndex(labelIndex) {
}

DataSet::DataSet (std::string fileSrc, char delimiter) :
  fileSrc(fileSrc),
  delimiter(delimiter),
  labelIndex(0) {
}

DataSet::DataSet (std::string fileSrc) :
  fileSrc(fileSrc),
  delimiter(','),
  labelIndex(0) {
}

DataSet::~DataSet () { }

bool DataSet::read () {
  std::string line;
  std::ifstream file (this->fileSrc);

  if (file.is_open()) {
    std::cout << "-> Reading file: " << this->fileSrc << std::endl;

    while (std::getline(file, line)) {
      this->data.push_back(this->split(line, this->delimiter));
    }

    std::cout << "-> Loaded " << this->data.size() << " instances with ";
    std::cout << this->data[0].size() << " features." << std::endl;
    return true;
  }

  std::cout << "-> Could not load file: " << this->fileSrc << std::endl;
  return false;
}

std::vector<std::string> DataSet::getLine (int row) {
  std::vector<std::string> cols;
  return cols;
}

std::string DataSet::getData (int row, int col) {
  return "empty.";
}

std::vector<std::string> DataSet::split(const std::string& s, char delimiter) {
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}
