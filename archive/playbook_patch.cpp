void playbook(double mTech, double mCapex, double mOil){
    std::cout<<"\n=== PLAYBOOK ===\n";

    if(mCapex > 1.0 && mTech < 0){
        std::cout<<"→ Long Industrials / Defense\n";
        std::cout<<"→ Reduce Big Tech exposure\n";
    }

    if(mOil > 0.5){
        std::cout<<"→ Long Energy (Oil majors)\n";
    }

    if(mTech < -0.5){
        std::cout<<"→ Avoid SaaS / Cloud overvaluation\n";
    }
}
