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

#include "print_job.h"

namespace nfet {

PrintJob::PrintJob(Printing* printing, int index)
    : printing(printing), index(index) {}

void PrintJob::directPrintPdf(std::string name,
                              std::string data,
                              std::string withPrinter) {}

void PrintJob::printPdf(std::string name,
                        double width,
                        double height,
                        double marginLeft,
                        double marginTop,
                        double marginRight,
                        double marginBottom) {}

void PrintJob::cancelJob(std::string error) {}

void PrintJob::sharePdf(std::string data, std::string name) {}

void PrintJob::pickPrinter(void* result) {}

void PrintJob::rasterPdf(std::string data, int pages[], double scale) {}

std::map<std::string, bool> PrintJob::printingInfo() {
  return std::map<std::string, bool>{
      {"directPrint", true},     {"dynamicLayout", false}, {"canPrint", true},
      {"canConvertHtml", false}, {"canShare", true},       {"canRaster", true},
  };
}

}  // namespace nfet
