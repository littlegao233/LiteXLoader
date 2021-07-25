#include "CommandAPI.h"
#include "APIHelp.h"
#include "PlayerAPI.h"
#include "EngineGlobalData.h"
#include "EngineOwnData.h"
#include "LoaderHelper.h"
#include <Kernel/Base.h>
#include <Kernel/Global.h>
#include <filesystem>
#include <Configs.h>
#include <vector>
#include <string>
using namespace std;

//////////////////// APIs ////////////////////

Local<Value> Runcmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)

    try {
        return Boolean::newBoolean(Raw_Runcmd(args[0].asString().toString()));
    }
    CATCH("Fail in RunCmd!")
}

Local<Value> RuncmdEx(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)

    try {
        std::pair<bool, string> result = Raw_RuncmdEx(args[0].asString().toString());
        Local<Object> resObj = Object::newObject();
        resObj.set("success", result.first);
        resObj.set("output", result.second);
        return resObj;
    }
    CATCH("Fail in RunCmdEx!")
}


// Helper
void LxlRegisterNewCmd(bool isPlayerCmd, string cmd, const string& describe, int level, Local<Function> func)
{
    if (cmd[0] == '/')
        cmd = cmd.erase(0, 1);

    if(isPlayerCmd)
        engineLocalData->playerCmdCallbacks[cmd] = { EngineScope::currentEngine(),level,Global<Function>(func) };
    else
        engineLocalData->consoleCmdCallbacks[cmd] = { EngineScope::currentEngine(),level,Global<Function>(func) };

    //延迟注册
    if (isCmdRegisterEnabled)
        Raw_RegisterCmd(cmd, describe, level);
    else
        toRegCmdQueue.push_back({ cmd, describe, level });
}

bool LxlRemoveCmdRegister(ScriptEngine* engine)
{
    for (auto& cmdData : engineLocalData->playerCmdCallbacks)
    {
        if (cmdData.second.fromEngine == engine)
            engineLocalData->playerCmdCallbacks.erase(cmdData.first);
    }

    for (auto& cmdData : engineLocalData->consoleCmdCallbacks)
    {
        if (cmdData.second.fromEngine == engine)
            engineLocalData->playerCmdCallbacks.erase(cmdData.first);
    }
    return true;
}
// Helper

Local<Value> RegisterPlayerCmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 3);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);
    CHECK_ARG_TYPE(args[2], ValueKind::kFunction);
    if (args.size() >= 4)
        CHECK_ARG_TYPE(args[3], ValueKind::kNumber);

    try {
        string cmd = args[0].asString().toString();
        string describe = args[1].asString().toString();
        int level = 0;

        if (args.size() >= 4)
        {
            int newLevel = args[3].asNumber().toInt32();
            if (newLevel >= 0 && newLevel <= 3)
                level = newLevel;
        }

        LxlRegisterNewCmd(true, cmd, describe, level, args[2].asFunction());
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in RegisterPlayerCmd!");
}

Local<Value> RegisterConsoleCmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 3);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);
    CHECK_ARG_TYPE(args[2], ValueKind::kFunction);

    try {
        string cmd = args[0].asString().toString();
        string describe = args[1].asString().toString();

        LxlRegisterNewCmd(false, cmd, describe, 4, args[2].asFunction());
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in RegisterConsoleCmd!");
}

Local<Value> SendCmdOutput(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try {
        return Boolean::newBoolean(Raw_SendCmdOutput(args[0].toStr()));
    }
    CATCH("Fail in SendCmdOutput!");
}


//////////////////// LXL Event Callbacks ////////////////////

// Helper
vector<string> SplitCmdParas(const string& paras)
{
    if (paras.empty())
        return vector<string>();

    vector<string> res;
    string now, strInQuote = "";
    istringstream strIn(paras);
    while (strIn >> now)
    {
        if (!strInQuote.empty())
        {
            strInQuote = strInQuote + " " + now;
            if (now.back() == '\"')
            {
                strInQuote.pop_back();
                res.push_back(strInQuote.erase(0, 1));
                strInQuote = "";
            }
        }
        else
        {
            if (now.front() == '\"')
                strInQuote = now;
            else
                res.push_back(now);
        }
    }
    if (!strInQuote.empty())
    {
        istringstream leftIn(strInQuote);
        while (leftIn >> now)
            res.push_back(now);
    }
    return res;
}
// Helper


