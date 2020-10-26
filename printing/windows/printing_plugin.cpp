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

#include "include/printing/printing_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

#include "print_job.h"
#include "printing.h"

namespace nfet {

class PrintingPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows* registrar);

  PrintingPlugin();

  virtual ~PrintingPlugin();

 private:
  Printing printing{};
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

// static
void PrintingPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows* registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "net.nfet.printing",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<PrintingPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto& call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

PrintingPlugin::PrintingPlugin() {}

PrintingPlugin::~PrintingPlugin() {}

void PrintingPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue>& method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("printPdf") != 0) {
    auto job = printing.createJob();
    job->printPdf("name", 200, 200, 0, 0, 0, 0);
    printing.remove(job);
    result->Success(flutter::EncodableValue(0));
  } else if (method_call.method_name().compare("directPrintPdf") != 0) {
    auto job = printing.createJob();
    job->directPrintPdf("name", nullptr, "withPrinter");
    printing.remove(job);
    result->Success(flutter::EncodableValue(0));
  } else if (method_call.method_name().compare("sharePdf") != 0) {
    auto job = printing.createJob();
    job->sharePdf(nullptr, "name");
    printing.remove(job);
    result->Success(flutter::EncodableValue(0));
  } else if (method_call.method_name().compare("pickPrinter") != 0) {
    auto job = printing.createJob();
    job->pickPrinter(nullptr);
    printing.remove(job);
    result->Success(flutter::EncodableValue(0));
  } else if (method_call.method_name().compare("rasterPdf") != 0) {
    auto job = printing.createJob();
    job->rasterPdf(nullptr, {}, 1);
    printing.remove(job);
    result->Success(flutter::EncodableValue(0));
  } else if (method_call.method_name().compare("printingInfo") == 0) {
    auto job = printing.createJob();
    auto map = flutter::EncodableMap{};
    for (auto item : job->printingInfo()) {
      map[flutter::EncodableValue(item.first)] =
          flutter::EncodableValue(item.second);
    }
    result->Success(flutter::EncodableValue(map));
    printing.remove(job);
  } else {
    result->NotImplemented();
  }
}

}  // namespace nfet

void PrintingPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  nfet::PrintingPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
