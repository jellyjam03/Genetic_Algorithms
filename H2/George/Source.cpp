#include "GeneticAlgorithm.h"

using namespace std;

int main() {
    point x(-500, 500, 0.00001, 30);
    cout << setprecision(5) << fixed;
    //cout << x.length;
    cout << geneticAlgorithm(30, fitnessFunction, Dejong, selectWheelOfFortune, unifromMutate, sortParentSelection, twoPointCross, evaluate, 1.0 / 600, 0.7, 0.05, -5.12, 5.12, 0.00001);
    //cout << geneticAlgorithm(30, fitnessFunction, Rastrigin, selectWheelOfFortune, unifromMutate, sortParentSelection, uniformCross, evaluate, 1.0 / 600, 0.7, 0.3, -5.12, 5.12, 0.00001);
    //cout << geneticAlgorithm(30, fitnessFunction, Michalewicz, selectWheelOfFortune, unifromMutate, sortParentSelection, uniformCross, evaluate, 1.0 / 570, 0.7, 0.2, 0, M_PI, 0.00001);
    //cout << geneticAlgorithm(30, fitnessFunction, Schwefel, selectWheelOfFortune, unifromMutate, sortParentSelection, twoPointCross, evaluate, 5.0 / 810, 1, 0.7, -500, 500, 0.00001);
    //Print(30, 30, hillClimbing, FirstImprovement, "First Improvement", Michalewicz, "Michalewicz", 0, M_PI, 0.00001);
    /*int dim[] = {5, 10 , 30};
    for (int i = 0; i < 3; i++) {
        cout << dim[i] << '\n';
        PrintAnneal(30, dim[i], Dejong, "Dejong", -5.12, 5.12, 0.00001);
        cout << dim[i] << '\n';
        PrintAnneal(30, dim[i], Rastrigin, "Rastrigin", -5.12, 5.12, 0.00001);
        cout << dim[i] << '\n';
        PrintAnneal(30, dim[i], Schwefel, "Schwefel", -500, 500, 0.00001);
        cout << dim[i] << '\n';
        PrintAnneal(30, dim[i], Michalewicz, "Michalewicz", 0, M_PI, 0.00001);
    }
    fout.close();*/
    //cout << setprecision(5) << fixed;
    //cout << hillClimbing(30, Dejong, -5.12, 5.12, 0.00001, BestImprovement) << '\n';
    //cout<<hillClimbing(30, Rastrigin, -5.12, 5.12, 0.00001, BestImprovement)<<'\n';
    //cout<<hillClimbing(30, Michalewicz, 0, M_PI, 0.00001, BestImprovement)<<'\n';
    //cout<<simulatedAnnealing(30, Dejong, -5.12, 5.12, 0.00001)<<'\n';
    //cout<<simulatedAnnealing(30, Schwefel, -500, 500, 0.00001)<<'\n';
    //cout<<simulatedAnnealing(30, Rastrigin, -5.12, 5.12, 0.00001)<<'\n';
    //cout<<simulatedAnnealing(30, Michalewicz, 0, M_PI, 0.00001)<<'\n';

    return 0;
}
