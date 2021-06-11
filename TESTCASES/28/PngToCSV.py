from PIL import Image
import csv
import os.path

path = R"C:\Users\Andra\OneDrive - University of Greenwich\PhD\Application\ObjectController\TESTCASES\28"
file = "maze.png"
spacing = 0.5

img = Image.open(os.path.join(path,file))
pixels = img.getdata()

row_length = img.width

p_array = []
for p in range(len(pixels)):
    if p % row_length == 0:
        p_array.append([])
    
    if pixels[p][0] == 0:
        p_array[-1].append(False)
    else:
        p_array[-1].append(True)
img.close()

csv_out = [["N"]]
id = 0
for x in range(len(p_array)):
    for y in range(len(p_array[x])):
        if p_array[x][y]:
            csv_out.append([y*spacing,x*spacing,id,1,-1,-1])
            id += 1
csv_out.append(["A"])
csv_out.append(["B"])
csv_out.append(["S"])
csv_out.append(["L"])
csv_out.append(["C"])
csv_out.append([22.5394,-1.23478,-0.371155])

with open(os.path.join(path,'geo.csv'), 'w', newline='') as csvfile:
    writer = csv.writer(csvfile, delimiter=',')
    writer.writerows(csv_out)
