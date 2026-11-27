/*
    ____                  __   ______ ___  
   / __ \                 \ \ / /___ \__ \ 
  | |  | |_ __   ___ _ __  \ V /  __) | ) |
  | |  | | '_ \ / _ \ '_ \  > <  |__ < / / 
  | |__| | |_) |  __/ | | |/ . \ ___) / /_ 
   \____/| .__/ \___|_| |_/_/ \_\____/____|
         | |                               
         |_|                               
  
  OpenX32 - The OpenSource Operating System for the Behringer X32 Audio Mixing Console
  Copyright 2025 OpenMixerProject
  https://github.com/OpenMixerProject/OpenX32
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 3 as published by the Free Software Foundation.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
*/

#include "fxmath.h"

#include <complex>
#include "../lib_ext/doctest/doctest/doctest.h"

namespace OMC
{

FxMath::FxMath(X32BaseParameter* basepar) : X32Base(basepar){ }

void FxMath::RecalcFilterCoefficients_PEQ(sPEQ* peq) {
  // Online-Calculator: https://www.earlevel.com/main/2021/09/02/biquad-calculator-v3
  // Source: https://www.earlevel.com/main/2012/11/26/biquad-c-source-code
  // Alternative Source: https://gcradix.de/an-introduction-to-biquad-filters (Caution: calculation of Notch-Coefficients is wrong)

  double V = pow(10.0, fabs(peq->gain)/20.0);
  double K = tan(PI * peq->fc / (float)config->GetUint(MP_ID::SAMPLERATE));
  double K2 = K * K;
  double norm;

  switch (peq->type) {
    case 0: // allpass
      norm = 1.0 / (1.0 + K * 1.0/peq->Q + K2);
      peq->a[0] = (1.0 - K * 1.0/peq->Q + K2) * norm;
      peq->a[1] = 2.0 * (K2 - 1.0) * norm;
      peq->a[2] = 1.0;
      peq->b[1] = peq->a[1];
      peq->b[2] = peq->a[0];
      break;
    case 1: // peak-filter
      if (peq->gain >= 0) {
        norm = 1.0 / (1.0 + 1.0/peq->Q * K + K2);
        peq->a[0] = (1.0 + V/peq->Q * K + K2) * norm;
        peq->a[1] = 2.0 * (K2 - 1.0) * norm;
        peq->a[2] = (1.0 - V/peq->Q * K + K2) * norm;
        peq->b[1] = peq->a[1];
        peq->b[2] = (1.0 - 1.0/peq->Q * K + K2) * norm;
      }else{
        norm = 1.0 / (1.0 + V/peq->Q * K + K2);
        peq->a[0] = (1.0 + 1.0/peq->Q * K + K2) * norm;
        peq->a[1] = 2.0 * (K2 - 1.0) * norm;
        peq->a[2] = (1.0 - 1.0/peq->Q * K + K2) * norm;
        peq->b[1] = peq->a[1];
        peq->b[2] = (1.0 - V/peq->Q * K + K2) * norm;
      }
      break;
    case 2: // low-shelf
      if (peq->gain >= 0) {    // boost
        norm = 1.0 / (1.0 + sqrt(2.0) * K + K2);
        peq->a[0] = (1.0 + sqrt(2.0*V) * K + V * K2) * norm;
        peq->a[1] = 2.0 * (V * K2 - 1.0) * norm;
        peq->a[2] = (1.0 - sqrt(2.0*V) * K + V * K2) * norm;
        peq->b[1] = 2.0 * (K2 - 1.0) * norm;
        peq->b[2] = (1.0 - sqrt(2.0) * K + K2) * norm;
      }
      else {    // cut
        norm = 1.0 / (1.0 + sqrt(2.0*V) * K + V * K2);
        peq->a[0] = (1.0 + sqrt(2.0) * K + K2) * norm;
        peq->a[1] = 2.0 * (K2 - 1.0) * norm;
        peq->a[2] = (1.0 - sqrt(2.0) * K + K2) * norm;
        peq->b[1] = 2.0 * (V * K2 - 1.0) * norm;
        peq->b[2] = (1.0 - sqrt(2.0*V) * K + V * K2) * norm;
      }
      break;
    case 3: // high-shelf
        if (peq->gain >= 0) {    // boost
          norm = 1.0 / (1.0 + sqrt(2.0) * K + K2);
          peq->a[0] = (V + sqrt(2.0*V) * K + K2) * norm;
          peq->a[1] = 2.0 * (K2 - V) * norm;
          peq->a[2] = (V - sqrt(2.0*V) * K + K2) * norm;
          peq->b[1] = 2.0 * (K2 - 1.0) * norm;
          peq->b[2] = (1.0 - sqrt(2.0) * K + K2) * norm;
        }
        else {    // cut
          norm = 1.0 / (V + sqrt(2.0*V) * K + K2);
          peq->a[0] = (1.0 + sqrt(2.0) * K + K2) * norm;
          peq->a[1] = 2.0 * (K2 - 1.0) * norm;
          peq->a[2] = (1.0 - sqrt(2.0) * K + K2) * norm;
          peq->b[1] = 2.0 * (K2 - V) * norm;
          peq->b[2] = (V - sqrt(2.0*V) * K + K2) * norm;
        }
      break;
    case 4: // bandpass
        norm = 1.0 / (1.0 + K / peq->Q + K2);
        peq->a[0] = (K / peq->Q) * norm;
        peq->a[1] = 0;
        peq->a[2] = -peq->a[0];
        peq->b[1] = 2.0 * (K2 - 1.0) * norm;
        peq->b[2] = (1.0 - K / peq->Q + K2) * norm;
      break;
    case 5: // notch
        norm = 1.0 / (1.0 + K / peq->Q + K2);
        peq->a[0] = (1.0 + K2) * norm;
        peq->a[1] = 2.0 * (K2 - 1.0) * norm;
        peq->a[2] = peq->a[0];
        peq->b[1] = peq->a[1];
        peq->b[2] = (1.0 - K / peq->Q + K2) * norm;
      break;
    case 6: // lowpass
        norm = 1.0 / (1.0 + K / peq->Q + K2);
        peq->a[0] = K2 * norm;
        peq->a[1] = 2.0 * peq->a[0];
        peq->a[2] = peq->a[0];
        peq->b[1] = 2.0 * (K2 - 1.0) * norm;
        peq->b[2] = (1.0 - K / peq->Q + K2) * norm;
      break;
    case 7: // highpass
        norm = 1.0 / (1.0 + K / peq->Q + K2);
        peq->a[0] = 1.0 * norm;
        peq->a[1] = -2.0 * peq->a[0];
        peq->a[2] = peq->a[0];
        peq->b[1] = 2.0 * (K2 - 1.0) * norm;
        peq->b[2] = (1.0 - K / peq->Q + K2) * norm;
      break;
  }
}

float FxMath::CalcFrequencyResponse_LC(float f, float fc, float fs) {
  float numerator = f / fc;
  return 10.0f * log10( numerator / sqrt(1 + numerator * numerator) );
}

float FxMath::CalcFrequencyResponse_HC(float f, float fc, float fs) {
  return 10.0f * log10( 1.0f/sqrt( 1.0f + pow(f/fc, 2.0f) ) );
}

float FxMath::CalcFrequencyResponse_PEQ(float a0, float a1, float a2,  float b1, float b2, float f, float fs) {
  float omega = (2.0f * PI * f) / fs;
  float phi = (1.0f - cosf(omega)) * 0.5f;
  float p_phi = phi * phi;

  float num_sum = a0 + a1 + a2;
  float num_term2 = a0*a1 + 4.0f*a0*a2 + a1*a2;
  float num_term3 = a0*a2;
  float numerator = (num_sum * num_sum) - (4.0f * num_term2 * phi) + (16.0f * num_term3 * p_phi);

  float b0 = 1.0f;
  float den_sum = b0 + b1 + b2;
  float den_term2 = b0*b1 + 4.0f*b0*b2 + b1*b2;
  float den_term3 = b0*b2;
  float denominator = (den_sum * den_sum) - (4.0f * den_term2 * phi) + (16.0f * den_term3 * p_phi);

  return 10.0f * log10f(numerator / denominator);

/*
  // this is the condensed form of the above code, but not optimized for speed
	float omega = (2.0f * PI * f)/fs;
	float phi = pow(sin(omega / 2.0f), 2.0f);
	float b0 = 1.0f;
	
	return ( 10.0f * log10( pow(a0+a1+a2,2.0f) - 4.0f*(a0*a1 + 4.0f*a0*a2 + a1*a2) * phi + 16.0f*a0*a2*pow(phi,2.0f) ) - 10.0f * log10( pow(b0+b1+b2, 2.0f) - 4.0f*(b0*b1 + 4.0f*b0*b2 + b1*b2)*phi + 16.0f*b0*b2*pow(phi, 2.0f) ) );
	//return 20*Math.log((Math.sqrt(square(a0*square(Math.cos(w))-a0*square(Math.sin(w))+a1*Math.cos(w)+a2)+square(2*a0*Math.cos(w)*Math.sin(w)+a1*(Math.sin(w)))) / Math.sqrt(square(square(Math.cos(w))-   square(Math.sin(w))+b1*Math.cos(w)+b2)+square(2*   Math.cos(w)*Math.sin(w)+b1*(Math.sin(w))))))
*/
}

float FxMath::CalcPhaseResponse_LC(float f, float fc) {
    // Phase = arctan(fc / f)
    return atan2f(fc, f);
}
float FxMath::CalcPhaseResponse_HC(float f, float fc) {
    // Phase = arctan(-f / fc)
    return atan2f(-f, fc);
}

float FxMath::CalcPhaseResponse_PEQ(float a0, float a1, float a2, float b0, float b1, float b2, float f, float fs) {
  float omega = (2.0f * PI * f) / fs;
  float sin_omega = sinf(omega);
  float cos_omega = cosf(omega);

  float num_real = a0 + a1 * cos_omega + a2 * (cos_omega * cos_omega - sin_omega * sin_omega);
  float num_imag = a1 * sin_omega + 2.0f * a2 * cos_omega * sin_omega;
  float den_real = b0 + b1 * cos_omega + b2 * (cos_omega * cos_omega - sin_omega * sin_omega);
  float den_imag = b1 * sin_omega + 2.0f * b2 * cos_omega * sin_omega;

  return atan2f(num_imag, num_real) - atan2f(den_imag, den_real);
}

void FxMath::RecalcFilterCoefficients_LR12(sLR12* LR12) {
  double wc = 2.0 * PI * LR12->fc;
  double wc2 = wc * wc;
  double wc22 = 2.0 * wc2;
  double k = wc / tan(PI * (LR12->fc / (float)config->GetUint(MP_ID::SAMPLERATE)));
  double k2 = k * k;
  double k22 = 2.0 * k2;
  double wck2 = 2.0 * wc * k;
  double norm = 1.0 / (k2 + wc2 + wck2);

  if (LR12->isHighpass) {
    // coefficients for HighPass-Filter
    LR12->a[0] = k2 * norm;
    LR12->a[1] = -k22 * norm;
    LR12->a[2] = k2 * norm;
  }else{
    // coefficients for LowPass-Filter
    LR12->a[0] = wc2 * norm;
    LR12->a[1] = wc22 * norm;
    LR12->a[2] = wc2 * norm;
  }

  LR12->b[0] = 0; // we are not using this coefficient but keep it to not confuse with indices
  LR12->b[1] = (-k22 + wc22) * norm;
  LR12->b[2] = (-wck2 + k2 + wc2) * norm;
}

void FxMath::RecalcFilterCoefficients_LR24(sLR24* LR24) {
  double wc = 2.0 * PI * LR24->fc;
  double wc2 = wc * wc;
  double wc3 = wc2 * wc;
  double wc4 = wc2 * wc2;
  double k = wc / tan(PI * (LR24->fc / (float)config->GetUint(MP_ID::SAMPLERATE)));
  double k2 = k * k;
  double k3 = k2 * k;
  double k4 = k2 * k2;
  double sq_tmp1 = sqrt(2.0) * wc3 * k;
  double sq_tmp2 = sqrt(2.0) * wc * k3;
  double norm = 1.0 / (4.0 * wc2 * k2 + 2.0 * sq_tmp1 + k4 + 2.0 * sq_tmp2 + wc4);

  if (LR24->isHighpass) {
    // coefficients for HighPass-Filter
    LR24->a[0] = k4 * norm;
    LR24->a[1] = -4.0 * k4 * norm;
    LR24->a[2] = 6.0 * k4 * norm;
    LR24->a[3] = LR24->a[1];
    LR24->a[4] = LR24->a[0];
  }else{
    // coefficients for LowPass-Filter
    LR24->a[0] = wc4 * norm;
    LR24->a[1] = 4.0 * wc4 * norm;
    LR24->a[2] = 6.0 * wc4 * norm;
    LR24->a[3] = LR24->a[1];
    LR24->a[4] = LR24->a[0];
  }

  LR24->b[0] = 0; // we are not using this coefficient but keep it to not confuse with indices
  LR24->b[1] = (4.0 * (wc4 + sq_tmp1 - k4 - sq_tmp2)) * norm;
  LR24->b[2] = (6.0 * wc4 - 8.0 * wc2 * k2 + 6.0 * k4) * norm;
  LR24->b[3] = (4.0 * (wc4 - sq_tmp1 + sq_tmp2 - k4)) * norm;
  LR24->b[4] = (k4 - 2.0 * sq_tmp1 + wc4 - 2.0 * sq_tmp2 + 4.0 * wc2 * k2) * norm;
}

void FxMath::fxCalcParameters_Reverb(float data[], float roomSizeMs, float rt60, float feedbackLowPassFreq, float dry, float wet) {
  float samplerate = config->GetUint(MP_ID::SAMPLERATE);

  data[0] = pow(10.0f, (-60.0f / (rt60 / (roomSizeMs * 1.5f * 0.001f))) / 20.0f); // decay = 10^(dBperCycle/20)  ->  -1.5dB/cycle = x0.85
	data[1] = (2.0f * PI * feedbackLowPassFreq) / (samplerate + 2.0f * PI * feedbackLowPassFreq); // 7kHz = 43982,297150257105338477007365913 / 91982,297150257105338477007365913 <- alpha = (2 * pi * f_c) / (f_s + 2 * pi * f_c) = (2 * pi * 7000Hz) / (48000Hz + 2 * pi * 7000Hz)
	data[2] = dry;
	data[3] = wet;
  data[4] = roomSizeMs;
  data[5] = (roomSizeMs * samplerate / 1000.0f);
}

void FxMath::fxCalcParameters_Chorus(float data[], float depth[2], float delayMs[2], float phase[2], float freq[2], float mix) {
  float samplerate = config->GetUint(MP_ID::SAMPLERATE);

  data[0] = depth[0];
  data[1] = depth[1];
  data[2] = delayMs[0] * samplerate / 1000.0f;
  data[3] = delayMs[1] * samplerate / 1000.0f;
  data[4] = phase[0];
  data[5] = phase[1];
  data[6] = (2.0f * PI * freq[0]) / samplerate;
  data[7] = (2.0f * PI * freq[0]) / samplerate;
  data[8] = mix;
}

void FxMath::fxCalcParameters_TransientShaper(float data[], float tFastMs, float tMediumMs, float tSlowMs, float attack, float sustain, float delayMs) {
  float samplerate = config->GetUint(MP_ID::SAMPLERATE);

	data[0] = 1.0f - exp(-1.0f / (samplerate * (tFastMs / 1000.0f))); // attack-envelope: 0.05 = softer response, 0.2 = fast on steep edges
	data[1] = 1.0f - exp(-1.0f / (samplerate * (tMediumMs / 1000.0f)));
	data[2] = 1.0f - exp(-1.0f / (samplerate * (tSlowMs / 1000.0f))); // sustain-envelope: 0.05 = short boost, 0.001 = wide punch
	data[3] = attack; // amount of the attack: <1 = less attack, 1 = Neutral, >1 = more attack
	data[4] = sustain; // amount of the sustain: <1 = less sustain, 1 = Neutral, >1 = more sustain
  data[5] = (delayMs * samplerate / 1000.0f); // 1ms
}

void FxMath::fxCalcParameters_Overdrive(float data[], float preGain, float Q, float bias, float hpfInputFreq, float lpfInputFreq, float lpfOutputFreq) {
  float samplerate = config->GetUint(MP_ID::SAMPLERATE);

	data[0] = powf(10.0f, preGain/20.0f);
	data[1] = Q;
	data[2] = bias;
	data[3] = 1.0f / (1.0f + 2.0f * PI * hpfInputFreq * (1.0f/samplerate)); // 1.0f / (1.0f + 2.0f * (float)M_PI * f_c * (1.0f/f_s))
	data[4] = (2.0f * PI * lpfInputFreq) / (samplerate + 2.0f * PI * lpfInputFreq); // (2.0f * (float)M_PI * f_c) / (f_s + 2.0f * (float)M_PI * f_c)
	data[5] = (2.0f * PI * lpfOutputFreq) / (samplerate + 2.0f * PI * lpfOutputFreq); // (2.0f * (float)M_PI * f_c) / (f_s + 2.0f * (float)M_PI * f_c)
}

void FxMath::fxCalcParameters_Delay(float data[], float delayMs[2]) {
  float samplerate = config->GetUint(MP_ID::SAMPLERATE);

  data[0] = (delayMs[0] * samplerate / 1000.0f);
  data[1] = (delayMs[1] * samplerate / 1000.0f);
}

void FxMath::fxCalcParameters_MultibandCompressor(float data[], int channel, int band, float threshold, float ratio, float attack, float hold, float release, float makeup) {
  float samplerate = (float)config->GetUint(MP_ID::SAMPLERATE) / 16.0f;

  data[0] = channel;
  data[1] = band;
	data[2] = threshold;
	data[3] = ratio;
	data[4] = 1.0f - exp(-2197.22457734f/(samplerate * attack)); // ln(10%) - ln(90%) = -2.197224577
	data[5] = hold * samplerate / 1000.0f;
	data[6] = 1.0f - exp(-2197.22457734f/(samplerate * release)); // ln(10%) - ln(90%) = -2.197224577
	data[7] = pow(10.0f, makeup / 20.0f);
}

void FxMath::fxCalcParameters_MultibandCompressorFreq(float data[], int channel, float freq[4]) {
  // Band 1: calculate coeffs for high-cut
  // Band 2..4: calculate coeffs for low- and high-cut
  // Band 5: calculate coeffs for low-cut
  /*
   _____  f0  _____  f1  _____  f2  _____  f3  _____
        \    /     \    /     \    /     \    /
     b0  \  /  b1   \  /   b2  \  /  b3   \  /   b4
          \/         \/         \/         \/
          /\         /\         /\         /\
         /  \       /  \       /  \       /  \
  */

  sPEQ peq;
  peq.Q = 1.0 / sqrt(2.0); // we take 0.707 as Q-factor to have zero gain at the crossover-point
  peq.gain = 1.0;
  float samplerate = config->GetUint(MP_ID::SAMPLERATE);
  data[0] = channel;

  // calculate coeffs for high-cut (low-pass)
  peq.type = 6;
  peq.fc = freq[0];
  RecalcFilterCoefficients_PEQ(&peq);
  data[1] = peq.a[0];
  data[2] = peq.a[1];
  data[3] = peq.a[2];
  data[4] = -peq.b[1];
  data[5] = -peq.b[2];

  for (int i = 1; i < 4; i++) {
    // inner bands: calculate low- and high-cut

    // calculate coeffs for low-cut (high-pass)
    peq.type = 7;
    peq.fc = freq[i-1];
    RecalcFilterCoefficients_PEQ(&peq);
    data[6 + ((i-1) * 10) + 0] = peq.a[0];
    data[6 + ((i-1) * 10) + 1] = peq.a[1];
    data[6 + ((i-1) * 10) + 2] = peq.a[2];
    data[6 + ((i-1) * 10) + 3] = -peq.b[1];
    data[6 + ((i-1) * 10) + 4] = -peq.b[2];

    // calculate coeffs for high-cut (low-pass)
    peq.type = 6;
    peq.fc = freq[i];
    RecalcFilterCoefficients_PEQ(&peq);
    data[6 + ((i-1) * 10) + 5] = peq.a[0];
    data[6 + ((i-1) * 10) + 6] = peq.a[1];
    data[6 + ((i-1) * 10) + 7] = peq.a[2];
    data[6 + ((i-1) * 10) + 8] = -peq.b[1];
    data[6 + ((i-1) * 10) + 9] = -peq.b[2];
  }

  // Band 5: calculate coeffs for low-cut (high-pass)
  peq.type = 6;
  peq.fc = freq[3];
  RecalcFilterCoefficients_PEQ(&peq);
  data[36] = peq.a[0];
  data[37] = peq.a[1];
  data[38] = peq.a[2];
  data[39] = -peq.b[1];
  data[40] = -peq.b[2];
}

void FxMath::fxCalcParameters_DynamicEQ(float data[], int band, int type, float frequency, float staticGain, float maxDynamicGain, float Q, float threshold, float ratio, float attack, float release) {
  data[0] = band;
  data[1] = type;

	// set control-type
	switch(type) {
		case 1: // Peak-Filter
			// here the control-filter has to be a band-pass
			data[2] = 4;
			break;
		case 2: // Low-Shelf
			// here the control-filter has to be a low-pass
			data[2] = 6;
			break;
		case 3: // High-Shelf
			// here the control-filter has to be a high-pass
			data[2] = 7;
			break;
	}

	data[3] = frequency;
	data[4] = staticGain;
  data[5] = maxDynamicGain;
	data[6] = Q;
  data[7] = threshold;
  data[8] = ratio;

  float fs = (float)config->GetUint(MP_ID::SAMPLERATE)/(16.0f); // 16 samples per buffer
  data[9] = 1.0f - exp(-1000.0f / (fs * attack)); // convert ms to coeff
  data[10] = 1.0f - exp(-1000.0f / (fs * release)); // convert ms to coeff
}

// Clean-room program equalizer after the published Pulse Techniques EQP-1A
// specifications and operating instructions (no plugin code, schematics or
// measured hardware curves are used):
//   20/30/60/100 cps shelf boost 0..13.5 dB and shelf attenuate 0..17.5 dB,
//   3/4/5/8/10/12/16 kcs peak boost 0..18 dB with bandwidth sharp..broad,
//   5/10/20 kcs shelf attenuate 0..16 dB, IN/OUT key zeroes all curves while
//   the amplifier stays in circuit; push-pull amplifier with 0.15 % distortion
//   at +10 dBm and +21 dBm maximum peak output.
// Modelling choices where the documents give no numbers:
//   - both low curves have their half-gain (dB) point at the selected
//     frequency, the attenuate curve half an octave higher. The boost is a
//     second-order shelf (Q 0.707), the attenuate a gentler first-order
//     shelf, so simultaneous boost and attenuate give the well-known low
//     bump with a shallow dip above it.
//   - the high boost is a resonant peak whose pole Q goes from 2.0 (sharp)
//     to 0.5 (broad); the boost changes only the zeros, so the peak gain
//     equals the boost at every bandwidth.
//   - the high attenuate is a first-order shelf with its half-gain point at
//     the selected frequency.
//   - the amplifier is an odd-order cubic (push-pull cancels even orders),
//     scaled so that at 0 dB drive digital full scale corresponds to +21 dBm
//     and a +10 dBm sine produces exactly 0.15 % THD.
// Coefficients for the channel EQ kernel (OpenAudioDSP eqp1a.c), in the order of its
// EQP1A_* enum. The DSP runs the low curves as trapezoidal integrators, whose
// coefficients keep the shelf gains in float32 where direct-form ones near DC are off
// by up to 0.4 dB, and the high boost as x + mix * bandpass. With g the integrator gain:
//   [0..4]  LF boost SVF: 2a, g a, 2g, gain - 1, k (sqrt(gain) - 1) + g (gain - 1), a = 1 / (1 + g (g + k))
//   [5..7]  LF atten one-pole: 2g / (1 + g), DC gain, (1 - DC gain) / (1 + g)
//   [8..10] HF boost: a1, a2 of the peak's denominator, gain of the bandpass (1 - 1/z^2)
//   [11,12] HF atten one-pole: 2g / (1 + g), (HF gain - 1) / (1 + g)
//   [13]    amplifier pre-scale for int32-scaled samples (0 = off)
// Identity sections get exactly zero mixes, so the DSP skips them.
void FxMath::fxCalcPultecChannelCoefficients(float data[], float samplerate, float lfFreq, float lfBoostDb, float lfAttenDb, float hfBoostFreq, float hfBoostDb, float bandwidth, float hfAttenFreq, float hfAttenDb, bool eqIn, bool tube, float driveDb) {
  const double pi = 3.14159265358979323846;
  double fs = samplerate;
  if (!eqIn) {
    lfBoostDb = lfAttenDb = hfBoostDb = hfAttenDb = 0.0f;
  }
  double fMax = 0.45 * fs;

  // LF boost: second-order bilinear shelf (Q 0.707, no prewarping), poles at w / sqrt(r)
  {
    double r = pow(10.0, lfBoostDb / 40.0), k = sqrt(2.0);
    double g = pi * lfFreq / (sqrt(r) * fs), a = 1.0 / (1.0 + g * (g + k));
    data[0] = (float)(2.0 * a);
    data[1] = (float)(g * a);
    data[2] = (float)(2.0 * g);
    data[3] = (float)(r * r - 1.0);
    data[4] = (float)(k * (r - 1.0) + g * (r * r - 1.0));
  }
  // LF atten: first-order shelf, pole at wl sqrt(att)
  {
    double att = pow(10.0, lfAttenDb / 20.0), g = pi * lfFreq * sqrt(2.0 * att) / fs;
    data[5] = (float)(2.0 * g / (1.0 + g));
    data[6] = (float)(1.0 / att);
    data[7] = (float)((1.0 - 1.0 / att) / (1.0 + g));
  }
  // HF boost: resonant peak, prewarped to f0
  {
    double f0 = fmin(hfBoostFreq, fMax), k = pow(10.0, hfBoostDb / 20.0);
    double q = 2.0 * pow(0.25, fmin(fmax(bandwidth, 0.0), 1.0));
    double w0 = 2.0 * pi * f0, c = w0 / tan(w0 / (2.0 * fs)), a0 = c * c + w0 / q * c + w0 * w0;
    data[8] = (float)(2.0 * (w0 * w0 - c * c) / a0);
    data[9] = (float)((c * c - w0 / q * c + w0 * w0) / a0);
    data[10] = (float)((k - 1.0) * w0 / q * c / a0);
  }
  // HF atten: first-order shelf, prewarped pole at wh / sqrt(att)
  {
    double att = pow(10.0, hfAttenDb / 20.0), g = tan(pi * fmin(hfAttenFreq, fMax) / fs) / sqrt(att);
    data[11] = (float)(2.0 * g / (1.0 + g));
    data[12] = (float)((1.0 / att - 1.0) / (1.0 + g));
  }
  data[13] = tube ? (float)(sqrt(12.0 * 0.0015) * pow(10.0, 11.0 / 20.0) * pow(10.0, driveDb / 20.0) / 2147483648.0) : 0.0f;
}

// response of the channel kernel's structure, evaluated in double
static double PultecChannelGainDb(const float c[], double f, double fs) {
  using C = std::complex<double>;
  C z = std::polar(1.0, 2.0 * 3.14159265358979323846 * f / fs);
  // LF SVF: state (z1/2, z2), band = A1 z1 + A2 (x - z2), z1 <- band - z1, z2 <- z2 + G2 band
  double a = c[0] - 1, b = -c[1], cc = c[2] * c[0], d = 1 - c[2] * c[1];
  C det = (z - a) * (z - d) - b * cc;
  C s0 = ((z - d) * double(c[1]) + b * double(c[2] * c[1])) / det, s1 = (cc * double(c[1]) + (z - a) * double(c[2] * c[1])) / det;
  C h = (1.0 + double(c[4]) * c[1]) + double(c[4]) * c[0] * s0 + (double(c[3]) - double(c[4]) * c[1]) * s1;
  h *= double(c[6]) + double(c[7]) - double(c[7]) * c[5] / (z - (1.0 - c[5]));
  h *= 1.0 + double(c[10]) * (1.0 - 1.0 / (z * z)) / (1.0 + double(c[8]) / z + double(c[9]) / (z * z));
  h *= 1.0 + double(c[12]) * (z - 1.0) / (z - 1.0 + double(c[11]));
  return 20.0 * log10(std::abs(h));
}

TEST_CASE("Program EQ curves follow the EQP-1A specification")
{
  float ch[FxMath::CHANNEL_EQP1A_COEFFS];
  for (double fs : {44100.0, 48000.0, 96000.0})
  {
    FxMath::fxCalcPultecChannelCoefficients(ch, fs, 60, 0, 0, 10000, 0, 0.5f, 10000, 0, true, false, 0);
    for (double f = 20; f < 20000; f *= 1.1)
      CHECK(std::fabs(PultecChannelGainDb(ch, f, fs)) < 1e-4);

    // simultaneous low boost and attenuate: bump below, dip above the selected frequency
    FxMath::fxCalcPultecChannelCoefficients(ch, fs, 60, 10, 8, 10000, 0, 0.5f, 10000, 0, true, false, 0);
    CHECK(PultecChannelGainDb(ch, 20, fs) > 1.5);
    CHECK(PultecChannelGainDb(ch, 120, fs) < -1.5);

    // full shelf gains, half gain at the selected frequency, HF peak stays at 16 kHz
    FxMath::fxCalcPultecChannelCoefficients(ch, fs, 20, 13.5f, 0, 16000, 18, 0, 20000, 0, true, false, 0);
    CHECK(std::fabs(PultecChannelGainDb(ch, 0.5, fs) - (13.5)) < 0.01);
    CHECK(std::fabs(PultecChannelGainDb(ch, 20, fs) - (6.75)) < 0.01);
    CHECK(std::fabs(PultecChannelGainDb(ch, 16000, fs) - (18.0)) < 0.01);
    FxMath::fxCalcPultecChannelCoefficients(ch, fs, 20, 0, 17.5f, 3000, 0, 0.5f, 5000, 16, true, false, 0);
    CHECK(std::fabs(PultecChannelGainDb(ch, 0.5, fs) - (-17.5)) < 0.01);
    CHECK(std::fabs(PultecChannelGainDb(ch, 5000, fs) - (-8.0)) < 0.01);
    CHECK(PultecChannelGainDb(ch, 1000, fs) > -1.0);
  }

  // EQ out: exact identity sections, which the DSP skips; the amplifier stays in
  FxMath::fxCalcPultecChannelCoefficients(ch, 48000, 100, 13.5f, 17.5f, 16000, 18, 0, 5000, 16, false, true, 0);
  CHECK(ch[3] == 0.0f);
  CHECK(ch[4] == 0.0f);
  CHECK(ch[6] == 1.0f);
  CHECK(ch[7] == 0.0f);
  CHECK(ch[10] == 0.0f);
  CHECK(ch[12] == 0.0f);
  CHECK(ch[13] > 0.0f);
}

}
