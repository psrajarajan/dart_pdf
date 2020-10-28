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

void PrintJob::printPdf(std::string name) {
  PRINTDLG pd;
  // HWND hwnd;

  // Initialize PRINTDLG
  ZeroMemory(&pd, sizeof(pd));
  pd.lStructSize = sizeof(pd);

  // Initialize PRINTDLG
  pd.hwndOwner = nullptr;
  pd.hDevMode = nullptr;   // Don't forget to free or store hDevMode.
  pd.hDevNames = nullptr;  // Don't forget to free or store hDevNames.
  pd.hDC = nullptr;
  pd.Flags = 0x0004014C;
  pd.nCopies = 1;
  pd.nFromPage = 0xFFFF;
  pd.nToPage = 0xFFFF;
  pd.nMinPage = 1;
  pd.nMaxPage = 0xFFFF;

  auto r = PrintDlg(&pd);

  if (r == 1) {
    printf("ncopies: %d\n", pd.nCopies);
    // printf("hDevMode: %d\n", (int)pd.hDevMode);

    // DEVMODE* b = static_cast<DEVMODE*>(GlobalLock(pd.hDevMode));
    // auto pageDpi = b->dmPrintQuality;

    // GlobalUnlock(pd.hDevMode);

    // auto pageHeight = b->dmPaperLength;
    // auto pageWidth = b->dmPaperWidth;
    // auto pageScale = b->dmScale / 100;

    auto dpiX = static_cast<double>(GetDeviceCaps(pd.hDC, LOGPIXELSX)) / 72;
    auto dpiY = static_cast<double>(GetDeviceCaps(pd.hDC, LOGPIXELSY)) / 72;
    auto pageWidth =
        static_cast<double>(GetDeviceCaps(pd.hDC, PHYSICALWIDTH)) / dpiX;
    auto pageHeight =
        static_cast<double>(GetDeviceCaps(pd.hDC, PHYSICALHEIGHT)) / dpiY;
    auto printableWidth =
        static_cast<double>(GetDeviceCaps(pd.hDC, HORZRES)) / dpiX;
    auto printableHeight =
        static_cast<double>(GetDeviceCaps(pd.hDC, VERTRES)) / dpiY;
    auto marginLeft =
        static_cast<double>(GetDeviceCaps(pd.hDC, PHYSICALOFFSETX)) / dpiX;
    auto marginTop =
        static_cast<double>(GetDeviceCaps(pd.hDC, PHYSICALOFFSETY)) / dpiY;
    auto marginRight = pageWidth - printableWidth - marginLeft;
    auto marginBottom = pageHeight - printableHeight - marginTop;

    printf("dpiX: %f\n", dpiX);
    printf("HORZRES: %d\n", GetDeviceCaps(pd.hDC, HORZRES));
    printf("PHYSICALOFFSETX: %d\n", GetDeviceCaps(pd.hDC, PHYSICALOFFSETX));
    printf("pageWidth: %f\n", pageWidth);

    hDC = pd.hDC;
    hDevMode = pd.hDevMode;
    hDevNames = pd.hDevNames;

    printf("HDC: %llu  job: %d\n", (size_t)pd.hDC, index);

    printing->onLayout(this, pageWidth, pageHeight, marginLeft, marginTop,
                       marginRight, marginBottom);
  }
}

