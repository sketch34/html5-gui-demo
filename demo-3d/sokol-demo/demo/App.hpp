// HTML 5 GUI Demo
// Copyright (c) 2019 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <memory>

struct sapp_event;
struct sapp_desc;

namespace demo
{

class GUI;
class Demo;

class RotatingCube;

struct vec3
{
    float c[3];
};

struct int3
{
    int c[3];
};

class App
{
public:
    App();
    virtual ~App();

    void init();
    void mainLoop();
    void shutdown();
    void onEvent(const sapp_event& e);

    static double_t getTicks();

protected:
    virtual GUI* createGUI() = 0;

private:
    friend class Demo;

    std::unique_ptr<RotatingCube> m_cube;

    GUI* m_gui = nullptr;

    vec3 m_rotation = {};
    int3 m_revolutions = {};

    // times and frames
    double_t m_currentFrameTime = 0; // start of current frame (ms)
    double_t m_timeSinceLastFrame = 0; // used as delta for updates
};

}
