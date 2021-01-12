#pragma once
#include <cmath>
#include <compare>
#include <vector>
#include <algorithm>

class SampledSpectrum;
class RGBSpectrum;

template<int NumSpectrumSamples>
class CoefficientSpectrum
{
public:
	CoefficientSpectrum(float v = 0.0f)
	{
		for (int i = 0; i < NumSpectrumSamples; ++i)
		{
			c[i] = v;
		}
	}

	CoefficientSpectrum(const CoefficientSpectrum& s2)
	{
		for (int i = 0; i < NumSpectrumSamples; ++i)
		{
			c[i] = s2.c[i];
		}
	}

	auto operator<=>(const CoefficientSpectrum&) const = default;

	float operator[](int i) const
	{
		return c[i];
	}

	float& operator[](int i)
	{
		return c[i];
	}

	CoefficientSpectrum operator-() const
	{
		CoefficientSpectrum ret;
		for (int i = 0; i < NumSpectrumSamples; ++i)
		{
			ret.c[i] = -c[i];
		}
		return ret;
	}

	CoefficientSpectrum& operator+=(const CoefficientSpectrum& s2)
	{
		for (int i = 0; i < NumSpectrumSamples; ++i)
		{
			c[i] += s2.c[i];
		}
		return *this;
	}

	CoefficientSpectrum operator+(const CoefficientSpectrum& s2) const
	{
		CoefficientSpectrum ret = *this;
		for (int i = 0; i < NumSpectrumSamples; ++i)
		{
			ret.c[i] += s2.c[i];
		}
		return ret;
	}

	CoefficientSpectrum& operator-=(const CoefficientSpectrum& s2)
	{
		for (int i = 0; i < NumSpectrumSamples; ++i)
		{
			c[i] -= s2.c[i];
		}
		return *this;
	}

	CoefficientSpectrum operator-(const CoefficientSpectrum& s2) const
	{
		CoefficientSpectrum ret = *this;
		for (int i = 0; i < NumSpectrumSamples; ++i)
		{
			ret.c[i] -= s2.c[i];
		}
		return ret;
	}

	CoefficientSpectrum& operator*=(const CoefficientSpectrum& s2)
	{
		for (int i = 0; i < NumSpectrumSamples; ++i)
		{
			c[i] *= s2.c[i];
		}
		return *this;
	}

	CoefficientSpectrum operator*(const CoefficientSpectrum& s2) const
	{
		CoefficientSpectrum ret = *this;
		for (int i = 0; i < NumSpectrumSamples; ++i)
		{
			ret.c[i] *= s2.c[i];
		}
		return ret;
	}

	CoefficientSpectrum& operator/=(const CoefficientSpectrum& s2)
	{
		for (int i = 0; i < NumSpectrumSamples; ++i)
		{
			c[i] /= s2.c[i];
		}
		return *this;
	}

	CoefficientSpectrum operator/(const CoefficientSpectrum& s2) const
	{
		CoefficientSpectrum ret = *this;
		for (int i = 0; i < NumSpectrumSamples; ++i)
		{
			ret.c[i] /= s2.c[i];
		}
		return ret;
	}

	CoefficientSpectrum Clamp(float low = 0, float high = INFINITY) const
	{
		CoefficientSpectrum ret;
		for (int i = 0; i < NumSpectrumSamples; ++i)
		{
			ret.c[i] = std::clamp(c[i], low, high);
		}
		return ret;
	}

	bool HasNans() const
	{
		for (const auto& l : c)
		{
			if (std::isnan(l) != 0.0f)
			{
				return true;
			}
		}
		return false;
	}

	bool IsBlack() const
	{
		for (const auto& l : c)
		{
			if (l != 0.0f)
				return false;
		}
		return true;
	}
protected:
	float c[NumSpectrumSamples];
};

template<int NumSpectrumSamples>
inline CoefficientSpectrum<NumSpectrumSamples> operator*(float a, const CoefficientSpectrum<NumSpectrumSamples>& s)
{
	return s * a;
}

// Sampled Spectrum
static constexpr int SampledLambdaStart = 400;
static constexpr int SampledLambdaEnd = 700;
static constexpr int NumSpectralSamples = 60;

