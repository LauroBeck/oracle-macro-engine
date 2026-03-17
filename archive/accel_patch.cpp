double acceleration(std::vector<double>& v){
    if(v.size() < 10) return 0;

    int n = v.size();

    double m1 = log((v[n-1]+1e-6)/(v[n-5]+1e-6));
    double m2 = log((v[n-5]+1e-6)/(v[n-10]+1e-6));

    if(!std::isfinite(m1) || !std::isfinite(m2)) return 0;

    return m1 - m2;
}
