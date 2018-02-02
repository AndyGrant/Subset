
from itertools import combinations

def getInvalidTriplets(n):
	
	invalid = []
	for (a,b,c) in combinations(range(1,n+1), 3):
		r = int((a*b*c) ** .5)
		if r * r == a * b * c:
			invalid.append((a,b,c))
	return invalid
	
def findMostCommonElement(triplets):

	single = []
	for a, b, c in triplets:
		single.extend([a, b, c])
		
	freq = [0 for f in range(max(single) + 1)]
	for item in single: freq[item] += 1
	
	times = max(freq)
	
	for f in range(len(freq)-1,-1,-1):
		if freq[f] == times: return f
		
def removeAllWithX(triplets, x):

	pruned = []
	for triplet in triplets:
		if x not in triplet:
			pruned.append(triplet)
	return pruned
	
start = getInvalidTriplets(21)
print start

total = 0
while len(start) != 0:
	most = findMostCommonElement(start)
	print most
	total += 1
	start = removeAllWithX(start, most)
	
print "\n\n",total

for f in getInvalidTriplets(22):
	if 22 in f: print f
	
	
	