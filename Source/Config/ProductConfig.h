#pragma once

#include <juce_graphics/juce_graphics.h>

namespace aurora::config
{
inline constexpr auto productName = "Aurora Broadcast Processor";
inline constexpr auto shortProductName = "Aurora BP";
inline constexpr auto companyName = "Ammar Audio Labs";
inline constexpr auto manufacturerCode = "AALB";
inline constexpr auto pluginCode = "AUBP";
inline constexpr auto bundleIdentifier = "com.ammaraudio.aurorabroadcastprocessor";
inline constexpr auto version = AURORA_VERSION_STRING;
inline constexpr auto juceVersion = AURORA_JUCE_VERSION_STRING;
inline constexpr auto website = "https://example.invalid/aurora-bp";
inline constexpr auto supportEmail = "support@example.invalid";
inline constexpr auto copyright = "Copyright (c) 2026 Ammar Audio Labs";
inline constexpr int defaultWidth = 1180;
inline constexpr int defaultHeight = 760;
inline constexpr int minimumWidth = 960;
inline constexpr int minimumHeight = 620;
inline constexpr int maximumWidth = 1600;
inline constexpr int maximumHeight = 1000;
}

