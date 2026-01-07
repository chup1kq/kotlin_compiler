package JavaRTL;

public class Double {

    public static final double MIN_VALUE = 4.9e-324;
    public static final double MAX_VALUE = 1.7976931348623157e+308;

    private double value;

    public double getValue() {
        return value;
    }

    public Double(double value) {
        this.value = value;
    }

    public Double toDouble() { return this; }

    public Double plus(Double o) { return new Double(value + o.value); }
    public Double minus(Double o) { return new Double(value - o.value); }
    public Double times(Double o) { return new Double(value * o.value); }
    public Double div(Double o) { return new Double(value / o.value); }
    public Double rem(Double o) { return new Double(value % o.value); }

    public Boolean equals(Double o) { return new Boolean(value == o.value); }
    public Boolean notEquals(Double o) { return new Boolean(value != o.value); }
    public Boolean greater(Double o) { return new Boolean(value > o.value); }
    public Boolean less(Double o) { return new Boolean(value < o.value); }
    public Boolean greaterEquals(Double o) { return new Boolean(value >= o.value); }
    public Boolean lessEquals(Double o) { return new Boolean(value <= o.value); }

    public Int toInt() { return new Int((int) value); }
    public Float toFloat() { return new Float((float) value); }
    public Char toChar() { return new Char((char) value); }

    public Double uMinus() { return new Double(-value); }
    public Double uPlus() { return new Double(value); }
}
