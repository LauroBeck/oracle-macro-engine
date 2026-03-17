# 🚀 Oracle Macro Engine (v9)

Multi-layer macroeconomic engine written in C++ for systematic market regime detection.

## 🧠 Features

- 📈 Growth (Capex acceleration)
- 🛢️ Inflation (Oil dynamics)
- 💻 Sector Rotation (Tech vs Industrials)
- 🏦 Interest Rates (US10Y)
- 💵 Dollar Liquidity (DXY)
- 🥇 Gold (Risk sentiment)
- 📉 Yield Curve (2Y–10Y)
- 💳 Credit Spreads (HY)
- 📊 VIX (Volatility regime)

---

## ⚙️ Build

```bash
g++ -O3 -march=native -std=c++17 src/macro_engine_v9.cpp -o build/macro_engine_v9
./build/macro_engine_v9
tech,capex,oil,us10y,us2y,dxy,gold,hy_spread,vix
