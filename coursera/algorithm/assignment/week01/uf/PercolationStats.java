import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdStats;

public class PercolationStats {
    private final int trials;
    private final double[] results;
    private double mean;

    // perform independent trials on an n-by-n grid
    public PercolationStats(int n, int trials) {
        if (n <= 0 || trials <= 0) {
            throw new IllegalArgumentException();
        }
        this.trials = trials;
        this.results = new double[trials];
        this.mean = 0.0;
        this.run(n);
    }

    // sample mean of percolation threshold
    public double mean() {
        this.mean = StdStats.mean(this.results);
        return this.mean;
    }

    // sample standard deviation of percolation threshold
    public double stddev() {
        return StdStats.stddev(this.results);
    }

    // low endpoint of 95% confidence interval
    public double confidenceLo() {
        return this.mean - (1.96 * stddev() / Math.sqrt(this.trials));
    }

    // high endpoint of 95% confidence interval
    public double confidenceHi() {
        return this.mean + (1.96 * stddev() / Math.sqrt(this.trials));
    }

    private void run(int n) {
        for (int i = 0; i < this.trials; i++) {
            Percolation p = new Percolation(n);
            while (!p.percolates()) {
                int r = StdRandom.uniform(n) + 1;
                int c = StdRandom.uniform(n) + 1;
                p.open(r, c);
            }
            this.results[i] = (double) p.numberOfOpenSites() / (n * n);
        }
    }

    // test client (see below)
    public static void main(String[] args) {
        PercolationStats ps = new PercolationStats(200, 100);
        System.out.println("mean    = " + ps.mean());
        System.out.println("stddev  = " + ps.stddev());
        System.out.println("92%  = [" + ps.confidenceLo() + "," + ps.confidenceHi() + "]");
    }
}
