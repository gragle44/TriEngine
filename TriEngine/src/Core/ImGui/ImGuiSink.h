#pragma once

#include "imgui.h"

#include "spdlog/sinks/base_sink.h"
#include "spdlog/details/null_mutex.h"
#include <mutex>

namespace TriEngine {

    struct ImGuiLogItem {
        std::string LoggerName;
        std::string Message;
        spdlog::level::level_enum Level;
    };

    template<typename Mutex>
    class ImGuiSink : public spdlog::sinks::base_sink<Mutex>
    {
    static constexpr ImVec4 black = ImVec4(0.000f, 0.000f, 0.000f, 1.00f);
    static constexpr ImVec4 red = ImVec4(0.502f, 0.000f, 0.000f, 1.00f);
    static constexpr ImVec4 green = ImVec4(0.000f, 0.502f, 0.000f, 1.00f);
    static constexpr ImVec4 yellow = ImVec4(0.502f, 0.502f, 0.000f, 1.00f);
    static constexpr ImVec4 blue = ImVec4(0.000f, 0.000f, 0.502f, 1.00f);
    static constexpr ImVec4 magenta = ImVec4(0.502f, 0.000f, 0.502f, 1.00f);
    static constexpr ImVec4 cyan = ImVec4(0.000f, 0.502f, 0.502f, 1.00f);
    static constexpr ImVec4 white = ImVec4(0.753f, 0.753f, 0.753f, 1.00f);

    public:
        void OnImGuiRender(uint32_t maxShownMessages = 200) {
            if (ImGui::Begin("Log")) {
                for (uint32_t i = m_Messages.size() > maxShownMessages ? m_Messages.size() - maxShownMessages : 0; i < m_Messages.size(); i++) {
                    const auto& msg = m_Messages[i];

                    switch (msg.Level) {
                    case spdlog::level::debug:
                        ImGui::TextColored(magenta, "[CRITICAL] %s: %s", msg.LoggerName.c_str(), msg.Message.c_str());
                        break;
                    case spdlog::level::trace:
                        ImGui::TextColored(white, "[TRACE] %s: %s", msg.LoggerName.c_str(), msg.Message.c_str());
                        break;
                    case spdlog::level::warn:
                        ImGui::TextColored(yellow, "[WARN] %s: %s", msg.LoggerName.c_str(), msg.Message.c_str());
                        break;
                    case spdlog::level::err:
                        ImGui::TextColored(red, "[ERROR] %s: %s", msg.LoggerName.c_str(), msg.Message.c_str());
                        break;
                    case spdlog::level::critical:
                        ImGui::TextColored(red, "[CRITICAL] %s: %s", msg.LoggerName.c_str(), msg.Message.c_str());
                        break;
                    case spdlog::level::info:
                        ImGui::TextColored(green, "[INFO] %s: %s", msg.LoggerName.c_str(), msg.Message.c_str());
                        break;
                    default:
                        TRI_CORE_ASSERT(false, "Unknown logging level");
                        break;
                    }
                }
                if (m_NewMessage) {
                    ImGui::SetScrollHereY();
                    m_NewMessage = false;
                }
            }

            ImGui::End();

        }
    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            ImGuiLogItem item;
            item.LoggerName = std::string(msg.logger_name.data(), msg.logger_name.size());
            item.Message = std::string(msg.payload.data(), msg.payload.size());
            item.Level = msg.level;
            m_Messages.push_back(item);

            m_NewMessage = true;
        }

        void flush_() override {}

    protected:
        std::vector<ImGuiLogItem> m_Messages;

        bool m_NewMessage = false;

    };

    using imgui_sink_mt = ImGuiSink<std::mutex>;
    using imgui_sink_st = ImGuiSink<spdlog::details::null_mutex>;
}
