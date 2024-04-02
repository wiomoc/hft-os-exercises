import java.util.Arrays;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.function.Consumer;

public class Sort {
    static int n = 50_000_000;
    static int n_threads = 8;

    public static void main(String[] args) {
        if (args.length >= 1) {
            n = Integer.parseInt(args[0]);
        }
        if (args.length >= 2) {
            n_threads = Integer.parseInt(args[1]);
        }
        int data[] = new int[n];
        for (int i = 0; i < n; i++) {
            data[i] = (int) (Math.random() * 1_000_000_000.0);
        }
        System.out.println("Data generated");

        HashMap<String, Long> durations = new HashMap<String, Long>();

        timeSort(durations, data, "Arrays::sort", Arrays::sort);
        timeSort(durations, data, "Arrays::parallelSort", Arrays::parallelSort);
        timeSort(durations, data, "multiThreadedSort", Sort::multiThreadedSort);
        timeSort(durations, data, "forkJoinSortFull", Sort::forkJoinSortFull);
        timeSort(durations, data, "forkJoinSort1000", Sort::forkJoinSort1000);

        for (Entry<String, Long> e : durations.entrySet()) {
            System.out.println(e.getKey() + " => " + e.getValue());
        }
    }

    private static void multiThreadedSort(int[] data) {
        // TODO multi threaded sort
    }

    private static void forkJoinSortFull(int[] data) {
        // TODO fork join sort recursive with threshold 2
    }

    private static void forkJoinSort1000(int[] data) {
        // TODO fork join sort recursive with threshold 1000
    }

    private static void timeSort(HashMap<String, Long> durations, int[] data_not_change, String label,
            Consumer<int[]> sortFunction) {
        int data[] = Arrays.copyOf(data_not_change, data_not_change.length);
       
        long start = System.currentTimeMillis();
        sortFunction.accept(data);
        long duration = System.currentTimeMillis() - start;

        for (int i = 0; i < data.length - 1; i++) {
            if (data[i] > data[i + 1]) {
                System.err.println("Function " + label + " did not sort correctly");
                durations.put(label, (Long) Long.valueOf(-1));
                return;
            }
        }
        durations.put(label, Long.valueOf(duration));
    }
}
