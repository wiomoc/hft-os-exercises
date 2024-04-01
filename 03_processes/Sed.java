import java.lang.ProcessBuilder;
import java.io.PrintStream;
import java.io.IOException;
import java.io.InputStream;

public class Sed {
    public static void main(String[] args) throws IOException {
        System.out.println(sed("s/[^ _-]*/\\u&/g", "operating systems hft stuttgart"));
    }

    public static String sed(String pattern, String input) throws IOException {
        return "";
    }
}