void RegisterBuiltinCmds()
{
    //调试引擎
    Raw_RegisterCmd(LXL_DEBUG_CMD, "LXL " + string(LXL_SCRIPT_LANG_TYPE) + " Engine Real-time Debugging", 4);
    
    //热管理
    Raw_RegisterCmd(LXL_HOT_LIST, "List current loaded LXL plugins", 4);
    Raw_RegisterCmd(LXL_HOT_LOAD, "Load a new LXL plugin", 4);
    Raw_RegisterCmd(LXL_HOT_UNLOAD, "Unload an existing LXL plugin", 4);
    Raw_RegisterCmd(LXL_HOT_RELOAD, "Reload an existing LXL plugin / all LXL plugins", 4);

    INFO("Builtin Cmds Registered.");
}

void ProcessRegCmdQueue()
{
    for (auto& cmdData : toRegCmdQueue)
    {
        Raw_RegisterCmd(cmdData.cmd, cmdData.describe, cmdData.level);
    }
    toRegCmdQueue.clear();
}

bool ProcessDebugEngine(const string& cmd)
{
#define OUTPUT_DEBUG_SIGN() std::cout << "LiteXLoader-" << LXL_SCRIPT_LANG_TYPE << ">" << std::flush
    extern bool globalDebug;
    extern ScriptEngine *debugEngine;

    if (cmd == LXL_DEBUG_CMD)
    {
        if (globalDebug)
        {
            //EndDebug
            INFO("Debug mode ended");
            globalDebug = false;
        }
        else
        {
            //StartDebug
            INFO("Debug mode begin");
            globalDebug = true;
            OUTPUT_DEBUG_SIGN();
        }
        return false;
    }
    if (globalDebug)
    {
        EngineScope enter(debugEngine);
        try
        {
            if (cmd == "stop")
            {
                WARN("Please exit real-time debug mode before 'stop'.");
                //WARN("请先退出Debug实时调试模式再使用stop！");
            }
            else
            {
                auto result = debugEngine->eval(cmd);
                PrintValue(std::cout, result);
                cout << endl;
                OUTPUT_DEBUG_SIGN();
            }
        }
        catch (Exception& e)
        {
            ERRPRINT(e);
            OUTPUT_DEBUG_SIGN();
        }
        return false;
    }
    return true;
}

bool ProcessHotManagement(const std::string& cmd)
{
    auto cmdList = SplitCmdParas(cmd);
    if (cmdList[0] != LXL_HOT_MANAGE_PREFIX)
        return true;

    if (cmd.find(LXL_HOT_LIST) == 0)
    {
        //list
        auto list = LxlListLocalAllPlugins();
        for (auto& name : list)
            PRINT(name);
    }
    else if (cmd.find(LXL_HOT_LOAD) == 0 && cmdList.size() == 3)
    {
        //load
        if (!filesystem::exists(cmdList[2]))
            ERROR("Plugin no found!");
        if(filesystem::path(cmdList[2]).extension() == LXL_PLUGINS_SUFFIX)
            LxlLoadPlugin(cmdList[2],true);
    }
    else if (cmd.find(LXL_HOT_UNLOAD) == 0 && cmdList.size() == 3)
    {
        //unload
        if (filesystem::path(cmdList[2]).extension() == LXL_PLUGINS_SUFFIX)
            if (LxlUnloadPlugin(cmdList[2]) == "")
                ERROR("Plugin no found!");
    }
    else if (cmd.find(LXL_HOT_RELOAD) == 0)
    {
        if (cmdList.size() == 2)
        {
            //reload all
            LxlReloadAllPlugins();
        }
        else if(cmdList.size() == 3)
        {
            //reload one
            if (filesystem::path(cmdList[2]).extension() == LXL_PLUGINS_SUFFIX)
                if(!LxlReloadPlugin(cmdList[2]))
                    ERROR("Plugin no found!");
        }
        else
            ERROR("Bad Command!");
    }
    else
        ERROR("Bad Command!");
    return false;
}

