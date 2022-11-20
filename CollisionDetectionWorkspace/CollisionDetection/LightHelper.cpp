#include "LightHelper.h"

const float LightHelper::DEFAULT_ATTEN_CONST = 0.1f;
const float LightHelper::DEFAULT_ATTEN_LINEAR = 0.1f;
const float LightHelper::DEFAULT_ATTEN_QUADRATIC = 0.1f;
const float LightHelper::DEFAULT_AMBIENT_TO_DIFFUSE_RATIO = 0.8f;
const float LightHelper::DEFAULT_INFINITE_DISTANCE = 10000.0f;
const float LightHelper::DEFAULT_DIFFUSE_ACCURACY_THRESHOLD = 0.001f;
const float LightHelper::DEFAULT_ZERO_THRESHOLD = 0.0001f;

LightHelper::LightHelper()
{
}

LightHelper::~LightHelper()
{
}

float LightHelper::CalcApproxDistFromAtten(float targetLightLevel)
{
	return this->CalcApproxDistFromAtten(targetLightLevel, DEFAULT_DIFFUSE_ACCURACY_THRESHOLD);
}

float LightHelper::CalcApproxDistFromAtten(float targetLightLevel, float accuracy)
{
	return this->CalcApproxDistFromAtten(targetLightLevel, accuracy,
		LightHelper::DEFAULT_ATTEN_CONST,
		LightHelper::DEFAULT_ATTEN_LINEAR,
		LightHelper::DEFAULT_ATTEN_QUADRATIC,
		LightHelper::DEFAULT_INFINITE_DISTANCE, DEFAULT_MAX_ITERATIONS);
}

float LightHelper::CalcApproxDistFromAtten(
	float targetLightLevel, 
	float accuracy, 
	float infiniteDistance, 
	float constAttenuation, 
	float linearAttenuation, 
	float quadraticAttenuation, 
	unsigned int maxIterations)
{
	if (targetLightLevel != 0.0f)
	{
		if ((accuracy * 10.0f) >= targetLightLevel * 10.0f)
		{
			accuracy = targetLightLevel / 10.0f;
		}
	}	

	float targetLightLevelLow = targetLightLevel - accuracy;
	float targetLightLevelHigh = targetLightLevel + accuracy;

	if (this->CalcDiffuseFromAttenByDistance(DEFAULT_INFINITE_DISTANCE,
		constAttenuation, linearAttenuation, quadraticAttenuation,
		accuracy) > targetLightLevelHigh)
	{	
		return DEFAULT_INFINITE_DISTANCE;
	}

	float distanceGuessLow = 0.0f;
	float distanceGuessHigh = DEFAULT_INFINITE_DISTANCE;

	unsigned int iterationCount = 0;
	while (iterationCount < maxIterations)
	{
		float curDistanceGuess = ((distanceGuessHigh - distanceGuessLow) / 2.0f) + distanceGuessLow;
		// 
		float curDiffuseAtGuessDistance
			= this->CalcDiffuseFromAttenByDistance(curDistanceGuess, constAttenuation,
				linearAttenuation, quadraticAttenuation, DEFAULT_ZERO_THRESHOLD);

		if (curDiffuseAtGuessDistance < targetLightLevelLow)
		{
			distanceGuessHigh = curDistanceGuess;
		}
		else if (curDiffuseAtGuessDistance > targetLightLevelHigh)
		{	
			distanceGuessLow = curDistanceGuess;
		}
		else
		{
			return curDistanceGuess;
		}

		iterationCount++;

	}

	float distance = (distanceGuessHigh - distanceGuessLow) / 2.0f;

	return distance;
}

float LightHelper::CalcDiffuseFromAttenByDistance(
	float distance, 
	float constAttenuation,
	float linearAttenuation, 
	float quadraticAttenuation,
	float zeroThreshold)
{
	float diffuse = 1.0f;

	float denominator = constAttenuation +
		linearAttenuation * distance +
		quadraticAttenuation * distance * distance;

	if (denominator <= zeroThreshold)
	{
		diffuse = 1.0f;
	}
	else
	{
		float atten = 1.0f / denominator;
		diffuse *= atten;
		if (diffuse > 1.0f)
		{
			diffuse = 1.0f;
		}
	}

	return diffuse;
}
