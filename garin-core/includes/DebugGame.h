#pragma once
#include <imgui.h>
#include <iostream>
#include <vector>
#include <Graphics.h>
#include <sstream>
#include <iomanip>

using namespace std;

class Message_Type;

class DebugGame
{
private:
    static DebugGame *instance;
    bool render_console_debug = false;

public:
    enum msg_types
    {
        logger,
        warning,
        error
    };

    vector<Message_Type> messages = vector<Message_Type>();

    static void init_console();
    static DebugGame *get_debug();
    static void add_message(string p_msg, DebugGame::msg_types msg_type);

    static void restore_console();
    static void run_debug_console();
    static void active_console(bool toggle_value);
    static bool console_is_active();
};

class Message_Type
{
public:
    DebugGame::msg_types msg_type;
    string message;
};