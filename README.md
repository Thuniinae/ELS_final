# K-means Segmentation

K-means Segmentation in systemc and Stratus HLS.

## Execution
1. Login NTHUEE server and source the tool
2. clone the project
> $ git clone https://github.com/Thuniinae/ELS_final.git
2. move into the directory
> $ cd ELS_final
3. execute the code

    &emsp;There are two branchs in total, which are main, optimize, representing the original design and optimize design in HLS.

    1. Original
    > $ git checkout main  
    $ cd stratus  
    $ make sim_V_BASIC 
    
    2. Optimize design
    > $ git checkout optimize
    $ cd stratus  
    $ make sim_V_BASIC 

    All of the output should be lena_color_512_seg.bmp which is K-means segmentation result of lena_color_512.bmp
