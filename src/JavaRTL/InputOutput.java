package JavaRTL;

import java.io.*;

public class InputOutput extends Any {

    public static Unit print(Int o) { System.out.print(o.getValue()); return Unit.getInstance(); }
    public static Unit print(Float o) { System.out.print(o.getValue()); return Unit.getInstance(); }
    public static Unit print(Double o) { System.out.print(o.getValue()); return Unit.getInstance(); }
    public static Unit print(String o) { System.out.print(o.getValue()); return Unit.getInstance(); }
    public static Unit print(Char o) { System.out.print(o.getValue()); return Unit.getInstance(); }
    public static Unit print(Boolean o) { System.out.print(o.getValue() ? "true" : "false"); return Unit.getInstance(); }

    public static Unit println(Int o) { System.out.println(o.getValue()); return Unit.getInstance(); }
    public static Unit println(Float o) { System.out.println(o.getValue()); return Unit.getInstance(); }
    public static Unit println(Double o) { System.out.println(o.getValue()); return Unit.getInstance(); }
    public static Unit println(String o) { System.out.println(o.getValue()); return Unit.getInstance(); }
    public static Unit println(Char o) { System.out.println(o.getValue()); return Unit.getInstance(); }
    public static Unit println(Boolean o) { System.out.println(o.getValue() ? "true" : "false"); return Unit.getInstance(); }

    public static Unit print() { System.out.print(""); return Unit.getInstance(); }
    public static Unit println() { System.out.println(""); return Unit.getInstance(); }

    public static String readLine() {
        try {
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            return new String(reader.readLine());
        } catch (Exception e) {
            return new String("");
        }
    }
}
