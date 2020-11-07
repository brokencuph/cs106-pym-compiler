# Creating an array of 10 integers, with initial value 1. In C, it will be:  int arr[10] = {1}; 
# global array
arr = [1]*10

def main():
    aaa = []
    j = 0
    while j<20 :
        aaa.append(j)
        print(aaa[j])
        j=j+1
    j = 0
	# do some computation without much meaning, just to test some expressions
    print("the list aaa  will be changed ")
    while j<20 :
		#  /* aaa[j] = aaa[j] + arr[j%10] */
        print(" int(j-(j/10)*10) == j%10" + " is ", str( j-(j/10)*10 == j%10 ) ) 
        print(" j-(j/10)*10 is " + str( j-(j/10)*10))  # in python3, 2/3 is 0.666..., while in python2, 2/3 is 0
        print(" j-(j/10.0)*10 is " + str( j-(j/10.0)*10)) 
        print(" j-int((j/10))*10 is " + str( j-int((j/10))*10 )  ) 
        print(" j%10 is " + str(j%10))
        aaa[j] = aaa[j] + arr[int((j-(j/10)*10) + 0)]   
        print(aaa[j])
        j=j+1

main()