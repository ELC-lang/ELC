//
// pch.h
//

#pragma once

#include <gtest/gtest.h>
#pragma comment(lib, "Shlwapi.lib")
#include <benchmark/benchmark.h>


#include <string>
#include <functional>

#define ELC_TEST_COUNT_MEMORY_ALLOC
#include "../../files/elc/string"
