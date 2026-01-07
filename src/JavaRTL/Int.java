package JavaRTL;

public class Int extends Any {

    public static final int MIN_VALUE = -2_147_483_648;

    public static final int MAX_VALUE = 2_147_483_647;

    private int value;

    public int getValue() {
        return value;
    }

    public Int(int value) {
        this.value = value;
    }

    public Int toInt() {
        return this;
    }

    public Int plus(Int o) { return new Int(value + o.value); }
    public Int minus(Int o) { return new Int(value - o.value); }
    public Int times(Int o) { return new Int(value * o.value); }
    public Int div(Int o) { return new Int(value / o.value); }
    public Int rem(Int o) { return new Int(value % o.value); }

    public Boolean equals(Int o) { return new Boolean(value == o.value); }
    public Boolean notEquals(Int o) { return new Boolean(value != o.value); }
    public Boolean greater(Int o) { return new Boolean(value > o.value); }
    public Boolean less(Int o) { return new Boolean(value < o.value); }
    public Boolean greaterEquals(Int o) { return new Boolean(value >= o.value); }
    public Boolean lessEquals(Int o) { return new Boolean(value <= o.value); }

    public Float toFloat() { return new Float((float) value); }
    public Double toDouble() { return new Double((double) value); }
    public Char toChar() { return new Char((char) value); }

    public Int uMinus() { return new Int(-value); }
    public Int uPlus() { return new Int(value); }

    public Int[] rangeTo(Int o) {
        int start = value;
        int end = o.value;
        int len = Math.abs(end - start) + 1;

        if (len <= 0) return new Int[0];

        Int[] res = new Int[len];
        if (end >= start) {
            for (int i = 0; i < len; i++) {
                res[i] = new Int(start + i);
            }
        } else {
            for (int i = 0; i < len; i++) {
                res[i] = new Int(start - i);
            }
        }
        return res;
    }
}
