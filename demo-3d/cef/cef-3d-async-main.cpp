// HTML 5 GUI Demo
// Copyright (c) 2019 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "DemoCefApp.hpp"
#include "DemoCefRendererApp.hpp"

#include <cef_cmake/disable_warnings.h>
#include <include/cef_app.h>
#include <cef_cmake/reenable_warnings.h>

#include <demo/Demo.hpp>

#include "URL.h"

int main(int argc, char* argv[])
{
    // printf("main() called with args:\n");
    // for (int i = 0; i < argc; ++i) {
    //     printf("\t%d: '%s'\n", i, argv[i]);
    // }

    CefRefPtr<CefCommandLine> commandLine = CefCommandLine::CreateCommandLine();
#if defined(_WIN32)
    CefEnableHighDPISupport();
    CefMainArgs args(GetModuleHandle(NULL));
    commandLine->InitFromString(GetCommandLineW());
#else
    CefMainArgs args(argc, argv);
    commandLine->InitFromArgv(argc, argv);
#endif

    void* windowsSandboxInfo = NULL;

#if defined(CEF_USE_SANDBOX) && defined(_WIN32)
    // Manage the life span of the sandbox information object. This is necessary
    // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
    CefScopedSandboxInfo scopedSandbox;
    windowsSandboxInfo = scopedSandbox.sandbox_info();
#endif

    CefRefPtr<CefApp> rendererApp = nullptr;
    std::string appType = commandLine->GetSwitchValue("type");
    if (appType == "renderer" || appType == "zygote") {
        rendererApp = new DemoCefRendererApp;
    }

    printf("App type: %s\n", appType.c_str());
    
    int result = CefExecuteProcess(args, rendererApp, windowsSandboxInfo);
    if (result >= 0)
    {
        printf("\tApp type %s - completed\n", appType.c_str());
        // child process completed
        return result;
    }

    printf("Running main demo loop.\n");

    CefSettings settings;
    settings.remote_debugging_port = 1234;
    settings.windowless_rendering_enabled = 0;
#if !defined(CEF_USE_SANDBOX)
    settings.no_sandbox = true;
#endif

    CefInitialize(args, settings, nullptr, windowsSandboxInfo);

    // run sokol demo
    int ret = demo::Demo::run(std::make_unique<DemoCefApp>());

    CefShutdown();

    return ret;
}

