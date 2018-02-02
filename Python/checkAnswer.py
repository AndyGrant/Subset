

N = 1000

with open("1000.txt", "r") as fin:
	data = fin.readlines()
	
foo = []
for f in range(254):
	foo.append(int(data[f]))
	
print foo

rng = [f for f in range(1, 1000+1) if f not in foo]

print rng
print len(rng)

for a in range(len(rng)):
	for b in range(a+1, len(rng)):
		for c in range(b+1, len(rng)):
			product = rng[a] * rng[b] * rng[c]
			roundedroot = (int)(product ** .5)
			if product * product == roundedroot:
				print "Err ",rng[a],rng[b],rng[c]