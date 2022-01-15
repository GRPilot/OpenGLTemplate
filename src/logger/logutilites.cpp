#include <incs.hpp>
#include <locale>
#include <codecvt>

#include <filesystem>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Record.h>

#include "logutilites.hpp"

namespace fs = std::filesystem;

namespace plog {

template<class Formatter>
class ImGuiAppender : public IAppender {
public:
    explicit ImGuiAppender(Utilites::ImGuiLogVisualizer::Ref visualizer)
        : mVisualizer{ visualizer }
    {}

    void write(const Record &record) {
        auto visualizer{ mVisualizer.lock() };
        if (nullptr == visualizer) {
            mCache.emplace(
                GetSeverityColor(record.getSeverity()),
                convert(Formatter::format(record))
            );
            return;
        }
        while (!mCache.empty()) {
            auto [clr, msg] { mCache.front() };
            mCache.pop();
            visualizer->Roll(
                std::move(clr),
                std::move(msg)
            );
        }
        visualizer->Roll(
            GetSeverityColor(record.getSeverity()),
            convert(Formatter::format(record))
        );
    }

private:
    Utilites::ImGuiLogVisualizer::WeakRef mVisualizer;
    std::queue<Utilites::ImGuiLogVisualizer::Log> mCache;

    ImVec4 GetSeverityColor(Severity severity) {
        switch (severity) {
            case fatal: return ImVec4{ 0.9, 0.0, 0.1, 1.0 };
            case error: return ImVec4{ 0.7, 0.2, 0.3, 1.0 };
            case warning: return ImVec4{ 0.9, 0.5, 0.1, 1.0 };
            case info: return ImVec4{ 0.1, 0.7, 0.3, 1.0 };
            default: return ImVec4{ 0.9, 0.9, 0.9, 1.0};
        }
        return {};
    }
    std::string convert(const util::nstring &str) const {
        using codec = std::codecvt_utf8<util::nchar>;
        std::wstring_convert<codec, util::nchar> conv;
        return conv.to_bytes(str);
    }
};

} // namespace plog

namespace Utilites {

void ImGuiLogVisualizer::Roll(ImVec4 &&clr, std::string &&msg) {
    mRollingLog.emplace(clr, msg);
}
void ImGuiLogVisualizer::Clear() {
    mRollingLog = {};
}
void ImGuiLogVisualizer::Draw() {
    if (mRollingLog.empty()) {
        return;
    }
    ImGui::Begin("Logs");
    while (!mRollingLog.empty()) {
        auto [clr, msg] { mRollingLog.front() };
        mRollingLog.pop();
        ImGui::TextColored(clr, "%s", msg.c_str());
    }
    ImGui::SetScrollY(ImGui::GetScrollMaxY());
    ImGui::End();

}

LogHelper *LogHelper::Instance() {
    static LogHelper helper;
    return &helper;
}

void LogHelper::Initialize(plog::Severity level) {
    if (!fs::exists(sLoggerDirectory)) {
        fs::create_directories(sLoggerDirectory);
    }

    std::string logfile{ sLoggerDirectory + GenerateLogFileName() };

    mVisualizer = std::make_shared<ImGuiLogVisualizer>();

    static plog::ColorConsoleAppender<plog::TxtFormatter> Console;
    static plog::RollingFileAppender<plog::TxtFormatter> File{ logfile.c_str() };
    static plog::ImGuiAppender<plog::TxtFormatter> ImGuiWidget{ mVisualizer };

    plog::init(level, logfile.c_str())
        .addAppender(&Console)
        .addAppender(&File)
        .addAppender(&ImGuiWidget);
}

ImGuiLogVisualizer::Ref LogHelper::Visualizer() const {
    return mVisualizer;
}

std::string LogHelper::GenerateLogFileName() const {
    time_t now{ time(nullptr) };
    std::stringstream out;
    out << std::put_time(localtime(&now), "%y%m%d%H");
    return out.str();
}

} // namespace Utilites