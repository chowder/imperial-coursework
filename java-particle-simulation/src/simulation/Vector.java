package simulation;

public class Vector {

    private final double[] v;

    public Vector(double v0, double v1) {
        v = new double[]{v0, v1};
    }

    public double x() {
        return v[0];
    }

    public double y() {
        return v[1];
    }

    public Vector div(double scalar) {
        return new Vector(v[0] / scalar, v[1] / scalar);
    }

    public double dot(Vector that) {
        return this.v[0] * that.v[0] + this.v[1] * that.v[1];
    }

    public Vector mult(double factor) {
        return new Vector(v[0] * factor, v[1] * factor);
    }

    public Vector plus(Vector that) {
        return new Vector(this.v[0] + that.v[0], this.v[1] + that.v[1]);
    }

}


