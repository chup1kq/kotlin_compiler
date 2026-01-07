package JavaRTL;

public class Unit extends Any {

    private static final Unit INSTANCE = new Unit();

    private Unit() {}

    public static Unit getInstance() { return INSTANCE; }

    public java.lang.String toString() { return "kotlin.Unit"; }
}