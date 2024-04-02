import java.util.Random;
import java.util.concurrent.CountDownLatch;

public class NOutOfMSolutionCDL {
    private final static int M = 10;
    private final static int N = 5;


    public static void main(String[] args) throws InterruptedException {
        CountDownLatch counter = new CountDownLatch(N);
        for (int m = 0; m < M; m++) {
            final int mCopy = m;
            Thread t = new Thread(() -> {
                int sleepDurationMilliseconds = new Random().nextInt(10000);
                System.out.println("Thread " + mCopy + " started and going to sleep for " + sleepDurationMilliseconds + " ms");
                try {
                    Thread.sleep(sleepDurationMilliseconds);
                    System.out.println("Thread " + mCopy + " end his sleep");
                    counter.countDown();
                } catch (InterruptedException e) {
                }

            });
            t.start();
        }
        counter.await();
        System.exit(0);
    }
}
