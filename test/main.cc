//
// Created by oldlonecoder on 6/18/22.
//

#include "test.h"


int main(int argc, char** argv)
{
    Lsc::Test Test(argc,argv);
    try
    {
        auto C = Test.execute();
        if(!C)
            std::cout << " Test execution failed, see the incoming messages\n";
        
        std::cout << "Flushing Message::List:\n--------------------------------------------------------\n";
        Lsc::Message::Clear( [](Lsc::Message& M) { std::cout << M() << '\n'; } );
        std::cout << "Flushing Message::List:\n----------------END-------------------------------------\n";
        return !(*C == Lsc::Message::Code::Accepted);        
    }

    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch(Lsc::Message M)
    {
        std::cout << " Aborted because of an exception occured:\n";
        Lsc::Message::Clear( [](Lsc::Message& M) { std::cout << M() << '\n'; } );
    }
    
}
