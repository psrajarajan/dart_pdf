/*
 * Copyright (C) 2017, David PHAM-VAN <dev.nfet.net@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PRINTING_PLUGIN_PRINT_JOB_H_
#define PRINTING_PLUGIN_PRINT_JOB_H_

#include <flutter/standard_method_codec.h>

#include <map>
#include <vector>
#include <memory>
#include <sstream>

namespace nfet {

class Printing;

class PrintJob {
 private:
  Printing* printing;
  int index;

 public:
  PrintJob(Printing* printing, int index);

  void directPrintPdf(std::string name,
                      std::vector<uint8_t> data,
                      std::string withPrinter);

  void printPdf(std::string name,
                double width,
                double height,
                double marginLeft,
                double marginTop,
                double marginRight,
                double marginBottom);

  void cancelJob(std::string error);

  void sharePdf(std::vector<uint8_t> data, std::string name);

  void pickPrinter(void* result);

  void rasterPdf(std::vector<uint8_t> data, std::vector<int> pages, double scale);

  std::map<std::string, bool> printingInfo();
};

}  // namespace nfet

#endif  // PRINTING_PLUGIN_PRINT_JOB_H_
