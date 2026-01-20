package JavaRTL;

public class String extends Any {

    private java.lang.String value;

    public java.lang.String getValue() {
        return value;
    }

    public String(java.lang.String value) {
        this.value = value;
    }

    public Int length() { return new Int(value.length()); }
    public Int toInt() { return new Int(Integer.parseInt(value)); }
    public Float toFloat() { return new Float(Float.parseFloat(value)); }
    public Double toDouble() { return new Double(Double.parseDouble(value)); }

    public Boolean equals(String o) { return new Boolean(value.equals(o.value)); }
    public Boolean notEquals(String o) { return new Boolean(!value.equals(o.value)); }

    public Char charAt(Int index) { return new Char(value.charAt(index.getValue())); }

    public String plus(String o) { return new String(value + o.value); }
    public String plus(Char o) { return new String(value + o.getValue()); }
    public String plus(Int o) { return new String(value + o.getValue()); }
}

