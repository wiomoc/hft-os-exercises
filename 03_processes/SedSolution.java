import java.lang.ProcessBuilder;
import java.io.PrintStream;
import java.io.IOException;
import java.io.InputStream;

public class SedSolution {
    public static void main(String[] args) throws IOException {
        System.out.println(sed("s/[^ _-]*/\\u&/g", "operating systems hft stuttgart"));
    }

    public static String sed(String pattern, String input) throws IOException {
        Process process = new ProcessBuilder("sed", pattern)
                .start();
        InputStream inStream = process.getInputStream();
        PrintStream outStream = new PrintStream(process.getOutputStream());

        outStream.print(input);
        outStream.flush();
        outStream.close();
        return new String(inStream.readAllBytes());
    }
}
