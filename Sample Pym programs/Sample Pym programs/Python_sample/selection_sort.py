# /* (A.3)   A program to perform selection sort on a 10 element array       */ 

# preparation:   when python is installed, easy_install is available. 
# sudo easy_install pip
# pip install numpy


import numpy as np 

array = np.random.randint(1,100,10)
print("A random array of 10 integers is created: ")
print(array)
print("The type of the array is " + str(type(array)))

def minloc(a, low, high):
    # int i; int x; int k;
    k = low
    x = a[low]
    i = low + 1
    while  i < high:
        if a[i] < x :
            x = a[i];   k = i
        i = i + 1
    return k


def sort(arr, low, high):
    #int i;  int k;
    i = low  
    while i < high-1 : 
           # int t;
           k = minloc(arr, i, high)
           t = arr[k]
           arr[k] = arr[i]
           arr[i] = t
           i = i + 1

"""
#int main(void)
# {  int i;   
    i = 0;
    while (i < 10)
        {  x[i] = read();
           i = i + 1;
        }
    sort(x, 0, 10);
    i = 0;
    while (i < 10)
      {write(x[i]);
       i = i + 1;}
    return 0;
}
"""

sort(array, 0, 10)
print("now after sorting the array, it is ")
print(array)