void PrintJob::writeJob(std::vector<uint8_t> data) {
  printf("hDC: %llu  job: %d\n", (size_t)hDC, index);
  auto dpiX = static_cast<double>(GetDeviceCaps(hDC, LOGPIXELSX)) / 72;
  auto dpiY = static_cast<double>(GetDeviceCaps(hDC, LOGPIXELSY)) / 72;

  // GlobalFree(pd.hDevMode);

  // print(
  //  'Paper size: ${pageWidth} ${pageHeight}  scale: $pageScale  dpi:
  //  $pageDpi');

  //     final printerDC = CreateDCW(nullptr, szPrinter,nullptr,
  //     devmode);
  DOCINFO info;

  // memset(&info, 0, sizeof(info));
  ZeroMemory(&info, sizeof(info));
  info.cbSize = sizeof(info);
  // info.fwType = 0;
  // info.lpszDatatype = nullptr;
  // info.lpszDocName = nullptr;
  // info.lpszOutput = nullptr;
  // auto printerDC = pd.hDC;
  // print('hDC: ${pd.hDC}');
  auto r = StartDoc(hDC, &info);
  // print('StartDoc = $r');

  FPDF_InitLibraryWithConfig(nullptr);
  // final buffer = allocate<ffi.Uint8>(count : bytes.length);
  // final nativeBuffer = buffer.asTypedList(bytes.length);
  // nativeBuffer.setAll(0, bytes);

  // auto buffer = std::vector<uint8_t>{};

  auto doc = FPDF_LoadMemDocument64(data.data(), data.size(), nullptr);
  if (!doc) {
    printf("Error loading the document: %d\n", FPDF_GetLastError());
    return;
  }

  auto pages = FPDF_GetPageCount(doc);
  printf("Page count: %d\n", pages);

  for (auto pageNum = 0; pageNum < pages; pageNum++) {
    r = StartPage(hDC);
    printf("StartPage = %d\n", r);

    auto page = FPDF_LoadPage(doc, pageNum);
    // print(FPDF_GetLastError());

    auto pdfWidth = FPDF_GetPageWidth(page);
    auto pdfHeight = FPDF_GetPageHeight(page);

    // print('$width x $height');

    printf("pdfWidth: %f  dpiX: %f  \n", pdfWidth, dpiX);
    int bWidth = static_cast<int>(pdfWidth * dpiX);
    int bHeight = static_cast<int>(pdfHeight * dpiY);

    printf("bwidth/bheight: %d x %d\n", bWidth, bHeight);

    FPDF_RenderPage(hDC, page, 0, 0, bWidth, bHeight, 0, FPDF_ANNOT);

    r = EndPage(hDC);
    printf("EndPage = %d\n", r);
  }

  FPDF_CloseDocument(doc);
  FPDF_DestroyLibrary();

  r = EndDoc(hDC);
  printf("EndDoc = %d\n", r);
  //     DeleteDC(printerDC);
  DeleteDC(hDC);
  GlobalFree(hDevNames);
  ClosePrinter(hDevMode);
}  // namespace nfet

void PrintJob::cancelJob(std::string error) {}

void PrintJob::sharePdf(std::vector<uint8_t> data, std::string name) {}

void PrintJob::pickPrinter(void* result) {}

void PrintJob::rasterPdf(std::vector<uint8_t> data,
                         std::vector<int> pages,
                         double scale) {
  FPDF_InitLibraryWithConfig(nullptr);

  auto doc = FPDF_LoadMemDocument64(data.data(), data.size(), nullptr);
  if (!doc) {
    fprintf(stderr, "Error: %d\n", FPDF_GetLastError());
    printing->onPageRasterEnd(this);
    return;
  }

  auto pageCount = FPDF_GetPageCount(doc);
  printf("pdf: pages:%d\n", pageCount);

  for (auto n : pages) {
    if (n >= pageCount) {
      continue;
    }

    auto page = FPDF_LoadPage(doc, n);
    if (!page) {
      printf("Page Error: %d\n", FPDF_GetLastError());
      continue;
    }

    auto width = FPDF_GetPageWidth(page);
    auto height = FPDF_GetPageHeight(page);

    printf("pdf: page:%d w:%f h:%f\n", n, width, height);

    auto bWidth = static_cast<int>(width * scale);
    auto bHeight = static_cast<int>(height * scale);

    auto bitmap = FPDFBitmap_Create(bWidth, bHeight, 0);
    FPDFBitmap_FillRect(bitmap, 0, 0, bWidth, bHeight, 0xffffffff);

    FPDF_RenderPageBitmap(bitmap, page, 0, 0, bWidth, bHeight, 0, FPDF_ANNOT);

    uint8_t* p = static_cast<uint8_t*>(FPDFBitmap_GetBuffer(bitmap));
    size_t l = static_cast<size_t>(bHeight * FPDFBitmap_GetStride(bitmap));

    printing->onPageRasterized(std::vector<uint8_t>{p, p + l}, bWidth, bHeight,
                               this);

    FPDFBitmap_Destroy(bitmap);
  }

  FPDF_CloseDocument(doc);

  FPDF_DestroyLibrary();

  printing->onPageRasterEnd(this);
}  // namespace nfet

std::map<std::string, bool> PrintJob::printingInfo() {
  return std::map<std::string, bool>{
      {"directPrint", true},     {"dynamicLayout", false}, {"canPrint", true},
      {"canConvertHtml", false}, {"canShare", true},       {"canRaster", true},
  };
}

}  // namespace nfet
