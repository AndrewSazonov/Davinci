// Catch: A modern, C++-native, header-only, framework for unit-tests, TDD and BDD
// https://github.com/philsquared/Catch
// https://github.com/philsquared/Catch/blob/master/docs/assertions.md
// https://github.com/catchorg/Catch2/blob/master/docs/own-main.md#top
// All tests are described in the separate files

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_RUNNER // Lets Catch take full control of args and config

#include "Functions.hpp"
#include "Macros.hpp"

#include "catch.hpp"

int main( int argc, char* argv[] ) {
    As::SetDebugOutputFormat(IS_DEBUG_OR_PROFILE);

    int result = Catch::Session().run( argc, argv );

    return result; }

