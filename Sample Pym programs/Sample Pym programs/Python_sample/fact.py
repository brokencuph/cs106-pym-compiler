# recursive factorial functions 
def fact(x:int) -> int:  # requires an integer parameger
    if x > 1:
        return x * fact(x-1)
    else: 
        return 1


print("Please input an integer n: ")
x = int(input())
if x >0 : 
    print(fact(x))
elif x == 0: 
    print("fact(0) is 1")
else :
    print("cannot compute fact(n) when n is negative")

print("Bye ")
    


