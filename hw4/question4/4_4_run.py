import subprocess

import math

W = [1,2,3,4,5,6,7,8]


def P_1d(W):
    return math.ceil(math.sqrt(W))


# THis is an approximation method that solves for the closes value of P given that W, as I was not able to directly solve this: p*(log(p))^{2} = W


def P_2d(W):

    best_p = 1
    min_diff = float("inf")
    #range chosen so that the least highest value of p that can give atleast max W
    for val in range(1, 17):
        
        approximate = 0
        if val == 1:
            approximate = 1.0
        else:
            approximate = val * (math.log2(val) ** 2)

        diff = abs(approximate - W)

        if diff < min_diff:
                min_diff = diff
                best_p = val
    return best_p


for w in W:
    w = w**2

    p1 = P_1d(w)
    p2 = P_2d(w)
    print(f"w={w}")
    print("Serial 1d: ")

    subprocess.run(["mpiexec", "-np", "1", "./4.4", str(w), "1"], check=True)

    print("Parallel 1d: ")
    subprocess.run(["mpiexec", "-np", str(p1), "./4.4", str(w), "1"], check=True)

    print("Serial 2d: ")

    subprocess.run(["mpiexec", "-np", "1", "./4.4", str(w), "0"], check=True)

    print("Parallel 2d: ")
    subprocess.run(["mpiexec", "-np", str(p2), "./4.4", str(w), "0"], check=True)


