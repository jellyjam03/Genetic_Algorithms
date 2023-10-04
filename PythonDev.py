import math
import random

def michalewicz(x):
    
    def michalewicz1Dimension(x, i):
        return math.sin(x) * math.sin((x ** 2) * (i + 1) / math.pi) ** (2 * 10)
    
    return -sum(michalewicz1Dimension(a, i) for i, a in enumerate(x))



def rastrigin(x):
    
    def rastrigin1DimensionAux(x):
        return x ** 2 - 10 * math.cos(math.pi * x)
    
    return 10 * len(x) + sum(rastrigin1DimensionAux(a) for a in x)


def func1(x):
    
    def func1Dimension(x):
        return math.sin(x * math.pi)
    
    return sum(func1Dimension(a) for a in x)

def euristicGlobalMin(func, dimensions: int, range_start, range_end):
    best_x = [round(random.uniform(range_start, range_end), 5) for _ in range(dimensions)]
    print(best_x)
    best_value = func(best_x)
    
    SMALLEST_INCREMENT = 1e-5
    INCREMENTER = [0, -SMALLEST_INCREMENT, SMALLEST_INCREMENT]

    div_offset = [3 ** i for i in range(dimensions)] 

    while True:
        best_new_x = best_x.copy()
        best_new_value = best_value
        # generate all possible neighbours of the guessed x
        for offset in range(3 ** dimensions):
            new_x = best_x.copy()
            ok = True
            
            # generate new x permutation
            for i in range(dimensions):
                new_x[i] = round(new_x[i] + INCREMENTER[int(offset / div_offset[i] % 3)], 5)
                if new_x[i] < range_start or range_end < new_x[i]:
                    ok = False
                    break
                    
            if not ok: 
                continue         

            new_value = func(new_x)
            if new_value < best_new_value:
                best_new_value = new_value
                best_new_x = new_x
        
        if best_new_value == best_value:
            break
        
        best_x = best_new_x
        best_value = best_new_value
           
    print(f"Found the global minimum {best_value}, for x {best_x}")
    

euristicGlobalMin(func1, 2, 0, 1)