function(aurora_apply_options target)
    target_compile_features(${target} PRIVATE cxx_std_20)
    target_compile_definitions(${target} PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        AURORA_VERSION_STRING="${PROJECT_VERSION}"
        AURORA_JUCE_VERSION_STRING="${AURORA_JUCE_VERSION}")
    if(MSVC)
        target_compile_options(${target} PRIVATE $<$<CONFIG:Release>:/O2>)
    endif()
endfunction()

