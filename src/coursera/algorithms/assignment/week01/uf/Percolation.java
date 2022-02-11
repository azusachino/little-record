import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.WeightedQuickUnionUF;

public class Percolation {

    private boolean[][] grid;
    private int length;
    private int top;
    private int bottom;
    private WeightedQuickUnionUF uf;

    public Percolation(int n) {
        if (n <= 0) {
            throw new IllegalArgumentException();
        }
        this.length = n;
        this.grid = new boolean[n][n];
        this.top = 0;
        this.bottom = n * n + 1;
        this.uf = new WeightedQuickUnionUF(n * n + 2);
    }

    public void open(int i, int j) {
        this.checkRange(i, j);
        this.grid[i - 1][j - 1] = true;

        if (i == 1) {
            this.uf.union(generateIndex(i, j), this.top);
        }
        if (i == this.length) {
            this.uf.union(generateIndex(i, j), this.bottom);
        }

        if (i > 1 && isOpen(i - 1, j)) {
            this.uf.union(this.generateIndex(i, j), this.generateIndex(i - 1, j));
        }
        if (i < this.length && isOpen(i + 1, j)) {
            this.uf.union(this.generateIndex(i, j), this.generateIndex(i + 1, j));
        }
        if (j > 1 && isOpen(i, j - 1)) {
            this.uf.union(this.generateIndex(i, j), this.generateIndex(i, j - 1));
        }
        if (j < this.length && isOpen(i, j + 1)) {
            this.uf.union(this.generateIndex(i, j), this.generateIndex(i, j + 1));
        }
    }

    public boolean isOpen(int i, int j) {
        this.checkRange(i, j);
        return grid[i - 1][j - 1];
    }

    public boolean isFull(int i, int j) {
        this.checkRange(i, j);
        return this.uf.connected(generateIndex(i, j), this.top);
    }

    public int numberOfOpenSites() {
        int count = 0;
        for (int i = 0; i < this.length; i++) {
            for (int j = 0; j < this.length; j++) {
                if (this.grid[i][j]) {
                    count++;
                }
            }
        }
        return count;
    }

    public boolean percolates() {
        return this.uf.connected(this.top, this.bottom);
    }

    private void checkRange(int p, int q) {
        if (p < 1 || p > this.length || q < 1 || q > this.length) {
            throw new IllegalArgumentException();
        }
    }

    private int generateIndex(int p, int q) {
        return (p - 1) * this.length + q;
    }

    // test client (optional)
    public static void main(String[] args) {
        int n = 20;
        Percolation p = new Percolation(n);

        while (!p.percolates()) {
            int r = StdRandom.uniform(n) + 1;
            int c = StdRandom.uniform(n) + 1;
            p.open(r, c);
        }
        System.out.println(p.numberOfOpenSites());
        double r = (double) p.numberOfOpenSites() / (n * n);
        System.out.println(r);
    }
}
