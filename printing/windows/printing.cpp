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

#include "printing.h"

#include "print_job.h"

namespace nfet {

Printing::Printing(
    flutter::MethodChannel<flutter::EncodableValue>*channel)
    : channel(channel) {}

Printing::~Printing() {}

PrintJob* Printing::createJob(int num) {
  return new PrintJob{this, num};
}

void Printing::remove(PrintJob* job) {
  delete job;
}

void Printing::onPageRasterized(std::vector<uint8_t> data,
                                double width,
                                double height,
                                int job) {
  channel->InvokeMethod(
      "onPageRasterized",
      std::make_unique<flutter::EncodableValue>(
          flutter::EncodableValue(flutter::EncodableMap{
              {flutter::EncodableValue("image"), flutter::EncodableValue(data)},
              {flutter::EncodableValue("width"),
               flutter::EncodableValue(width)},
              {flutter::EncodableValue("height"),
               flutter::EncodableValue(height)},
              {flutter::EncodableValue("job"), flutter::EncodableValue(job)}

          })));
}

void Printing::onPageRasterEnd(int job) {
  channel->InvokeMethod(
      "onPageRasterEnd",
      std::make_unique<flutter::EncodableValue>(
          flutter::EncodableValue(flutter::EncodableMap{
              {flutter::EncodableValue("job"), flutter::EncodableValue(job)}

          })));
}

}  // namespace nfet
