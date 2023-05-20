# Median and Weighted Mean Filter (MWMF)

Median and Weighted Mean Filter module with FIFO interfaces.

## Execution
1. Login NTHUEE server and source the tool
2. clone the project
> $ git clone https://github.com/Thuniinae/ELS_hw03.git
2. move into the directory
> $ cd ELS_hw03
3. execute the code

    &emsp;There are three branchs in total, which are main, coding, optimize, representing problem 1, 2, and 3.

    1. Median and mean filters with TLM interface
    > $ git checkout main  
    $ cd stratus  
    $ make sim_V_BASIC 
    
    2. Median and mean filters with quantum keeper
    > $ git checkout coding  
    $ cd stratus  
    $ make sim_V_BASIC 
    
    3.  Median and mean filters with TLM interconnect
    > $ git checkout optimize  
    $ cd stratus  
    $ make sim_V_BASIC  

    All of the output should be output.bmp which is MWMF result of lena_std_short.bmp
