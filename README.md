# Code structure and run instructions

## Question 6:

    -> To run file hw2_question6.py you need these libraries installed: numpy, random, matplotlib, math
    -> To  run the hw2_q6_plot.py you need these libraries:  numpy, matplotlib, scipy

    -> Instructions to run hw2_question6.py:

        This file takes multiple arguments: 
        args_parser.add_argument("-m", "--mesh", type=str,help="Choose between 2D, 3D, Hypercube", default="2D", required=False)
        args_parser.add_argument("-p", "--processors", type=int, help="Choose the p value", default=0, required=False)
        args_parser.add_argument("-k", "--connections", type=int,dest="k", help="Choose the number of connetions for each processor", default=4, required=False)
        args_parser.add_argument("-n", "--network_mapping",  help="network_mapping",action="store_true",  required=False)
        args_parser.add_argument("-all", "--all_meshes", help="Run for all meshes for diameter and bisection width",action="store_true",required=False)
        args_parser.add_argument("-p4", "-part_4", help="part_4 code",action="store_true",required=False)


        -> For parts 6.1 and 6.2 you can just run it with the -all flag and it will give the diameters and bisection for all the cases mentioned
        -> For parts 6.3 you can just run it with the -n flag anf it will give all the congestion and Dilation values for all the cases mentioned
        -> For parts 6.4: for the first part of the question you can get the values from the above runs, but for the second part you need to run it with this command: hw2_question6.py -p4 -p 4096 -k 2

    -> Instructions to run hw2_q6_plot.py:
        -> we need to run this file to get the plots for parts 6.1, 6.2, 6.3
        -> All the values are already mentioned in this file and we just need to simply run this


