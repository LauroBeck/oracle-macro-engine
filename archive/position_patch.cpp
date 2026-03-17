void position(double alpha){
    double size = std::min(1.0, fabs(alpha));

    if(alpha > 0.5)
        std::cout<<"📈 LONG RISK | Size: "<<size*100<<"%\n";

    else if(alpha > 0.2)
        std::cout<<"🔄 ROTATION MODE | Partial Long: "<<size*50<<"%\n";

    else if(alpha > 0.0)
        std::cout<<"⚖️ NEUTRAL | Small Exposure: "<<size*30<<"%\n";

    else
        std::cout<<"🛑 SHORT / DEFENSIVE | Size: "<<size*100<<"%\n";
}
