#include <ScriptX/ScriptX.h>
#include <API/APIHelp.h>
#include <Engine/GlobalShareData.h>
#include <Engine/EngineOwnData.h>
#include <Engine/LoaderHelper.h>
#include <Kernel/Data.h>
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <exception>
#include <filesystem>
#include <memory>
#include <Configs.h>
using namespace script;

//基础库 & 依赖库
std::vector<std::string> depends;

//调试引擎
ScriptEngine *debugEngine;
bool globalDebug = false;

//前置声明
extern std::vector<ScriptEngine*> lxlModules;
extern void BindAPIs(ScriptEngine *engine);

//配置文件
extern INI_ROOT iniConf;

//预加载依赖库
void LoadDepends()
{
    std::filesystem::directory_iterator deps(Raw_IniGetString(iniConf, "Main", "DependsDir", LXL_DEPENDS_DIR));
    for (auto& i : deps) {
        if (i.is_regular_file() && i.path().extension() == LXL_PLUGINS_SUFFIX)
        {
            try
            {
                depends.emplace_back(ReadFileFrom(i.path().string()));
                INFO("Dependence " + i.path().filename().string() + " Loaded.");
            }
            catch (std::exception e)
            {
                WARN("Fail to load dependence " + i.path().filename().string() + ".");
                WARN(e.what());
            }
            catch (...)
            {
                WARN("Fail to load dependence " + i.path().filename().string() + ".");
            }
        }
    }
}

//加载调试引擎
void LoadDebugEngine()
{
    //启动引擎
    debugEngine = NewEngine();
    lxlModules.push_back(debugEngine);
    EngineScope enter(debugEngine);

    //setData
    ENGINE_OWN_DATA()->pluginName = LXL_DEBUG_ENGINE_NAME;

    //绑定API
    try {
        BindAPIs(debugEngine);
    }
    catch (const Exception& e)
    {
        ERROR("Fail in binding Debug Engine!\n");
        throw;
    }

    //加载libs依赖库
    try
    {
        for (auto& i : depends) {
            debugEngine->eval(i);
        }
    }
    catch (const Exception& e)
    {
        ERROR("Fail in Loading Dependence Lib!\n");
        throw;
    }
}

//主加载
void LoadMain()
{
    INFO("Loading plugins...");
    int count = 0;
    std::filesystem::directory_iterator files(Raw_IniGetString(iniConf, "Main", "PluginsDir", LXL_PLUGINS_LOAD_DIR));
    for (auto& i : files) {
        if (i.is_regular_file() && i.path().extension() == LXL_PLUGINS_SUFFIX)
        {
            if (LxlLoadPlugin(i.path().string()))
                ++count;
        }
    }
    INFO(std::to_string(count) + " " + LXL_SCRIPT_LANG_TYPE + " plugins loaded in all.");
}