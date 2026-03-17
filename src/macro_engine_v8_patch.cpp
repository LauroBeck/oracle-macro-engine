// ===== V8 MACRO EXTENSION =====

double rate_accel = accel(us10y);
double dollar_accel = accel(dxy);
double gold_accel = accel(gold);

std::cout << "\n=== GLOBAL MACRO (v8) ===\n";
std::cout << "Rates accel : " << rate_accel << "\n";
std::cout << "Dollar accel: " << dollar_accel << "\n";
std::cout << "Gold accel  : " << gold_accel << "\n";

// FED PRESSURE
if(rate_accel > 0)
    std::cout << "🏦 FED tightening pressure\n";
else
    std::cout << "🏦 FED easing bias\n";

// LIQUIDITY
if(dollar_accel > 0)
    std::cout << "💵 Liquidity tightening (strong USD)\n";
else
    std::cout << "💵 Liquidity expansion (weak USD)\n";

// RISK SIGNAL
if(gold_accel > 0)
    std::cout << "🥇 Risk-off hedge demand rising\n";
else
    std::cout << "🥇 Risk-on (gold weakening)\n";

// ===== MASTER OVERRIDE =====

// Strong risk-off condition
if(dollar_accel > 0 && rate_accel > 0 && gold_accel > 0){
    std::cout << "\n🚨 GLOBAL RISK-OFF OVERRIDE\n";
    std::cout << "→ Cut equity exposure\n";
    std::cout << "→ Long USD / Gold\n";
}

// Strong risk-on condition
if(dollar_accel < 0 && rate_accel < 0 && gold_accel < 0){
    std::cout << "\n🚀 GLOBAL RISK-ON CONFIRMATION\n";
    std::cout << "→ Increase equity exposure\n";
}
