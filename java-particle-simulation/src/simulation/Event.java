package simulation;

public interface Event extends Comparable<Event>{

    public double time();

    public void happen(ParticleEventHandler h);

    public boolean isValid();

}


