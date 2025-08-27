#pragma once

inline int clampInt(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }
inline int coinFlip50(int seed) { return (seed & 1); } // placeholder sin RNG real
