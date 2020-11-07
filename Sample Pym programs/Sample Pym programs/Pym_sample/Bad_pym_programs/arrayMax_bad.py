# return the largest item in an array, correct 
  def max(arr, size) :  # !!error!! first line should not be indented. 
    # int j;
    # int result;
    j = 0
    result = arr[j]
    while j<size : 
      if arr[j] > result :
            result = arr[j]
        j = j+1 # !!!ERRROR  when dedent, not aligned with an earlier line. 
    return result

def  driver( ) :
    arr = [ ]  # arr is an empty list. no easy way to say arr has 10 elements. 
    # int j;
    # int s;
    j = 0
    while j<5 :
        # arr[j] = input()   ## index out of range
        # # without line 14 NameError: name 'arr' is not defined
        print("please input an integer: ")
        arr.append(int(input()))  # have to use append() to add item into a list, 
                                  # without int(), a list of sting is recorded
        j = j + 1
    print("The list is : ")
    print(arr)
    print("The maximum in the list is", max(arr, 5))
    s = (arr[0]+arr[1]-arr[2])*arr[3]/arr[4]
    print("(arr[0]+arr[1]-arr[2])*arr[3]/arr[4] = ", s)
    print("Bye")
    # return 0

driver()


