#pragma once

#include "vec.h"

namespace engine {

	struct Material {
		vec4 diffuse;
		vec4 ambient;
		vec4 specular;
		vec4 emissive;
		float shininess;
		int texCount;
	};

}