//Christian Weeks
//Cp Sc 817 - Physically Based Animation
//Main run loop
#include"glutManager.h"

int main(int argc, char** argv){
    try {
        glutManager manager(argc, argv);
        manager.run();

    }
    catch (const std::string& msg) { std::cout << msg << "\n"; }
    catch (...) { std::cout << "An unknown exception was thrown\n";}
    return 0;
}
