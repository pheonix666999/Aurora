# DSP design

```mermaid
flowchart LR
  A[Trim/sanitize] --> B[DC + HPF] --> C[Gated RMS AGC] --> D[6-band EQ]
  D --> E[LR4 crossover] --> F[4 linked compressors] --> G[Recombine]
  G --> H[Harmonic density] --> I[M/S width, balance, low-end preservation, correlation guard]
  I --> J[4x soft peak] --> K[Look-ahead limiter] --> L[Trim/polarity/meters/FFT]
```

Input sanitation replaces non-finite samples with zero and bounds pathological finite input. The DC blocker is a stable one-pole differentiating filter; the optional high pass is a smoothed 12 dB/octave biquad. Gains and bypasses use sample smoothing.

The AGC uses per-channel 350 ms RMS energy, a configurable silence gate, attack/release coefficients, bounded gain/attenuation, and a blend from independent to maximum-linked detection. It applies smooth per-channel correction without changing balance at the default 100% link. It is not described as LUFS.

The EQ uses low/high shelves plus four bell biquads. Coefficients update only when a band value changes. The UI response is calculated from the same coefficient designs. Three fourth-order Linkwitz-Riley filters split around a middle crossover and then split each half; ordering and minimum spacing are clamped before coefficient updates.

Each band compressor detects the stereo maximum, calculates a continuous soft-knee curve, smooths linked gain with its attack/release, applies makeup and parallel mix, and reports actual gain reduction. One lowest-numbered active solo wins. Recombination sums all audible bands.

The enhancer uses bounded `tanh` density, tilt-derived tone, wet/dry mix, and post-shaper DC removal. Stereo processing uses mid/side width and asymmetric balance gains. Its enabled-by-default low-end preservation filter leaves center/mid bass intact while removing low-frequency side energy below the adjustable cutoff (120 Hz by default). The slow correlation guard reduces unsafe widening rather than abruptly switching to mono.

The soft peak stage upsamples 4× with polyphase half-band IIR filters, applies a bounded Soft or Firm curve, downsamples, and contributes integer latency. It is labelled **Oversampled Peak**, not certified true peak.

The limiter maintains preallocated audio and detector rings. Audio always uses a fixed 10 ms maximum delay, keeping reported host latency stable; the detector tap changes so the parameter selects 1–10 ms of effective anticipation. Attack is immediate, release is style-adjusted, channels are linked, and output is bounded to the selected sample ceiling. Global bypass uses a separate equal-latency dry ring and a smoothed crossfade.
