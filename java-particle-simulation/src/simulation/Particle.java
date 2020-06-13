package simulation;

import java.awt.Color;
import java.awt.Shape;
import java.awt.geom.Ellipse2D;

public class Particle{

    private static       int count = 0;
    public  static final int BIG   = 30;
    
    private final int    diam;
    private final int    mass;
    private final Color  color;
    private double       x;
    private double       y;
    private Vector       v;
    private int          hits;
    private int          id;

    public Particle(int x, int y, double vx, double vy, int diam, int mass, 
            Color c) {
        this.x    = x;
        this.y    = y;
        v         = new Vector(vx, vy);
        this.diam = diam;
        this.mass = mass;
        color     = c;
        hits      = 0;
        id        = count++;
    }

    @Override
    public String toString() {
        return Integer.toString(id);
    }

    public Color colour() {
        return color;
    }

    public void move(double dt) {
        x += v.x() * dt;
        y += v.y() * dt;
    }

    public int collisions() {
        return hits;
    }
    
    public Shape represent() {
        return new Ellipse2D.Double(x - diam / 2, y - diam / 2, diam, diam);
    }

    /*
     * In a collision between p1 and p2, the distance between their positions
     * will be p1.r + p2.r.
     * The position of Particle p at time (now+t) is 
     * (p.x + t*p.vx, p.y + t*p.vy).
     * So, the SQUARE of the distance between p1 and p2 at (now+t) is
     * (p1.x + t*p1.vx - p2.x - t*p2.vx)^2 
     *                     + (p1.y + t*p1.vy - p2.y - t*p2.vy)^2
     * which gives us a quadratic equation in t.
     * We solve this using the quadratic formula:
     *             t = (-b +or- sqrt(b^2 - 4ac))/2a
     * where b^2 - 4ac is the "discriminant".
     * If the equation has real roots (discriminant >= 0) then we want
     * the smallest solution for t, assuming it is positive.
     * If some solution for t is negative, then the particles have already
     * collided and are moving apart.
     * Since the quadratic coefficient is a sum of squares:
     * a = (p1.dx - p2.dx)^2 + (p1.dy - p2.dy)^2 it cannot be negative.
     * So, the smaller root must be the one where sqrt(D) is subtracted.
     */
    public static Collision getCollision(Particle p1, Particle p2, double now) {
        if (p1 == p2)  { return null; }
        double xDiff  = p1.x - p2.x;
        double yDiff  = p1.y - p2.y;
        double vxDiff = p1.v.x() - p2.v.x(); 
        double vyDiff = p1.v.y() - p2.v.y();
        double sep    = (double)p1.diam / 2 + (double)p2.diam / 2;
        
        double a      = vxDiff * vxDiff + vyDiff * vyDiff;
        double b      = 2 * xDiff * vxDiff + 2 * yDiff * vyDiff;
        double c      = xDiff * xDiff + yDiff * yDiff - sep * sep;
        double det    = b * b - 4 * a * c;
        if (det < 0)   { return null; }
        
        double rootDet = Math.sqrt(det);
        double t       = (b * (-1) - rootDet) / (2 * a);
        if (t > 0) {
            return new TwoParticleCollision(p1, p2, now + t);
        }
        return null;
    }

    public static Collision getCollision(Particle p, Wall w, double now) {
        Collision c = null;
        double t = 0;
        switch(w.direction()) {
        case NORTH:
            if (p.v.y() < 0) {
                t = (w.location() + p.diam / 2 - p.y) / p.v.y();
            }
            break;
        case SOUTH:
            if (p.v.y() > 0) {
                t = (w.location() - p.diam / 2 - p.y) / p.v.y();
            }
            break;
        case WEST:
            if (p.v.x() < 0) {
                t = (w.location() + p.diam / 2 - p.x) / p.v.x();
            }
            break;
        case EAST:
            if (p.v.x() > 0) {
                t = (w.location() - p.diam / 2 - p.x) / p.v.x();
            }
            break;
        }
    
        if (t > 0) {
            c = new ParticleWallCollision(p, w, now + t);
        }
        
        return c;
    }

    /**
     * Updates two colliding particles from their state immediately prior to 
     * collision to their state immediately after. In particular, the velocities
     * of the particles are updated.
     */
    public static void collide(Particle p1, Particle p2) {
        // find unit normal and unit tangent vectors
        double xDiff = p2.x - p1.x;
        double yDiff = p2.y - p1.y;
        double sep = Math.sqrt(xDiff * xDiff + yDiff * yDiff);
        Vector unitNormal = new Vector(xDiff, yDiff).div(sep);
        Vector unitTangent = new Vector(unitNormal.y() * -1, unitNormal.x());
        
        // translate particle velocities into normal/tangent reference
        double magV1Normal  = p1.v.dot(unitNormal);
        double magV1Tangent = p1.v.dot(unitTangent);
        double magV2Normal  = p2.v.dot(unitNormal);
        double magV2Tangent = p2.v.dot(unitTangent);
        
        // find scalar normal velocities after the collision 
        // (tangential remain same)
        double newMagV1Normal = 
                (magV1Normal * (p1.mass - p2.mass) + 2 * p2.mass * magV2Normal)
                / (p1.mass + p2.mass);
        double newMagV2Normal = 
                (magV2Normal * (p2.mass - p1.mass) + 2 * p1.mass * magV1Normal)
                / (p1.mass + p2.mass);
        
        // convert scalar velocities into vectors
        Vector newV1Normal  = unitNormal.mult(newMagV1Normal);
        Vector newV1Tangent = unitTangent.mult(magV1Tangent);
        Vector newV2Normal  = unitNormal.mult(newMagV2Normal);
        Vector newV2Tangent = unitTangent.mult(magV2Tangent);
        
        // convert normal, tangential vectors into x,y vectors
        p1.v = newV1Normal.plus(newV1Tangent);
        p2.v = newV2Normal.plus(newV2Tangent);
        p1.hits++;
        p2.hits++;
    }

    /**
     * Updates Particle p from its state immediately before colliding with Wall
     * w to its state immediately post the collision.
     */
    public static void collide(Particle p, Wall w) {
        Vector v = null;
        switch(w.direction()) {
        case NORTH:
        case SOUTH:
            v = new Vector(p.v.x(), p.v.y() * -1);
            break;
        case EAST:
        case WEST:
            v = new Vector(p.v.x() * -1, p.v.y());
            break;
        }
        p.v = v;
        p.hits++;
    }

}


