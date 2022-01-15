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

class DefaultFormatter {
public:
    static util::nstring header() { }
    static util::nstring format(const Record &record) {
        tm t;
        util::localtime_s(&t, &record.getTime().time);
        util::nostringstream ss;
        ss << PLOG_NSTR("[") << t.tm_year + 1900 << "-" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon + 1 << PLOG_NSTR("-") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << PLOG_NSTR("]");
        ss << PLOG_NSTR("[") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << PLOG_NSTR(".") << std::setfill(PLOG_NSTR('0')) << std::setw(3) << record.getTime().millitm << PLOG_NSTR("]");
        ss << PLOG_NSTR("[") << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left << severityToString(record.getSeverity()) << PLOG_NSTR("]");
        ss << PLOG_NSTR("[") << record.getTid() << PLOG_NSTR("]");
        ss << PLOG_NSTR("[Line:") << record.getLine() << PLOG_NSTR("] ");
        ss << record.getMessage() << PLOG_NSTR("\n");
        return ss.str();
    }
};

class SimpleFormatter {
public:
    static util::nstring header() { }
    static util::nstring format(const Record &record) {
        tm t;
        util::localtime_s(&t, &record.getTime().time);
        util::nostringstream ss;
        ss << PLOG_NSTR("[") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << PLOG_NSTR(".") << std::setfill(PLOG_NSTR('0')) << std::setw(3) << record.getTime().millitm << PLOG_NSTR("]");
        ss << PLOG_NSTR("[") << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left << severityToString(record.getSeverity()) << PLOG_NSTR("]");
        ss << GetName(record) << PLOG_NSTR(" ");
        ss << record.getMessage() << PLOG_NSTR("\n");
        return ss.str();
    }

private:
    static util::nstring GetName(const Record &record) {
        util::nostringstream ss;
        ss << record.getFunc();
        util::nstring name{ ss.str() };
        util::nostringstream result;
        if (auto cur{ name.find(PLOG_NSTR("::")) }; cur != util::nstring::npos) {
            if (name.find(PLOG_NSTR("anon"))) {
                result << PLOG_NSTR("[Anonymous@");
            } else {
                result << PLOG_NSTR("[") << record.getObject();
            }
            result << record.getLine() << PLOG_NSTR("]");
        } else {
            result << PLOG_NSTR("[") << name << PLOG_NSTR("]");
        }
        return result.str();
    }
};

} // namespace plog

namespace Utilites {

void ImGuiLogVisualizer::Roll(ImVec4 &&clr, std::string &&msg) {
    mRollingLog.emplace_back(clr, msg);
    mUpdated = true;
}
void ImGuiLogVisualizer::Clear() {
    mRollingLog = {};
}
void ImGuiLogVisualizer::Draw() {
    ImGui::Begin("Logs");
    for (const auto &[clr, msg] : mRollingLog) {
        ImGui::TextColored(clr, "%s", msg.c_str());
    }
    if (mUpdated) {
        ImGui::SetScrollY(ImGui::GetScrollMaxY());
        mUpdated = false;
    }
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

    static plog::ColorConsoleAppender<plog::DefaultFormatter> Console;
    static plog::RollingFileAppender<plog::TxtFormatter> File{ logfile.c_str() };
    static plog::ImGuiAppender<plog::SimpleFormatter> ImGuiWidget{ mVisualizer };

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