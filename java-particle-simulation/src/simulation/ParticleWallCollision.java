package simulation;

public class ParticleWallCollision extends Collision {

    private Particle particle;
    private Wall wall;

    public ParticleWallCollision(Particle p, Wall w, double time) {
        super(time, new Particle[]{p});
        particle = p;
        wall = w;
    }

    @Override
    public void happen(ParticleEventHandler h) {
        Particle.collide(particle, wall);
        h.reactTo(this);
    }
}
