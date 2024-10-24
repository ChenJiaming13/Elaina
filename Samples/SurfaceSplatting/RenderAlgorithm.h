#pragma once

#include <memory>
#include <vector>
#include <string>
#include "types.h"

namespace Elaina
{
	class CCamera;
}

class CRenderAlgorithm
{
public:
	virtual ~CRenderAlgorithm() = default;

	[[nodiscard]] virtual std::string getName() const = 0;

	virtual void initV(int vWidth, int vHeight, const std::vector<SSurfel>& vSurfels) = 0;

	virtual void onWindowSizeChangeV(int vWidth, int vHeight) = 0;

	virtual void renderV(const std::shared_ptr<Elaina::CCamera>& vCamera) = 0;

	virtual void cleanup() = 0;
};

