#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

struct Row {
    double tech, capex, oil;
    double us10y, us2y, dxy, gold;
    double hy_spread;
    double vix;
};

class MacroEngine {
private:
    std::vector<Row> data;

public:
    void load(const std::string& file){
        std::ifstream f(file);
        std::string line;
        std::getline(f, line);

        while(std::getline(f, line)){
            std::stringstream ss(line);
            Row r;
            char comma;

            ss >> r.tech >> comma
               >> r.capex >> comma
               >> r.oil >> comma
               >> r.us10y >> comma
               >> r.us2y >> comma
               >> r.dxy >> comma
               >> r.gold >> comma
               >> r.hy_spread >> comma
               >> r.vix;

            data.push_back(r);
        }

        std::cout << "Loaded rows: " << data.size() << "\n";
    }

    double accel(double a,double b,double c){
        return (c-b)-(b-a);
    }

    double compute_vol(){
        if(data.size()<2) return 0;
        double sum=0;
        for(size_t i=1;i<data.size();i++){
            double r=(data[i].tech-data[i-1].tech)/data[i-1].tech;
            sum+=r*r;
        }
        return std::sqrt(sum/data.size());
    }

    void run(){
        if(data.size()<3){
            std::cout<<"Not enough data\n";
            return;
        }

        int n=data.size();

        // CORE
        double tech_accel  = accel(data[n-3].tech,data[n-2].tech,data[n-1].tech);
        double capex_accel = accel(data[n-3].capex,data[n-2].capex,data[n-1].capex);
        double oil_accel   = accel(data[n-3].oil,data[n-2].oil,data[n-1].oil);

        // GLOBAL
        double rate_accel  = accel(data[n-3].us10y,data[n-2].us10y,data[n-1].us10y);
        double dxy_accel   = accel(data[n-3].dxy,data[n-2].dxy,data[n-1].dxy);
        double gold_accel  = accel(data[n-3].gold,data[n-2].gold,data[n-1].gold);

        // v9
        double spread_now = data[n-1].us10y - data[n-1].us2y;
        double spread_prev = data[n-2].us10y - data[n-2].us2y;
        double curve_accel = spread_now - spread_prev;

        double credit_accel = accel(
            data[n-3].hy_spread,
            data[n-2].hy_spread,
            data[n-1].hy_spread
        );

        double vix_now = data[n-1].vix;

        double alpha = (capex_accel - tech_accel - oil_accel)/3.0;
        double vol = compute_vol();

        std::cout << "\n=== MACRO ENGINE v9 ===\n";
        std::cout << "Alpha: " << alpha << "\n";

        // ===== v9 BLOCK =====
        std::cout << "\n=== SYSTEMIC RISK (v9) ===\n";

        std::cout << "Yield Curve Spread: " << spread_now << "\n";
        if(spread_now < 0)
            std::cout << "⚠️ Yield curve inversion (recession risk)\n";
        else
            std::cout << "📈 Normal curve\n";

        if(curve_accel > 0)
            std::cout << "📈 Curve steepening (growth improving)\n";
        else
            std::cout << "📉 Curve flattening\n";

        std::cout << "\nCredit accel: " << credit_accel << "\n";
        if(credit_accel > 0)
            std::cout << "💳 Credit stress rising\n";
        else
            std::cout << "💳 Credit stable\n";

        std::cout << "\nVIX: " << vix_now << "\n";
        if(vix_now > 25)
            std::cout << "🚨 HIGH VOL (risk-off)\n";
        else if(vix_now < 15)
            std::cout << "🟢 LOW VOL (trend)\n";
        else
            std::cout << "⚖️ NEUTRAL VOL\n";

        // ===== MASTER OVERRIDE =====
        if(spread_now < 0 && credit_accel > 0 && vix_now > 25){
            std::cout << "\n🚨 SYSTEMIC RISK-OFF\n";
            std::cout << "→ Exit equities\n";
        }

        if(spread_now > 0 && credit_accel <= 0 && vix_now < 15){
            std::cout << "\n🚀 SYSTEMIC RISK-ON\n";
            std::cout << "→ Max long exposure\n";
        }

        // ===== RISK ENGINE =====
        std::cout << "\n=== RISK ENGINE ===\n";
        std::cout << "Volatility: " << vol << "\n";

        if(vol < 0.01)
            std::cout << "🟢 LOW VOL → Add exposure\n";
    }
};

int main(){
    MacroEngine e;
    e.load("data/macro_data_v4.csv");
    e.run();
}
