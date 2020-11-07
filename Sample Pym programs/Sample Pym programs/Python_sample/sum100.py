# computes the sum from 1 to n 
def sum1(n):
    j = 0
    result = 0
    while j != n :
      result = result + j
      j = j+1
    return  result

# ok to specify the parameter type and return type in function definition
def sum2(n:int) -> int:
    j = 0
    result = 0
    while j != n :
      result = result + j
      j = j+1
    return  result

def driver():
  print("Please input a positive integer : ")
  k = int(input())
  print("The sum from 1 to " + str(k) + " is : ")
  print(sum1(k)) # print the result in a line
  print(sum2(k))

driver()