void ProcessStopServer(const string& cmd)
{
    if (cmd == "stop")
    {
        isServerStarted = false;
    }
}

string LxlFindCmdReg(bool isPlayerCmd, const string& cmd, vector<string> &receiveParas)
{
    std::map<std::string, CmdCallbackData, EngineOwnData_MapCmp>& cmdMap =
        isPlayerCmd ? engineLocalData->playerCmdCallbacks : engineLocalData->consoleCmdCallbacks;

    for (auto& cmdData : cmdMap)
    {
        string prefix = cmdData.first;
        if (cmd == prefix || (cmd.find(prefix) == 0 && cmd[prefix.size()] == ' '))
            //如果命令与注册前缀全匹配，或者目标前缀后面为空格
        {
            //Matched
            if (cmd.size() > prefix.size())
            {
                //除了注册前缀之外还有额外参数
                receiveParas = SplitCmdParas(cmd.substr(prefix.size() + 1));
            }
            else
                receiveParas = vector<string>();

            return prefix;
        }
    }
    return string();
}

bool CallPlayerCmdCallback(Player* player, const string& cmdPrefix, const vector<string> &paras)
{
    auto cmdData = engineLocalData->playerCmdCallbacks[cmdPrefix];
    EngineScope enter(cmdData.fromEngine);
    Local<Value> res{};
    try
    {
        Local<Array> args = Array::newArray();
        for (auto& para : paras)
            args.add(String::newString(para));
        res = cmdData.func.get().call({}, PlayerClass::newPlayer(player), args);
    }
    catch (const Exception& e)
    {
        ERROR("PlayerCmd Callback Failed!");
        ERRPRINT("[Error] In Plugin: " + ENGINE_OWN_DATA()->pluginName);
        ERRPRINT(e);
    }
    if (res.isNull() || (res.isBoolean() && res.asBoolean().value() == false))
        return false;

    return true;
}

bool CallServerCmdCallback(const string& cmdPrefix, const vector<string>& paras)
{
    auto cmdData = engineLocalData->consoleCmdCallbacks[cmdPrefix];
    EngineScope enter(cmdData.fromEngine);
    Local<Value> res{};
    try
    {
        Local<Array> args = Array::newArray();
        for (auto& para : paras)
            args.add(String::newString(para));
        res = cmdData.func.get().call({}, args);
    }
    catch (const Exception& e)
    {
        ERROR("ServerCmd Callback Failed!");
        ERRPRINT("[Error] In Plugin: " + ENGINE_OWN_DATA()->pluginName);
        ERRPRINT(e);
    }
    if (res.isNull() || (res.isBoolean() && res.asBoolean().value() == false))
        return false;

    return true;
}

bool CallFormCallback(Player* player, unsigned formId, const string& data)
{
    bool passToBDS = true;

    try
    {
        for (auto engine : lxlModules)
        {
            EngineScope enter(engine);
            FormCallbackData callback;
            try
            {
                callback = ENGINE_GET_DATA(engine)->formCallbacks.at(formId);
            }
            catch (...)
            {
                continue;
            }

            EngineScope scope(callback.engine);
            Local<Value> res{};
            try
            {
                res = callback.func.get().call({}, PlayerClass::newPlayer(player), JsonToValue(data));
            }
            catch (const Exception& e)
            {
                ERROR("Form Callback Failed!");
                ERRPRINT("[Error] In Plugin: " + ENGINE_OWN_DATA()->pluginName);
                ERRPRINT(e);
            }
            if (res.isNull() || (res.isBoolean() && res.asBoolean().value() == false))
                passToBDS = false;

            ENGINE_OWN_DATA()->formCallbacks.erase(formId);
        }
    }
    catch (...)
    {
        ;
    }

    return passToBDS;
}