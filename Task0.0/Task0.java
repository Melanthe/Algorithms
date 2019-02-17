import java.io.File;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;

public class Task0 {
    public static void main(String[] args) {

        try {

            List<Long> keys = new ArrayList<Long>();
            Long summ = new Long(0);
            Scanner scanner = new Scanner(new File("input.txt"));

            while (scanner.hasNext()) {
                keys.add(scanner.nextLong());
            }

            keys = keys.stream().distinct().collect(Collectors.toList());

            for (Long item: keys) {
                summ += item;
            }

            FileWriter fw = new FileWriter("output.txt");
            fw.write(summ.toString());

            scanner.close();
            fw.close();

        } catch (Exception e) {
            System.out.println("Error!");
        }
    }
}
