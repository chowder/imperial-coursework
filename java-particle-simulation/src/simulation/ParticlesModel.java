package simulation;

import java.awt.Dimension;
import utils.List;

public class ParticlesModel {

    public enum Direction {
        NORTH("north"), SOUTH("south"), EAST("east"), WEST("west");
        
        private String label;
        
        private Direction(String l) {
            label = l;
        }

        @Override
        public String toString() {
            return label;
        }   
    }

    private int        width;
    private int        height;
    private Particle[] particles;
    private Wall[]     walls;

    /**
     * Creates a model of size w x h, with particles ps.
     */ 
    public ParticlesModel(int w, int h, Particle[] ps) {
        width = w;
        height = h;
        particles = ps;
        walls = createWalls();
    }

    /**
     * Moves all particles for time dt.
     */
    public void moveParticles(double dt) {
        for (Particle p : particles) {
            p.move(dt);
        }
    }

    /**
     * Returns the particles that are part of this model.
     */
    public Particle[] getParticles() {
        return particles;
    }

    /**
     * Returns an iterable object containing all possible collisions that 
     * p would undergo starting 'now' if it continued along its current 
     * trajectory.
     */
    public Iterable<Collision> predictCollisions(Particle p, double now) {
        List<Collision> cs = new List<Collision>();
        addCollisions(p, now, cs);
        return cs;
    }

    /**
     * Returns an iterable object containing all possible collisions that 
     * all particles would undergo starting 'now' if they all continued along 
     * their current trajectories.
     */
    public Iterable<Collision> predictAllCollisions(double now) {
        List<Collision> cs = new List<Collision>();
        for(Particle p : particles) {
            addCollisions(p, now, cs);
        }
        return cs;
    }

    public Dimension getSize() {
        return new Dimension(width, height);
    }

    private void addCollisions(Particle p1, double now, List<Collision> cs) {
        Collision c;
        for (Particle p2 : particles) {
            c = Particle.getCollision(p1, p2, now);
            if (c != null) { cs.add(c); }
        }
        for (Wall w: walls) {
            c = Particle.getCollision(p1, w, now);
            if (c != null) { cs.add(c); }
        }        
    }
    
    private Wall[] createWalls() {
        Wall[] ws = new Wall[4];
        ws[0] = new Wall(Direction.NORTH, 0);
        ws[1] = new Wall(Direction.EAST, width);
        ws[2] = new Wall(Direction.SOUTH, height);
        ws[3] = new Wall(Direction.WEST, 0);
        return ws;
    }

}


