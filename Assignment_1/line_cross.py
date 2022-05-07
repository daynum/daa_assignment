import matplotlib.pyplot as plt

line_file = open("plot_lines.txt", "r")
cross_file = open("plot_cross.txt", "r")

lines=[]
for l in line_file:
    l=l.strip();
    cords = l.split(' ')
    lines.append(cords)
line_file.close()

intersections=[]
for l in cross_file:
    l=l.strip();
    cords = l.split(' ')
    intersections.append(cords)
cross_file.close()

def find_and_plot(lines, intersections):
    plt.axis('equal')
    i=0
    for line in lines:
        x1 = float(line[0])
        x2 = float(line[1])
        y1 = float(line[2])
        y2 = float(line[3])
        plt.plot((x1, y1), (x2, y2), 'ro-')
        #label = "Line " + str(i)
        #plt.annotate(label,(min(x1,x2)+(x1+x2)/2,min(y1,y2)+(y1+y2)/2),textcoords="offset points", xytext=(0,10),ha='center')
        i = i+1
    for point in intersections:
        plt.plot(float(point[0]), float(point[1]), marker='x', markersize=10, color="blue")
    for point in intersections:
        label = "("+point[0]+", "+point[1]+")"
        plt.annotate(label,(float(point[0]),float(point[1])),textcoords="offset points", xytext=(0,10),ha='center')
find_and_plot(lines,intersections)
plt.show()