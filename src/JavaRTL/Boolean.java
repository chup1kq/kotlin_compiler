package JavaRTL;

public class Boolean extends Any {
    private boolean value;

    public Boolean(boolean value) {
        this.value = value;
    }

    public Boolean(int value) {
        this.value = value != 0;
    }

    public Boolean toBoolean() { return this; }

    public boolean getValue() {
        return value;
    }

    public Boolean equals(Boolean o) { return new Boolean(value == o.value); }
    public Boolean notEquals(Boolean o) { return new Boolean(value != o.value); }
    public Boolean not() { return new Boolean(!value); }
    public Boolean or(Boolean o) { return new Boolean(value || o.value); }
    public Boolean and(Boolean o) { return new Boolean(value && o.value); }

    public Int toInt() { return new Int(value ? 1 : 0); }
    public Float toFloat() { return new Float(value ? 1.0f : 0.0f); }
    public Double toDouble() { return new Double(value ? 1.0 : 0.0); }
}
