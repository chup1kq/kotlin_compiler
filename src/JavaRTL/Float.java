package JavaRTL;

public class Float extends Any {
    public static final float MIN_VALUE = 1.4e-45f;
    public static final float MAX_VALUE = 3.4028235e+38f;

    private float value;

    public float getValue() {
        return value;
    }

    public Float(float value) {
        this.value = value;
    }

    public Float toFloat() { return this; }

    public Float plus(Float o) { return new Float(value + o.value); }
    public Float minus(Float o) { return new Float(value - o.value); }
    public Float times(Float o) { return new Float(value * o.value); }
    public Float div(Float o) { return new Float(value / o.value); }
    public Float rem(Float o) { return new Float(value % o.value); }

    public Boolean equals(Float o) { return new Boolean(value == o.value); }
    public Boolean notEquals(Float o) { return new Boolean(value != o.value); }
    public Boolean greater(Float o) { return new Boolean(value > o.value); }
    public Boolean less(Float o) { return new Boolean(value < o.value); }
    public Boolean greaterEquals(Float o) { return new Boolean(value >= o.value); }
    public Boolean lessEquals(Float o) { return new Boolean(value <= o.value); }

    public Int toInt() { return new Int((int) value); }
    public Double toDouble() { return new Double((double) value); }
    public Char toChar() { return new Char((char) value); }

    public Float uMinus() { return new Float(-value); }
    public Float uPlus() { return new Float(value); }
}
