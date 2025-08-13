# Helper to fetch JUCE from the local copy
# Assumes JUCE/ directory exists in the project root

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/JUCE" "${CMAKE_BINARY_DIR}/JUCE")
