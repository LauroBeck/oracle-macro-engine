#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

struct Row {
    double tech, capex, oil;
    double us10y, dxy, gold;
};

class MacroEngine {
private:
    std::vector<Row> data;

public:
    void load(const std::string& file){
        std::ifstream f(file);
        std::string line;
        std::getline(f, line); // skip header

        while(std::getline(f, line)){
            std::stringstream ss(line);
            Row r;
            char comma;

            ss >> r.tech >> comma
               >> r.capex >> comma
               >> r.oil >> comma
               >> r.us10y >> comma
               >> r.dxy >> comma
               >> r.gold;

            data.push_back(r);
        }

        std::cout << "Loaded rows: " << data.size() << "\n";
    }

    double accel(double a, double b, double c){
        return (c - b) - (b - a);
    }

    double compute_vol(){
        if(data.size() < 2) return 0;
        double sum = 0;
        for(size_t i=1;i<data.size();i++){
            double r = (data[i].tech - data[i-1].tech) / data[i-1].tech;
            sum += r*r;
        }
        return std::sqrt(sum / data.size());
    }

    void run(){
        if(data.size() < 3){
            std::cout << "Not enough data\n";
            return;
        }

        int n = data.size();

        double tech_accel  = accel(data[n-3].tech,  data[n-2].tech,  data[n-1].tech);
        double capex_accel = accel(data[n-3].capex, data[n-2].capex, data[n-1].capex);
        double oil_accel   = accel(data[n-3].oil,   data[n-2].oil,   data[n-1].oil);

        double rate_accel  = accel(data[n-3].us10y, data[n-2].us10y, data[n-1].us10y);
        double dxy_accel   = accel(data[n-3].dxy,   data[n-2].dxy,   data[n-1].dxy);
        double gold_accel  = accel(data[n-3].gold,  data[n-2].gold,  data[n-1].gold);

        double alpha = (capex_accel - tech_accel - oil_accel) / 3.0;
        double confidence = std::abs(alpha) * 100;

        double vol = compute_vol();

        // ===== OUTPUT =====

        std::cout << "\n=== MACRO ENGINE v8 ===\n";
        std::cout << "Alpha: " << alpha << "\n";
        std::cout << "Confidence: " << confidence << "%\n";

        // ROTATION
        if(tech_accel < 0 && capex_accel > 0)
            std::cout << "⚖️ ROTATION\n➡️ TECH → INDUSTRIALS\n";

        if(oil_accel > 0)
            std::cout << "⛽ ENERGY DOMINANCE\n";

        // ACCELERATION
        std::cout << "\n=== ACCELERATION ===\n";
        std::cout << "Tech accel : " << tech_accel << "\n";
        std::cout << "Capex accel: " << capex_accel << "\n";
        std::cout << "Oil accel  : " << oil_accel << "\n";

        if(capex_accel > 0) std::cout << "📈 Capex accelerating\n";
        if(tech_accel < 0) std::cout << "📉 Tech weakening\n";

        // POSITION
        double position = std::min(std::max(alpha * 50, 0.0), 100.0);

        std::cout << "\n=== POSITION ===\n";
        if(position == 0)
            std::cout << "🛑 DEFENSIVE / SHORT | 0%\n";
        else
            std::cout << "🔄 PARTIAL LONG | " << position << "%\n";

        // PLAYBOOK
        std::cout << "\n=== PLAYBOOK ===\n";
        if(capex_accel > 0)
            std::cout << "→ Long Industrials / Defense\n";
        if(tech_accel < 0)
            std::cout << "→ Reduce Tech\n";
        if(oil_accel > 0)
            std::cout << "→ Long Energy\n";
        std::cout << "→ Avoid SaaS / Cloud\n";

        // FORWARD BIAS
        std::cout << "\n=== FORWARD BIAS ===\n";
        if(capex_accel > 0)
            std::cout << "🚀 Industrial expansion strengthening\n";
        if(tech_accel < 0)
            std::cout << "⚠️ Tech downside continuation likely\n";
        if(oil_accel < 0)
            std::cout << "🛢️ Oil cooling (inflation easing)\n";
        if(capex_accel > 0 && oil_accel < 0)
            std::cout << "💡 GOLDILOCKS setup (growth + lower inflation)\n";

        // ===== GLOBAL MACRO (v8) =====
        std::cout << "\n=== GLOBAL MACRO ===\n";
        std::cout << "Rates accel : " << rate_accel << "\n";
        std::cout << "Dollar accel: " << dxy_accel << "\n";
        std::cout << "Gold accel  : " << gold_accel << "\n";

        if(rate_accel > 0)
            std::cout << "🏦 FED tightening pressure\n";
        else
            std::cout << "🏦 FED easing bias\n";

        if(dxy_accel > 0)
            std::cout << "💵 Liquidity tightening (strong USD)\n";
        else
            std::cout << "💵 Liquidity expansion (weak USD)\n";

        if(gold_accel > 0)
            std::cout << "🥇 Risk-off hedge demand rising\n";
        else
            std::cout << "🥇 Risk-on (gold weakening)\n";

        if(dxy_accel > 0 && rate_accel > 0 && gold_accel > 0){
            std::cout << "\n🚨 GLOBAL RISK-OFF OVERRIDE\n";
            std::cout << "→ Cut equity exposure\n";
        }

        if(dxy_accel < 0 && rate_accel < 0 && gold_accel < 0){
            std::cout << "\n🚀 GLOBAL RISK-ON CONFIRMATION\n";
            std::cout << "→ Increase equity exposure\n";
        }

        // ===== RISK ENGINE =====
        std::cout << "\n=== RISK ENGINE ===\n";
        std::cout << "Volatility: " << vol << "\n";

        if(vol < 0.01)
            std::cout << "🟢 LOW VOL → Add exposure (trend regime)\n";

        if(vol < 0.01 && alpha > 0.4)
            std::cout << "🚀 TREND CONFIRMED → Scale faster\n";
    }
};

int main(){
    MacroEngine e;
    e.load("data/macro_data_v3.csv");
    e.run();
    return 0;
}
