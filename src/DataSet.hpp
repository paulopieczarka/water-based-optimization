#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

class DataSet {
  public:
    std::string fileSrc;

    DataSet(std::string fileSrc, char delimiter, int labelIndex);
    DataSet(std::string fileSrc, char delimiter);
    DataSet(std::string fileSrc);
    ~DataSet();

    bool read();

    int instances();
    int features();

    std::string item(int row, int col);
    double itemAsDouble(int row, int col);
    std::vector<std::string> rows(int row);

    // TODO: implement drop rows
    // TODO: implement drop cols
    // TODO: create trainingset x testset
    // TODO: normalize database

  private:
    char delimiter;

    int labelIndex;

    std::vector<std::string> headers;
    std::vector<std::vector<std::string> > data;

    // Util
    std::vector<std::string> split(const std::string& s, char delimiter);
};
