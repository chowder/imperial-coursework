package simulation;

public interface ParticleEventHandler {

    public void reactTo(Tick tick);

    public void reactTo(Collision c);

}


