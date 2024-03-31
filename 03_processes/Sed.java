import java.lang.ProcessBuilder;
import java.io.PrintStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Sed {
    public static void main(String[] args) throws IOException {
        Process process = new ProcessBuilder("sed", "s/B/2/g")
                .start();
        BufferedReader inStream = new BufferedReader(new InputStreamReader(process.getInputStream()));
        PrintStream outStream = new PrintStream(process.getOutputStream());

        outStream.println("ABC");
        outStream.flush();
        outStream.close();
        System.out.println(inStream.readLine());
    }
}
