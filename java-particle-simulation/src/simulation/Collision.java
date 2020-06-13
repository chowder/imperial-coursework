package simulation;

public abstract class Collision extends AbstractEvent{

    Particle[] particles;
    int[] numCollisions;
    
    /**
     * Constructor for Collision
     */
    public Collision(double t, Particle[] ps) {
        super(t);
        particles = ps;
        numCollisions = new int[particles.length];
        fetchCollisions();
    }

    /**
     * Returns true if this Collision is (still) valid.
     */
    @Override
    public boolean isValid() {
        int i = 0;
        for (Particle p: particles) {
            if (numCollisions[i++] != p.collisions()) {
                return false;
            }
        }
        return true;
    }

    /**
     * Returns an array containing the Particles involved in this Collision.
     */
    public Particle[] getParticles() {
        return particles.clone();
    }


    private void fetchCollisions() {
        int i = 0;
        for (Particle p: particles) {
            numCollisions[i++] = p.collisions();
        }
    }
}
