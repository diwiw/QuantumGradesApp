#include "cli/AppCLI.hpp"

int main(int argc, char** argv)
{
    qga::cli::AppCLI app;
    return app.run(argc, argv);
}
