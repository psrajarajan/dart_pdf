#ifndef PRINTING_PLUGIN_PRINT_JOB_H_
#define PRINTING_PLUGIN_PRINT_JOB_H_

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
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

#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

#include "printing.h"

namespace printing {
class PrintJob {
 private:
  std::shared_ptr<PrintingPlugin> printing;
  int index;

 public:
  PrintJob(std::shared_ptr<PrintingPlugin> printing, int index);

  void directPrintPdf(std::string name,
                      std::string data,
                      std::string withPrinter);

  void printPdf(std::string name,
                double width,
                double height,
                double marginLeft,
                double marginTop,
                double marginRight,
                double marginBottom);

  void cancelJob(std::string error);

  void sharePdf(std::string data, std::string name);

  void pickPrinter(void* result);

  void rasterPdf(std::string data, int pages[], double scale);

  static flutter::EncodableMap printingInfo();
};

}  // namespace printing

#endif  // PRINTING_PLUGIN_PRINT_JOB_H_
