/*-------------------------------------------------------------------------------
This file is part of ranger.

Copyright (c) [2014-2018] [Marvin N. Wright]

This software may be modified and distributed under the terms of the MIT license.

Please note that the C++ core of ranger is distributed under MIT license and the
R package "ranger" under GPL3 license.
#-------------------------------------------------------------------------------*/

// Ignore in coverage report (not used in R package)
// #nocov start
 
#ifndef DATAFLOAT_H_
#define DATAFLOAT_H_

#include "globals.h"
#include "Data.h"

class DataFloat: public Data {
public:
  DataFloat();
  DataFloat(double* data_double, std::vector<std::string> variable_names, size_t num_rows, size_t num_cols);
  virtual ~DataFloat();

  DataFloat(const DataFloat&)            = delete;
  DataFloat& operator=(const DataFloat&) = delete;

  double get(size_t row, size_t col) const {
    // Use permuted data for corrected impurity importance
    if (col >= num_cols) {
      col = getUnpermutedVarID(col);
      row = getPermutedSampleID(row);
    }

    if (col < num_cols_no_snp) {
      return data[col * num_rows + row];
    } else {
      // Get data out of snp storage. -1 because of GenABEL coding.
      size_t idx = (col - num_cols_no_snp) * num_rows_rounded + row;
      return (((snp_data[idx / 4] & mask[idx % 4]) >> offset[idx % 4]) - 1);
    }
  }

  void reserveMemory() {
    data = new float[num_cols * num_rows];
  }

  void set(size_t col, size_t row, double value, bool& error) {
    data[col * num_rows + row] = (float) value;
  }

private:
  float* data;
};

#endif /* DATAFLOAT_H_ */
// #nocov end

