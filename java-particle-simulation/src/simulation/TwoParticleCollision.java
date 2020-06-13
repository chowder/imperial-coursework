package simulation;

public class TwoParticleCollision extends Collision {

    private Particle particle_one;
    private Particle particle_two;

    public TwoParticleCollision(Particle one, Particle two, double time) {
        super(time, new Particle[]{one, two});
        particle_one = one;
        particle_two = two;
    }

    @Override
    public void happen(ParticleEventHandler h) {
        Particle.collide(particle_one, particle_two);
        h.reactTo(this);
    }
}
