#include "init_spdlog.h"

#include <cstdlib>

#include <spdlog/spdlog.h>

int main(int agrc, char** argv)
{
    if (!init_spdlog()) {
        spdlog::error("Failed to init rotating spdlog.");
        return EXIT_FAILURE;
    }

    spdlog::debug("Debug msg.");

    return EXIT_SUCCESS;
}
