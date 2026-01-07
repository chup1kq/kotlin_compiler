package JavaRTL;

public class Char extends Any {

    public static final char MIN_VALUE = '\u0000';
    public static final char MAX_VALUE = '\uffff';

    private char value;

    public char getValue() {
        return value;
    }

    public Char(char value) {
        this.value = value;
    }

    public Char(int value) {
        this.value = (char)value;
    }

    public Char plus(Char o) { return new Char(value + o.value); }
    public Char minus(Char o) { return new Char(value - o.value); }

    public Char toChar() { return this; }

    public Int toInt() { return new Int((int) value); }
    public Float toFloat() { return new Float((float) value); }
    public Double toDouble() { return new Double((double) value); }

    public Boolean equals(Char o) { return new Boolean(value == o.value); }
    public Boolean notEquals(Char o) { return new Boolean(value != o.value); }
    public Boolean greater(Char o) { return new Boolean(value > o.value); }
    public Boolean less(Char o) { return new Boolean(value < o.value); }
    public Boolean greaterEquals(Char o) { return new Boolean(value >= o.value); }
    public Boolean lessEquals(Char o) { return new Boolean(value <= o.value); }
}
