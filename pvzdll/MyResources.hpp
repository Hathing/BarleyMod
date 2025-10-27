#pragma once
#include "MyClasses.hpp"

typedef int MyImage;
MyImage ImportImage(const char* file_path, const PVZ::PVZApp& app);
void ImportNewResources(PVZ::PVZApp& app);

extern MyImage IMAGE_TEST;