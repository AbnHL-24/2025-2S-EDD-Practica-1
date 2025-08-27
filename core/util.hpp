#pragma once

// Función que restringe un valor entero dentro de un rango específico
// Parámetros: v = valor a restringir, lo = límite inferior, hi = límite superior
// Retorna: el valor original si está dentro del rango, o el límite correspondiente si está fuera
inline int clampInt(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }

// Función que simula un lanzamiento de moneda basado en una semilla
// Parámetro: seed = semilla para generar resultado determinístico
// Retorna: 0 o 1 dependiendo del bit menos significativo de la semilla (placeholder sin RNG real)
inline int coinFlip50(int seed) { return (seed & 1); }