static constexpr int NumCIESamples = 471;
extern const float CIE_X[NumCIESamples];
extern const float CIE_Y[NumCIESamples];
extern const float CIE_Z[NumCIESamples];
extern const float CIE_lambda[NumCIESamples];
static constexpr float CIE_Y_integral = 106.856895f;
static constexpr int nRGB2SpectSamples = 32;
extern const float RGB2SpectLambda[nRGB2SpectSamples];
extern const float RGBRefl2SpectWhite[nRGB2SpectSamples];
extern const float RGBRefl2SpectCyan[nRGB2SpectSamples];
extern const float RGBRefl2SpectMagenta[nRGB2SpectSamples];
extern const float RGBRefl2SpectYellow[nRGB2SpectSamples];
extern const float RGBRefl2SpectRed[nRGB2SpectSamples];
extern const float RGBRefl2SpectGreen[nRGB2SpectSamples];
extern const float RGBRefl2SpectBlue[nRGB2SpectSamples];

extern const float RGBIllum2SpectWhite[nRGB2SpectSamples];
extern const float RGBIllum2SpectCyan[nRGB2SpectSamples];
extern const float RGBIllum2SpectMagenta[nRGB2SpectSamples];
extern const float RGBIllum2SpectYellow[nRGB2SpectSamples];
extern const float RGBIllum2SpectRed[nRGB2SpectSamples];
extern const float RGBIllum2SpectGreen[nRGB2SpectSamples];
extern const float RGBIllum2SpectBlue[nRGB2SpectSamples];

enum class SpectrumType
{
	Reflectance,
	Illuminant
};

bool IsSpectrumSamplesSorted(const float* lambda, const float* vals, int n);
void SortSpectrumSamples(float* lambda, float* vals, int n);

float AverageSpectrumSamples(const float* lambda, const float* vals, int n, float lambdaStart, float lambdaEnd);
float InterpolateSpectrumSamples(const float* lambda, const float* vals, int n, float l);

void XYZToRGB(const float xyz[3], float rgb[3]);
void RGBToXYZ(const float rgb[3], float xyz[3]);

class SampledSpectrum : public CoefficientSpectrum<NumSpectralSamples>
{
public:
	SampledSpectrum(float v = 0.0f);
	SampledSpectrum(const CoefficientSpectrum<NumSpectralSamples>& v);
	SampledSpectrum(const RGBSpectrum& s, SpectrumType Type = SpectrumType::Reflectance);

	static SampledSpectrum FromSampled(const float* lambda, const float* v, int n);
	static SampledSpectrum FromRGB(const float rgb[3], SpectrumType Type = SpectrumType::Illuminant);
	static SampledSpectrum FromXYZ(const float xyz[3], SpectrumType Type = SpectrumType::Reflectance);

	float y() const;
	void ToXYZ(float xyz[3]) const;
	void ToRGB(float rgb[3]) const;
	RGBSpectrum ToRGBSpectrum() const;
};

class RGBSpectrum : public CoefficientSpectrum<3>
{
public:
	RGBSpectrum(float v = 0.0f);
	RGBSpectrum(float r, float g, float b);
	RGBSpectrum(const CoefficientSpectrum<3>& v);
	RGBSpectrum(const RGBSpectrum& s, SpectrumType Type = SpectrumType::Reflectance);

	static RGBSpectrum FromSampled(const float* lambda, const float* v, int n);
	static RGBSpectrum FromRGB(const float rgb[3], SpectrumType Type = SpectrumType::Reflectance);
	static RGBSpectrum FromXYZ(const float xyz[3], SpectrumType Type = SpectrumType::Reflectance);

	float y() const;
	void ToXYZ(float xyz[3]) const;
	void ToRGB(float rgb[3]) const;
	RGBSpectrum ToRGBSpectrum() const;
};

inline static SampledSpectrum X, Y, Z;
inline static SampledSpectrum rgbRefl2SpectWhite, rgbRefl2SpectCyan;
inline static SampledSpectrum rgbRefl2SpectMagenta, rgbRefl2SpectYellow;
inline static SampledSpectrum rgbRefl2SpectRed, rgbRefl2SpectGreen;
inline static SampledSpectrum rgbRefl2SpectBlue;
inline static SampledSpectrum rgbIllum2SpectWhite, rgbIllum2SpectCyan;
inline static SampledSpectrum rgbIllum2SpectMagenta, rgbIllum2SpectYellow;
inline static SampledSpectrum rgbIllum2SpectRed, rgbIllum2SpectGreen;
inline static SampledSpectrum rgbIllum2SpectBlue;

void InitSampledSpectrums();