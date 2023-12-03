#include "GeneticAlgorithm.h"

using namespace std;

int main() {
    point x(-5.12, 5.12, 0.00001, 10);
    cout << setprecision(5) << fixed;
    //cout << x.length;
    //cout << geneticAlgorithm(30, fitnessFunctionDiv, Dejong, selectGreedy, unifromMutate, sortParentSelection, nPointCross, evaluate, 1.0 / 600, 0.7, 0.1, 5, -5.12, 5.12, 0.00001) << '\n';
    //cout << geneticAlgorithm(30, fitnessFunctionDiv, Rastrigin, selectGreedy, unifromMutate, sortParentSelection, nPointCross, evaluate, 1.0 / 600, 1, 0.1, 5, -5.12, 5.12, 0.00001) << '\n';//pm = 0.3
    //cout << geneticAlgorithm(30, fitnessFunctionMin, Michalewicz, selectGreedy, unifromMutate, sortParentSelection, nPointCross, evaluate, 1.0 / 570, 1, 0.1, 30, 0, M_PI, 0.00001) << '\n';//pm = 0.2
    //cout << geneticAlgorithm(30, fitnessFunctionMin, Schwefel, selectGreedy, unifromMutate, sortParentSelection, nPointCross, evaluate, 1.0 / 810, 1, 0.1, 50, -500, 500, 0.00001) << '\n';
    PrintGenetic(5, fitnessFunctionMin, "Schwefel", Schwefel, selectGreedy, unifromMutate, sortParentSelection, nPointCross, evaluate, 1.0 / 135, 1, 0.1, 50, -500, 500, 0.00001);
    PrintGenetic(10, fitnessFunctionMin, "Schwefel", Schwefel, selectGreedy, unifromMutate, sortParentSelection, nPointCross, evaluate, 1.0 / 270, 1, 0.1, 50, -500, 500, 0.00001);
    PrintGenetic(30, fitnessFunctionMin, "Schwefel", Schwefel, selectGreedy, unifromMutate, sortParentSelection, nPointCross, evaluate, 1.0 / 810, 1, 0.1, 50, -500, 500, 0.00001);
    //PrintGenetic(5, fitnessFunctionMin, "Michalewicz", Michalewicz, selectGreedy, unifromMutate, sortParentSelection, nPointCross, evaluate, 1.0 / 95, 1, 0.1, 20, 0, M_PI, 0.00001);
    return 0;
}
