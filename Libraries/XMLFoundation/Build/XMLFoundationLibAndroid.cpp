#include "XMLFoundationLibAndroid.h"

// in vs2022 the include directories contains   "$(SolutionDir)Libraries\XMLFoundation\src"     to find the following #include
#include "XMLFoundation.cpp" // notice we are including a .cpp as opposed to the typical .h
// also the build path must be able to find the .h files so add      $(SolutionDir)Libraries\XMLFoundation\inc

extern "C" {

	void XMLFoundationLib()
	{
	}

	XMLFoundationLib::XMLFoundationLib()
	{
	}

	XMLFoundationLib::~XMLFoundationLib()
	{
	}
}
