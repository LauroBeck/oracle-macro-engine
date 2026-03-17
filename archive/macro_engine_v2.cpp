#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <numeric>

struct Row {
    double oracle, capex, nasdaq, europe, oil;
};

class Engine {
private:
    std::vector<Row> data;

public:
    void load(const std::string& file) {
        std::ifstream f(file);
        std::string line;

        getline(f, line);

        while (getline(f, line)) {
            std::stringstream ss(line);
            std::string val;
            std::vector<double> row;

            while (getline(ss, val, ',')) {
                row.push_back(std::stod(val));
            }

            if (row.size() >= 5)
                data.push_back({row[0], row[1], row[2], row[3], row[4]});
        }

        std::cout << "Loaded rows: " << data.size() << std::endl;
    }

    std::vector<double> col(int i) {
        std::vector<double> v;
        for (auto& r : data) {
            if (i==0) v.push_back(r.oracle);
            if (i==1) v.push_back(r.capex);
            if (i==2) v.push_back(r.nasdaq);
            if (i==3) v.push_back(r.europe);
            if (i==4) v.push_back(r.oil);
        }
        return v;
    }

    double corr(std::vector<double>& x, std::vector<double>& y) {
        int n=x.size();
        double sx=0, sy=0, sxy=0, sx2=0, sy2=0;

        for(int i=0;i<n;i++){
            sx+=x[i]; sy+=y[i];
            sxy+=x[i]*y[i];
            sx2+=x[i]*x[i];
            sy2+=y[i]*y[i];
        }

        return (n*sxy - sx*sy) /
               sqrt((n*sx2 - sx*sx)*(n*sy2 - sy*sy));
    }

    double momentum(std::vector<double>& v, int w=5) {
        if (v.size()<w) return 0;
        double start=v[v.size()-w];
        double end=v.back();
        return log((end+1e-6)/(start+1e-6));
    }

    double compute() {
        auto oracle = col(0);
        auto capex  = col(1);
        auto nasdaq = col(2);
        auto europe = col(3);
        auto oil    = col(4);

        double c_oracle = corr(oracle, nasdaq);
        double c_capex  = corr(capex, nasdaq);

        double m_oracle = momentum(oracle);
        double m_capex  = momentum(capex);
        double m_europe = momentum(europe);
        double m_oil    = momentum(oil);

        // Oil inverse effect
        m_oil = -m_oil;

        std::cout << "Corr Oracle: " << c_oracle << std::endl;
        std::cout << "Corr Capex : " << c_capex << std::endl;

        std::cout << "Momentum Oracle: " << m_oracle << std::endl;
        std::cout << "Momentum Capex : " << m_capex << std::endl;
        std::cout << "Momentum Europe: " << m_europe << std::endl;
        std::cout << "Momentum Oil   : " << m_oil << std::endl;

        double alpha =
            0.30*c_capex +
            0.20*c_oracle +
            0.15*m_capex +
            0.10*m_oracle +
            0.15*m_europe +
            0.10*m_oil;

        return alpha;
    }

    void regime(double a) {
        std::cout << "\nAlpha: " << a << std::endl;

        if(a>0.8) std::cout<<"🚀 STRONG BULL\n";
        else if(a>0.5) std::cout<<"📈 GROWTH\n";
        else if(a>0.2) std::cout<<"⚖️ ROTATION\n";
        else std::cout<<"⚠️ RISK OFF\n";
    }
};

int main(){
    Engine e;
    e.load("macro_data_v2.csv");

    double a = e.compute();
    e.regime(a);
}
