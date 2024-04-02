import java.util.Random;

public class NOutOfMSolution {
    private final static int M = 10;
    private final static int N = 5;

    private static class Counter {
        private int value;

        Counter(int value) {
            this.value = value;
        }

        synchronized void decrement() {
            value--;
            if(value <= 0) this.notifyAll();
        }

        synchronized void waitOnFinish() {
            while (value > 0) {
                try {
                    this.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static void main(String[] args) {
        Counter counter = new Counter(N);
        for (int m = 0; m < M; m++) {
            final int mCopy = m;
            Thread t = new Thread(() -> {
                int sleepDurationMilliseconds = new Random().nextInt(10000);
                System.out.println("Thread " + mCopy + " started and going to sleep for " + sleepDurationMilliseconds + " ms");
                try {
                    Thread.sleep(sleepDurationMilliseconds);
                    System.out.println("Thread " + mCopy + " end his sleep");
                    counter.decrement();
                } catch (InterruptedException e) {
                }

            });
            t.start();
        }
        counter.waitOnFinish();
        System.exit(0);
    }
}
