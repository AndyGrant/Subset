
import java.util.*;


public class Subset {
    
    public int length;
    public int[] values;
    public BitSet excluded;
    public static boolean[][][] lookupTable;
    
    public static void main(String[] args){
        
        long startTime = System.nanoTime();
        int N = Integer.parseInt(args[0]);
        initalizeLookupTable(N);
        ArrayList<Subset> stack = new ArrayList<Subset>();
        
        // Start by searching for the largest size (N), and then
        // work our way down until we find a valid subset.
        for (int targetLength = N; targetLength > 0; targetLength--){
            
            System.out.printf("Searching for subset of size %d", targetLength);
            
            // Start with all subsets starting with the values
            // of N through 1. We go backwards because it is
            // faster to prove that sets cannot exist at a given
            // targetLength when the first value is quite large
            for (int i = N; i > 0; i--)
                stack.add(new Subset(i, N));
            
            while (!stack.isEmpty()){
                
                Subset top = stack.remove(stack.size() - 1);
                
                // In the best case we can add the values between our last
                // value and the upper bound of N. To further reduce the best
                // case we can discount the values between the last value and N
                // that we know are unable to be in the subset without forming a square
                int bestCase = top.length + N - top.values[top.length-1] - top.relevantExcluded();
                
                // This Subset is a bust. Nothing can be added
                // to it to reach a length of targetLength
                if (bestCase < targetLength)
                    continue;
                
                // A Subet of targetLength has been found. In order to get to
                // this point there must not have been a subset of length
                // targetLength + 1. Therefore, this is the best we can do
                if (top.length == targetLength){
                    System.out.printf("\nFound a Subset of size %d\n", top.length);
                    top.print();
                    exitProgram(startTime);
                }
                
                // Update the excluded values for the popped Subset
                for (int i = N; i > top.values[top.length-1]; i--)
                    if (!top.excluded.get(i) && !top.canBeAdded(i))
                        top.excluded.set(i);
                    
               // Create a new Subset for each possible value between
               // the last value of top and N inclusive, that my be 
               // added to the array. We will work backwords like before
               // for speed, constistancy, and the correctness of the seach.
               for (int i = N; i > top.values[top.length-1]; i--)
                   if (!top.excluded.get(i))
                       stack.add(new Subset(top, i, N));
                
            }
            
            System.out.printf(" -> %f\n", (float)(System.nanoTime() - startTime)/1e9);
        }
    }
    
	public static void exitProgram(long startTime){
		long endTime = System.nanoTime();
		float difference = (float)((endTime - startTime)/1e9);
		System.out.printf("\nRan to completion in %f seconds\n", difference);
		System.exit(-1);
	}
    
    /**
     * Fill a boolean table based on whether or not the indexes
     * can be multipied together to form a perfect square. This allows
     * for faster lookup later on, rather than recomputing every time.
     * This speed up is quite significant, as sqrt is quite slow.
     *
     * @param   int N       Max value that may be used in a subset
     */
    public static void initalizeLookupTable(int N){
        
        lookupTable = new boolean[N+1][N+1][N+1];
        
        for (int a = 1; a <= N; a++)
            for (int b = a + 1; b <= N; b++)
                for (int c = b + 1; c <= N; c++)
                    lookupTable[a][b][c] = isPerfectSquare(a * b * c);
    }
    
    /**
     * Check to see if X is a perfect square.
     *
     * @param   int X     Value to check
     */
    public static boolean isPerfectSquare(int x){
        int rootFloored = (int)Math.sqrt(x);
        return rootFloored * rootFloored == x;
    }
    
    
    public Subset(int value, int maxValue){
        length = 1;
        values = new int[]{value};
        excluded = new BitSet(maxValue + 1);
    }
    
    public Subset(Subset old, int value, int maxValue){
        length = 1 + old.length;
        
        values = new int[length];
        System.arraycopy(old.values, 0, values, 0, old.length);
        values[old.length] = value;
        
        excluded = new BitSet(maxValue + 1);
        excluded.or(old.excluded);
    }
    
    /**
     * Count the number of numbers that CANNOT be added to the subset
     * between our current largest (last) number, and the max value (N).
     */
    public int relevantExcluded(){
        int popcount = 0;
        for (int i = values[length-1] + 1; i < excluded.size(); i++)
            if (excluded.get(i)) popcount++;
        return popcount;
    }
    
    /**
     * Check to see if a given value may be added to our subset without
     * creating a triplet that forms a perfect square when multipied together
     *
     * @param   int value       Value to check for ability to add
     */
    public boolean canBeAdded(int value){
        for (int a = 0; a < length; a++)
            for (int b = a + 1; b < length; b++)
                if (lookupTable[values[a]][values[b]][value])
                    return false;
        return true;
    }
    
    public void print(){
        for (int i = 0; i < length; i++)
            System.out.printf("%d ", values[i]);
        System.out.printf("\n");
    }
    
}