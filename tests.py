import os

x_p =[]
y_p = []
x = 1
f = open("test_case_3.txt","w")
i=0
for i in range(1,101):
	f.write("{0} {1} {2} {3}\n".format(0,i,100,i))
i=0
for i in range(1,101):
	f.write("{0} {1} {2} {3}\n".format(i,0,i,100))
f.close()

#os.system("g++ *.cpp")
#os.system("a < tc.txt")
