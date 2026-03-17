#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

struct Row {
    double oracle, capex, nasdaq, europe, oil;
};

bool valid(double x){ return std::isfinite(x); }

double clamp(double x){
    if(!valid(x)) return 0;
    if(x>1) return 1;
    if(x<-1) return -1;
    return x;
}

class Engine {
private:
    std::vector<Row> data;

public:
    void load(const std::string& file){
        std::ifstream f(file);
        std::string line;
        getline(f,line);

        while(getline(f,line)){
            std::stringstream ss(line);
            std::string val;
            std::vector<double> row;

            while(getline(ss,val,',')){
                row.push_back(std::stod(val));
            }

            if(row.size()>=5){
                bool ok=true;
                for(auto v:row) if(!valid(v)) ok=false;

                if(ok)
                    data.push_back({row[0],row[1],row[2],row[3],row[4]});
            }
        }

        std::cout<<"Loaded rows: "<<data.size()<<"\n";
    }

    std::vector<double> col(int i){
        std::vector<double> v;
        for(auto&r:data){
            if(i==0) v.push_back(r.oracle);
            if(i==1) v.push_back(r.capex);
            if(i==2) v.push_back(r.nasdaq);
            if(i==3) v.push_back(r.europe);
            if(i==4) v.push_back(r.oil);
        }
        return v;
    }

    double corr(std::vector<double>&x,std::vector<double>&y){
        int n=x.size();
        double sx=0,sy=0,sxy=0,sx2=0,sy2=0;

        for(int i=0;i<n;i++){
            sx+=x[i]; sy+=y[i];
            sxy+=x[i]*y[i];
            sx2+=x[i]*x[i];
            sy2+=y[i]*y[i];
        }

        double denom = sqrt((n*sx2-sx*sx)*(n*sy2-sy*sy));
        if(denom==0) return 0;

        return clamp((n*sxy - sx*sy)/denom);
    }

    double momentum(std::vector<double>&v,int w=5){
        if(v.size()<w) return 0;

        double s=v[v.size()-w];
        double e=v.back();

        if(!valid(s)||!valid(e)||fabs(s)<1e-8||fabs(e)<1e-8) return 0;

        double val=log((e+1e-6)/(s+1e-6));
        if(!valid(val)) return 0;

        return val;
    }

    double acceleration(std::vector<double>&v){
        if(v.size()<10) return 0;

        int n=v.size();

        double m1 = log((v[n-1]+1e-6)/(v[n-5]+1e-6));
        double m2 = log((v[n-5]+1e-6)/(v[n-10]+1e-6));

        if(!valid(m1)||!valid(m2)) return 0;

        return m1 - m2;
    }

    void run(){
        auto oracle=col(0);
        auto capex=col(1);
        auto nasdaq=col(2);
        auto europe=col(3);
        auto oil=col(4);

        double c1=corr(oracle,nasdaq);
        double c2=corr(capex,nasdaq);

        double mTech=momentum(oracle);
        double mCapex=momentum(capex);
        double mEurope=momentum(europe);
        double mOil=-momentum(oil);

        double aTech=acceleration(oracle);
        double aCapex=acceleration(capex);
        double aOil=acceleration(oil);

        double alpha =
            0.30*c2+
            0.20*c1+
            0.15*mCapex+
            0.10*mTech+
            0.15*mEurope+
            0.10*mOil;

        double conf = std::min(1.0, fabs(alpha));

        // ===== OUTPUT =====
        std::cout<<"\n=== MACRO ENGINE v5 ===\n";

        std::cout<<"Alpha: "<<alpha<<"\n";
        std::cout<<"Confidence: "<<conf*100<<"%\n";

        if(alpha>0.8) std::cout<<"🚀 STRONG BULL\n";
        else if(alpha>0.5) std::cout<<"📈 GROWTH\n";
        else if(alpha>0.2) std::cout<<"⚖️ ROTATION\n";
        else std::cout<<"⚠️ RISK OFF\n";

        if(mCapex > 1.0 && mTech < 0)
            std::cout<<"➡️ TECH → INDUSTRIALS\n";

        if(mOil > 0.5)
            std::cout<<"⛽ ENERGY DOMINANCE\n";

        // ===== ACCELERATION =====
        std::cout<<"\n=== ACCELERATION ===\n";
        std::cout<<"Tech accel : "<<aTech<<"\n";
        std::cout<<"Capex accel: "<<aCapex<<"\n";
        std::cout<<"Oil accel  : "<<aOil<<"\n";

        if(aCapex > 0)
            std::cout<<"📈 Capex accelerating\n";

        if(aTech < 0)
            std::cout<<"📉 Tech weakening\n";

        if(aOil > 0)
            std::cout<<"🔥 Oil strengthening\n";

        // ===== POSITION =====
        double size = std::min(1.0, fabs(alpha));

        std::cout<<"\n=== POSITION ===\n";

        if(alpha > 0.5)
            std::cout<<"📈 LONG RISK | "<<size*100<<"%\n";
        else if(alpha > 0.2)
            std::cout<<"🔄 PARTIAL LONG | "<<size*50<<"%\n";
        else if(alpha > 0)
            std::cout<<"⚖️ LIGHT EXPOSURE | "<<size*30<<"%\n";
        else
            std::cout<<"🛑 DEFENSIVE / SHORT | "<<size*100<<"%\n";

        // ===== PLAYBOOK =====
        std::cout<<"\n=== PLAYBOOK ===\n";

        if(mCapex > 1.0 && mTech < 0){
            std::cout<<"→ Long Industrials / Defense\n";
            std::cout<<"→ Reduce Tech\n";
        }

        if(mOil > 0.5)
            std::cout<<"→ Long Energy\n";

        if(mTech < -0.5)
            std::cout<<"→ Avoid SaaS / Cloud\n";
    }
};

int main(){
    Engine e;
    e.load("macro_data_v2.csv");
    e.run();
}
