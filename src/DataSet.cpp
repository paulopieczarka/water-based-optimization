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

    std::cout << "-> Loaded " << this->instances() << " instances with ";
    std::cout << this->features() << " features." << std::endl;
    return true;
  }

  std::cout << "-> Could not load file: " << this->fileSrc << std::endl;
  return false;
}

int DataSet::instances () {
  return this->data.size();
}

int DataSet::features () {
  return this->data[0].size();
}

std::vector<std::string> DataSet::rows (int row) {
  return this->data[row];
}

std::string DataSet::item (int row, int col) {
  return this->data[row][col];
}

double DataSet::itemAsDouble (int row, int col) {
  return std::stod(this->item(row, col));
}

std::vector<std::string> DataSet::split (const std::string& s, char delimiter) {
   std::vector<std::string> tokens;
   std::istringstream tokenStream(s);

   std::string token;
   while (std::getline(tokenStream, token, delimiter)) {
      tokens.push_back(token);
   }

   return tokens;
}
