cmake_minimum_required(VERSION 3.9)

project(breps)

include(FeatureSummary)

add_subdirectory(vendor)
add_subdirectory(cpp)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
