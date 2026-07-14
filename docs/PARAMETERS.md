# Parameter contract

All IDs are stable lowercase camelCase JUCE parameters with version hint 1. Gains, mixes, drives, width/balance, polarity, and bypass transitions are smoothed; EQ/crossover coefficients update at block boundaries. Choice indices are zero based.

| IDs | Type/range/default | Unit | Module |
|---|---|---|---|
| `inputTrimDb` | float -18..18 / 0 | dB | Input |
| `dcBlockEnabled` | bool / on | — | Input |
| `highPassEnabled`, `highPassFrequencyHz` | bool on; float 15..60 / 25 | Hz | Input |
| `agcEnabled` | bool / on | — | AGC |
| `agcTargetDb`, `agcMaxGainDb`, `agcMaxAttenuationDb` | -24..-8 / -15; 0..15 / 9; -15..0 / -9 | dBFS/dB | AGC |
| `agcAttackMs`, `agcReleaseMs`, `agcGateThresholdDb`, `agcStereoLinkPercent` | 100..2000 / 500; 1000..12000 / 4500; -80..-30 / -52; 0..100 / 100 | ms, dBFS, % | AGC |
| `eqEnabled`, `eqBand1Enabled`…`eqBand6Enabled` | bool / on | — | EQ |
| `eqBand1FrequencyHz`…`eqBand6FrequencyHz` | band ranges 30–250, 80–1000, 250–3500, 1000–8000, 2500–14000, 5000–18000; defaults 90, 280, 1000, 3000, 7000, 11000 | Hz | EQ |
| `eqBand1GainDb`…`eqBand6GainDb` | -12..12 / 0 | dB | EQ |
| `eqBand1Q`…`eqBand6Q` | 0.25..6 / 0.8 | Q | EQ |
| `crossover1Hz`, `crossover2Hz`, `crossover3Hz` | 60..300 /120; 400..3000 /1100; 3000..14000 /6000 | Hz | Crossover |
| `multibandEnabled`, `multibandAmountPercent` | bool on; 0..100 /70 | % | Multiband |
| `band1Enabled`…`band4Enabled`, `band1Solo`…`band4Solo` | bool / on; bool / off | — | Band dynamics |
| `band1ThresholdDb`…`band4ThresholdDb` | -50..0 / -20,-18,-17,-16 | dB | Band dynamics |
| `band1Ratio`…`band4Ratio` | 1..10 / 2.4,2,1.8,1.6 | ratio | Band dynamics |
| `band1AttackMs`…`band4AttackMs` | 0.5..250 / 35,20,10,4 | ms | Band dynamics |
| `band1ReleaseMs`…`band4ReleaseMs` | 20..2000 / 280,220,160,110 | ms | Band dynamics |
| `band1KneeDb`…`band4KneeDb` | 0..18 / 6 | dB | Band dynamics |
| `band1MakeupDb`…`band4MakeupDb` | -6..12 / 0 | dB | Band dynamics |
| `band1MixPercent`…`band4MixPercent` | 0..100 / 100 | % | Band dynamics |
| `enhancerEnabled`, `enhancerDriveDb`, `enhancerMixPercent`, `enhancerTonePercent` | bool on; 0..12 /2; 0..100 /12; -100..100 /0 | dB, % | Enhancer |
| `stereoEnabled`, `stereoWidthPercent`, `stereoBalancePercent` | bool on; 0..150 /108; -100..100 /0 | % | Stereo |
| `monoBassEnabled`, `monoBassFrequencyHz`, `correlationProtectionEnabled` | bool on; 40..250 /120; bool on | Hz | Stereo |
| `clipperEnabled`, `clipperDriveDb`, `clipperMode` | bool on; 0..9 /1.5; Soft/Firm | dB | Oversampled Peak |
| `limiterEnabled`, `limiterDriveDb`, `limiterCeilingDb` | bool on; 0..12 /3; -3..-0.1 /-0.8 | dB | Limiter |
| `limiterLookaheadMs`, `limiterReleaseMs`, `limiterStyle` | 1..10 /5; 20..500 /120; Transparent/Punch | ms | Limiter |
| `outputTrimDb`, `invertLeft`, `invertRight`, `globalBypass` | -18..6 /0; bool off; bool off; bool off | dB | Output |
| `analyzerEnabled`, `analyzerMode`, `analyzerSmoothing`, `analyzerFreeze` | bool on; Input/Output/Both; 0..100 /65; bool off | % | UI analysis |

