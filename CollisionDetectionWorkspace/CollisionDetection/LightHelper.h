#pragma once

class LightHelper
{
public:
	LightHelper();
	~LightHelper();

	static const float DEFAULT_ATTEN_CONST;
	static const float DEFAULT_ATTEN_LINEAR;
	static const float DEFAULT_ATTEN_QUADRATIC;
	static const float DEFAULT_AMBIENT_TO_DIFFUSE_RATIO;
	static const unsigned int DEFAULT_MAX_ITERATIONS = 50;
	static const float DEFAULT_INFINITE_DISTANCE;	
	static const float DEFAULT_DIFFUSE_ACCURACY_THRESHOLD;
	static const float DEFAULT_ZERO_THRESHOLD;

	float CalcApproxDistFromAtten(float targetLightLevel);
	float CalcApproxDistFromAtten(float targetLightLevel, float accuracy);

	float CalcApproxDistFromAtten(float targetLightLevel, float accuracy, float infiniteDistance,
		float constAttenuation, float linearAttenuation, float quadraticAttenuation,
		unsigned int maxIterations = DEFAULT_MAX_ITERATIONS);
	static const float DEFAULT_ZERO_THRESHOLD;
	float CalcDiffuseFromAttenByDistance(float distance,
		float constAttenuation,
		float linearAttenuation,
		float quadraticAttenuation,
		float zeroThreshold = DEFAULT_ZERO_THRESHOLD);
};
