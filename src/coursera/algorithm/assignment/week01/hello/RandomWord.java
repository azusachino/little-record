import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

public class RandomWord {
    public static void main(String[] args) {
        int count = 0;
        String chap = "";
        while (!StdIn.isEmpty()) {
            String value = StdIn.readString();
            count++;
            if (StdRandom.bernoulli((double) 1 / count)) {
                chap = value;
            }
        }
        StdOut.print(chap);
    }
}
