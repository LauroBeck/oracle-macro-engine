#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

struct Row {
    double oracle, capex, nasdaq, europe, oil;
};

bool valid(double x){
    return std::isfinite(x);
}

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

        std::cout<<"Loaded rows: "<<data.size()<<std::endl;
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

    double compute(double &m1,double &m2,double &m3,double &m4){
        auto oracle=col(0);
        auto capex=col(1);
        auto nasdaq=col(2);
        auto europe=col(3);
        auto oil=col(4);

        double c1=corr(oracle,nasdaq);
        double c2=corr(capex,nasdaq);

        m1=momentum(oracle);
        m2=momentum(capex);
        m3=momentum(europe);
        m4=-momentum(oil);

        std::cout<<"Corr Oracle: "<<c1<<std::endl;
        std::cout<<"Corr Capex : "<<c2<<std::endl;

        std::cout<<"Momentum Oracle: "<<m1<<std::endl;
        std::cout<<"Momentum Capex : "<<m2<<std::endl;
        std::cout<<"Momentum Europe: "<<m3<<std::endl;
        std::cout<<"Momentum Oil   : "<<m4<<std::endl;

        double alpha=
            0.30*c2+
            0.20*c1+
            0.15*m2+
            0.10*m1+
            0.15*m3+
            0.10*m4;

        return alpha;
    }

    double confidence(double a){
        return std::min(1.0, std::fabs(a));
    }

    void rotation(double mTech,double mCapex,double mOil){
        if(mCapex > 1.0 && mTech < 0)
            std::cout<<"➡️ Rotation: TECH → INDUSTRIALS\n";

        if(mOil > 0.5)
            std::cout<<"⛽ Energy-led regime\n";
    }

    void alert(double a){
        if(a > 0.7)
            std::cout<<"🔥 BUY RISK\n";
        else if(a < 0.1)
            std::cout<<"🛑 DEFENSIVE\n";
    }

    void regime(double a){
        std::cout<<"\nAlpha: "<<a<<std::endl;
        std::cout<<"Confidence: "<<confidence(a)*100<<"%\n";

        if(a>0.8) std::cout<<"🚀 STRONG BULL\n";
        else if(a>0.5) std::cout<<"📈 GROWTH\n";
        else if(a>0.2) std::cout<<"⚖️ ROTATION\n";
        else std::cout<<"⚠️ RISK OFF\n";

        alert(a);
    }
};

int main(){
    Engine e;
    e.load("macro_data_v2.csv");

    double m1,m2,m3,m4;
    double alpha = e.compute(m1,m2,m3,m4);

    e.rotation(m1,m2,m4);
    e.regime(alpha);
}
