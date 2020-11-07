# from random import randint

def bubble_sort(arr, n):  # n is the number of elements in the array arr
    if n < 0 or n == 0:
       print("in bubble_sort(arr, n), n should be positive")
       return 
    i = 1 # 1 ... n-1   n-1 iterations. 
    already_sorted = 0 # 0 means false
    while (i < n) and (already_sorted == 0) : 
    # for i in range(n):
    # Create a flag that will allow the function to
         # terminate early if there's nothing left to sort
        j = 0
        no_swap = 1
        while j < n - i :  
            if arr[j] > arr[j + 1]:
                # If the item you're looking at is greater than its
                # adjacent value, then swap them
                temp = arr[j]
                arr[j] = arr[j+1]
                arr[j+1] = temp
                # array[j], array[j + 1] = array[j + 1], array[j]
                no_swap = 0
            j=j+1
        if not (no_swap == 1):
            already_sorted = 1
        i = i + 1
    return arr


def print_num_array(arr, n):
    j = 0
    while j<n-1:
        print(arr[j], end=', ') # print without ending with '\n'. 
        j=j+1
    print(arr[j]) # print the last element followed by newline
    

#ARRAY_LENGTH = 10
#array = [randint(0, 1000) for i in range(ARRAY_LENGTH)]

someArray = [1, 5, 123, 24, 789, 232, 79, 89, 567, 678]

#print("type of array is ")
#print(type(array))
print("the array is ")
print_num_array(someArray, 10)
print("now sort the array")
bubble_sort(someArray, 10)
print("the array is ")
# print(someArray)
print_num_array(someArray, 10)