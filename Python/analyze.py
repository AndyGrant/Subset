

from math import sqrt

r = []
with open("1000.txt", "r") as fin:
	data = fin.readlines()
	
for f in range(254):
	r.append(int(data[f]))
	
r = list(set(r))

primes = [2]
for f in range(2, 1000):
	for x in primes:
		if f % x == 0:
			break
	else:
		primes.append(f)
	
for p in primes:
	z = p*p
	while z < 1000:
		print z, z in r
		z *= p
		
for p in primes:
	if p <= 500 and p in r:
		print p
		
for f in range(30, 1001, 30):
	print f, f in r