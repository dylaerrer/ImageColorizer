//
// Created by Tymoteusz Pilarz on 10/06/2023.
//

#include <memory>

#include "ImageColorizer.h"

int main(int argc, char **argv)
{
    return Wt::WRun(argc, argv, [](const Wt::WEnvironment& env)
    {
        return std::make_unique<ImageColorizer>(env);
    });
}
