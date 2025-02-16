#ifndef HELPER_H

#include<string>
#include<iostream>

class Helper{
    public:
        void logging(std::string message){
            std::cout<<message<<std::endl;
        }
};

#endif