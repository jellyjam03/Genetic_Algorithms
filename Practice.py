import random
import math
import copy


def este_in_domeniu(x, domeniu):
    for i in range(len(x)):
        if x[i] < domeniu[i][0] or domeniu[i][1] < x[i]:
            return False
    return True

def minim_vecinatate_recursiv(x, f, add, poz, domeniu):
    if poz == len(x):
        return x
    punct_minim = copy.copy(x)
    for epsilon in add:
        x[poz] += epsilon
        if este_in_domeniu(x, domeniu):
            aux = minim_vecinatate_recursiv(copy.copy(x), f, add, poz + 1, domeniu)
            x[poz] -= epsilon
            if f(aux) < f(punct_minim):
                punct_minim = copy.copy(aux)
    return punct_minim


def punct_de_minim_euristica(f, epsilon, domeniu_de_definitie): #f e functia pe care o avem in vedere
    x = []
    for interval in domeniu_de_definitie:
        x.append(random.uniform(interval[0], interval[1]))
    print(x)
    punct_minim_vecinatate = minim_vecinatate_recursiv(x, f, (-epsilon, 0, epsilon), 0, domeniu_de_definitie)
    while f(punct_minim_vecinatate) < f(x):
        x = punct_minim_vecinatate
        punct_minim_vecinatate = minim_vecinatate_recursiv(x, f, (-epsilon, 0, epsilon), 0, domeniu_de_definitie)
        print(punct_minim_vecinatate)
        print(f(punct_minim_vecinatate))
    print(f(punct_minim_vecinatate))
    return punct_minim_vecinatate


def patrat(x):
    return x[0] * x[0]


def michalewicz(x):
    suma = 0
    for i in range(len(x)):
        suma += math.sin(x[i]) * (math.sin(((i+1) * x[i]**2) / math.pi)**20)
    return suma * (-1)


domeniu = []
dimensiune = 2
for i in range(dimensiune):
    domeniu.append([0, math.pi])

punct_de_minim = punct_de_minim_euristica(michalewicz, 0.00001, domeniu)

print(punct_de_minim)
#print(michalewicz((2.20, 1.57)))

