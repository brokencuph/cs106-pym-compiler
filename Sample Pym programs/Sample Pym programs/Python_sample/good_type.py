# int arr[20];
## there is no way to implement address + integer like C using list of Python.
import sys

arr = [0]*20
print("id(arr[0]) = " + str(id(arr[0])) )
print("id(arr[1]) = " + str(id(arr[1])) )
print("id(arr[2]) = " + str(id(arr[2])) )
print("id(5) = " + str(id(5)) )
print("id(88) = " + str(id(88)) )
arr[0] = 1
arr[1] = 2
arr[2] = 3
arr[3] = 4
arr[4] = 88
print("after assigning differnet values to arr[0] to arr[4]")
print("id(arr[0]) = " + str(id(arr[0])) )
print("id(arr[1]) = " + str(id(arr[1])) )
print("id(arr[2]) = " + str(id(arr[2])) )
print("id(arr[3]) = " + str(id(arr[3])) )
print("id(arr[4]) = " + str(id(arr[4])) )
print("the address difference between arr[1] and arr[0] = " + str( id(arr[1]) - id(arr[0])))
print("the address difference between arr[3] and arr[2] = " + str( id(arr[3]) - id(arr[2])))
print("the address difference between arr[4] and arr[3] = " + str( id(arr[4]) - id(arr[3])))
print("the address difference between arr[5] and arr[4] = " + str( id(arr[5]) - id(arr[4])))
print("the address difference between arr[5] and arr[18] = " + str( id(arr[5]) - id(arr[18])))
print("type of arr is " + str(type(arr)))
print("type of arr[0] is " + str(type(arr[0])))
print("sizeof arr[0] is " + str(sys.getsizeof(arr[0])) )
print("sizeof arr is " + str(  sys.getsizeof(arr)) )

# if (arr+3) < (arr+4) : 
#    (arr + ((arr+5) - (arr+2-1)))[7] = 21
# arr[0] = arr[2] = arr[5] = (4 <= 8)
