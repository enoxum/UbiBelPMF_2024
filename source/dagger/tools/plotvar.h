#pragma once

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#ifdef MAX
#undef MAX
#endif
#define MAX max

#ifdef MIN
#undef MIN
#endif
#define MIN min

#include <limits>

namespace ImGui
{
	void PlotVar(
		const char* label_, 
		float value_, 
		float scaleMin_ = std::numeric_limits<float>::min(), 
		float scaleMax_ = std::numeric_limits<float>::max(), 
		size_t bufferSize_ = 120);

	void PlotVarFlushOldEntries();
}