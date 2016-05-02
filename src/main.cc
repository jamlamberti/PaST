#include "main.h"

void run_model(char* model_file)
{
    ModelLoader ml;
    if (!ml.load_model(model_file))
    {
        std::cerr << "Error loading model..." << std::endl;
        return;
    }
    
    // Simulate the bechmarks
    Simulator sim(&ml);
    sim.model_benchmarks();
    sim.simulate_benchmarks();
}

int main(int argc, char** argv)
{
    #ifdef TESTING
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    #else
    //std::cout << "Hello World" << std::endl;
    ModelLoader ml;
    for (int i = 1; i < argc; i++)
    {
        std::cout << " [+] Running Model " << argv[i] << std::endl;
        run_model(argv[i]);
    }
    return 0;
    #endif

}
