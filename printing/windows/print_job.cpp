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

#include "printing.h"

#include <fpdfview.h>

namespace nfet {

PrintJob::PrintJob(Printing* printing, int index)
    : printing(printing), index(index) {}

void PrintJob::directPrintPdf(std::string name,
                              std::vector<uint8_t> data,
                              std::string withPrinter) {}

void PrintJob::printPdf(std::string name,
                        double width,
                        double height,
                        double marginLeft,
                        double marginTop,
                        double marginRight,
                        double marginBottom) {}

void PrintJob::cancelJob(std::string error) {}

void PrintJob::sharePdf(std::vector<uint8_t> data, std::string name) {}

void PrintJob::pickPrinter(void* result) {}

void PrintJob::rasterPdf(std::vector<uint8_t> data,
                         std::vector<int> pages,
                         double scale) {
  FPDF_InitLibraryWithConfig(nullptr);

  auto doc = FPDF_LoadMemDocument64(data.data(), data.size(), nullptr);
  printf("Error: %d\n", FPDF_GetLastError());

  auto pageCount = FPDF_GetPageCount(doc);

  for (auto n : pages) {
    if (n >= pageCount) {
      continue;
    }

    auto page = FPDF_LoadPage(doc, n);
    printf("Error: %d\n", FPDF_GetLastError());

    auto width = FPDF_GetPageWidth(page);
    auto height = FPDF_GetPageHeight(page);

    printf("pdf: pages:%d w:%f h:%f\n", pageCount, width, height);

    auto bWidth = static_cast<int>(width);
    auto bHeight = static_cast<int>(height);

    auto bitmap = FPDFBitmap_Create(bWidth, bHeight, 0);
    FPDFBitmap_FillRect(bitmap, 0, 0, bWidth, bHeight, 0xffffffff);

    FPDF_RenderPageBitmap(bitmap, page, 0, 0, bWidth, bHeight, 0, 0);

    uint8_t* p = static_cast<uint8_t*>(FPDFBitmap_GetBuffer(bitmap));
    size_t l = bHeight * FPDFBitmap_GetStride(bitmap);

    printing->onPageRasterized(std::vector<uint8_t>{p, p + l}, width, height, index);
  }

  FPDF_CloseDocument(doc);

  FPDF_DestroyLibrary();

  printing->onPageRasterEnd(index);
}

std::map<std::string, bool> PrintJob::printingInfo() {
  return std::map<std::string, bool>{
      {"directPrint", true},     {"dynamicLayout", false}, {"canPrint", true},
      {"canConvertHtml", false}, {"canShare", true},       {"canRaster", true},
  };
}

}  // namespace nfet
