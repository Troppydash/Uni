package preworkshops.ten;

// must be a singleton
public class Earth {
    private static Earth instance = null;

    private double x;
    private double y;
    private double z;

    private double vx;
    private double vy;
    private double vz;

    private Earth() {
        this.x = this.y = this.z = 0.0;
        this.vx = this.vy = this.vz = 0.0;
    }

    public static Earth getInstance() {
        if (instance == null) {
            instance = new Earth();
        }

        return instance;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    public double getZ() {
        return z;
    }

    public double getVx() {
        return vx;
    }

    public double getVy() {
        return vy;
    }

    public double getVz() {
        return vz;
    }


    public static void main(String[] args) {
        Earth earth = Earth.getInstance();
        System.out.println(earth.getX());
    }
}