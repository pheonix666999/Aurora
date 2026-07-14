include(FetchContent)

# Exact JUCE 8.0.14 tag commit. Do not replace this with a moving branch.
set(AURORA_JUCE_VERSION "8.0.14" CACHE STRING "Pinned JUCE version")
set(AURORA_JUCE_COMMIT "2cdfca8feb300fb424002ba2c2751569e5bacb64" CACHE STRING "Pinned JUCE commit")
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG "${AURORA_JUCE_COMMIT}"
    GIT_SHALLOW FALSE
    GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(JUCE)

