#pragma once

#include <queue>
#include <plog/Log.h>
#include <plog/Severity.h>

namespace Utilites {

class ImGuiLogVisualizer {
public:
    using Ref = std::shared_ptr<ImGuiLogVisualizer>;
    using WeakRef = std::weak_ptr<ImGuiLogVisualizer>;
    using Log = std::pair<ImVec4, std::string>;

    void Roll(ImVec4 &&clr, std::string &&msg);
    void Clear();
    void Draw();

private:
    std::queue<Log> mRollingLog;
};

class LogHelper : public plog::util::Singleton<LogHelper> {
public:
    static LogHelper *Instance();

    void Initialize(plog::Severity level);
    ImGuiLogVisualizer::Ref Visualizer() const;

private:
    ImGuiLogVisualizer::Ref mVisualizer;
    const std::string sLoggerDirectory{ "logs/" };

    std::string GenerateLogFileName() const;
};

} // namespace Utilites
